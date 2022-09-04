// inada1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "inada1.h"
#define GV_JS
#include "gv.hpp"
#include "simulator.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

int width, height;
uint8_t* image;

#define LOCAL_DEBUG 1

void applyMove(std::string line, Canvas& c) {
    std::stringstream ss;
    ss << line;

    std::string move;
    ss >> move;
    if (move == "cut") {
        auto block = read_block(ss);
        std::string orient_or_point;
        ss >> orient_or_point;
        if (orient_or_point.back() != ']') {
            std::string next;
            ss >> next;
            orient_or_point += next;
        }

        if (orient_or_point == "[X]" || orient_or_point == "[x]") {
            int line_number = read_line_number(ss);
            c.VerticalCutCanvas(block, line_number);
        }
        else if (orient_or_point == "[Y]" || orient_or_point == "[y]") {
            int line_number = read_line_number(ss);
            c.HorizontalCutCanvas(block, line_number);
        }
        else {
            auto p = read_point(orient_or_point);
            c.PointCut(block, p);
        }
    }
    if (move == "color") {
        auto block = read_block(ss);
        auto color = read_color(ss);

        c.ColorMove(block, color);
    }
    if (move == "swap") {
        auto block1 = read_block(ss);
        auto block2 = read_block(ss);

        c.SwapCanvas(block1, block2);
    }
    if (move == "merge") {
        auto block1 = read_block(ss);
        auto block2 = read_block(ss);

        c.MergeCanvas(block1, block2);
    }
}

vector<string> readIsl(string path) {
    vector<string> result;
    ifstream ifs("1.isl");
    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty()) {
            result.emplace_back(line);
        }
    }
    return result;
}

GV_RGB gvColor(const Color& c) {
    return gvRGB(c[0], c[1], c[2], c[3]);
}

void gvLine4(double x, double y, double w, double h, GV_RGB rgb) {
    gvLine(x, y, x + w, y, rgb);
    gvLine(x + w, y, x + w, y + h, rgb);
    gvLine(x + w, y + h, x, y + h, rgb);
    gvLine(x, y + h, x, y, rgb);
}

void gvBlock(const Block& b) {
    if (b.typ == SimpleBlockType) {
        auto p = reinterpret_cast<const SimpleBlock*>(&b);
        gvRect(p->bottomLeft.p[0], 400 - p->topRight.p[1], p->size.p[0], p->size.p[1], gvColor(p->color));
    }
    if (b.typ == ComplexBlockType) {
        auto p = reinterpret_cast<const ComplexBlock*>(&b);
        for (auto& block : p->subBlocks) {
            gvBlock(block);
        }
    }
}

void gvOverlay(const Block& b) {
    if (b.typ == SimpleBlockType) {
        auto p = reinterpret_cast<const SimpleBlock*>(&b);
        gvLine4(p->bottomLeft.p[0], 400 - p->topRight.p[1], p->size.p[0], p->size.p[1], gvRGB(255, 64, 64, 128));
        gvText(p->bottomLeft.p[0] + p->size.p[0] / 2.0, 400 - p->topRight.p[1] + p->size.p[1] / 2.0, 3, gvRGB(255, 64, 64, 128), p->id.c_str());
    }
    if (b.typ == ComplexBlockType) {
        auto p = reinterpret_cast<const ComplexBlock*>(&b);
        for (auto& block : p->subBlocks) {
            gvOverlay(block);
        }
    }
}

void gvCanvas(Canvas& c) {
    for (auto& block : c.blocks) {
        gvBlock(*block.second);
    }
    for (auto& block : c.blocks) {
        gvOverlay(*block.second);
    }
}

int calcSimilarity(Canvas& c) {
    double similarity = 0;
    for (int x = 0; x < 400; ++x) {
        for (int y = 0; y < 400; ++y) {
            auto canvasColor = c.C(Point(x, y));
            auto imageColor = hasiImageAt(image, width, height, Point(x, y));
            similarity += pixeDiff(canvasColor, imageColor);
        }
    }
    similarity *= 0.005;
    return std::lround(similarity);
}

void printBlockTree(const Block& b, const std::string& indent) {
    if (b.typ == SimpleBlockType) {
        auto p = reinterpret_cast<const SimpleBlock*>(&b);
        printf("%s(S) BL(%d,%d) TR(%d,%d)\n", indent.c_str(), p->bottomLeft.p[0], p->bottomLeft.p[1], p->topRight.p[0], p->topRight.p[1]);
    }
    if (b.typ == ComplexBlockType) {
        auto p = reinterpret_cast<const ComplexBlock*>(&b);
        printf("%s(C) BL(%d,%d) TR(%d,%d) Childs:%d\n", indent.c_str(), p->bottomLeft.p[0], p->bottomLeft.p[1], p->topRight.p[0], p->topRight.p[1], (int)p->subBlocks.size());
        for (auto& block : p->subBlocks) {
            printBlockTree(block, indent + " ");
        }
    }
}

void printCanvasTree(Canvas& c) {
    printf("Canvas:\n");
    for (const auto& block : c.blocks) {
        printBlockTree(*block.second.get(), "");
    }
}

int main(int argc, char* argv[]) {
    std::string line;

#if LOCAL_DEBUG
    fs::current_path(R"(c:\projects\icfpc2022\ai\inada)");
#endif


    if (getenv("PROBLEM_ID")) {
        auto pamPath = "../../problems.pam/" + string(getenv("PROBLEM_ID")) + ".pam";
        FILE* fp = fopen(pamPath.c_str(), "rb");
        if (fp == NULL) {
            printf("failed to open pam");
            exit(EXIT_FAILURE);
        }
        image = hasiImageRead(fp, width, height);
        fclose(fp);
    }
    else {
        FILE* fp = fopen("1.pam", "rb");
        if (fp == NULL) {
            printf("failed to open 1.pam");
            exit(EXIT_FAILURE);
        }
        image = hasiImageRead(fp, width, height);
        fclose(fp);
    }

    vector<string> moves;
    if (getenv("ISL_FILE")) {
        moves = readIsl(getenv("ISL_FILE"));
    }
    else {
        moves = readIsl("1.isl");
    }

    Canvas c = Canvas(width, height);
    gvNewTime();
    gvCanvas(c);
    gvOutput("move:%d sim:%d", c.moveCost, calcSimilarity(c));

    for (auto& move : moves) {
        gvNewTime();
        applyMove(move, c);
        gvOutput(move.c_str());
        auto sim = calcSimilarity(c);
        gvOutput("move:%d sim:%d", c.moveCost, sim);
        gvCanvas(c);
        printf("%s", move.c_str());
        printf("move:%d sim:%d", c.moveCost, sim);
        printCanvasTree(c);
    }

    return 0;
}
