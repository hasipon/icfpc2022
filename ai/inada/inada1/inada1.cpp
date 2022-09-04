// inada1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#define ENABLE_GV 1
#define LOCAL_DEBUG 1

#include "inada1.h"
#define GV_JS
#include "gv.hpp"
#include "simulator.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cassert>

using namespace std;
namespace fs = std::filesystem;

int problemId;
string problemPamPath;
string inputIslPath;

int width, height;
uint8_t* image;
vector<string> inputIsl;


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
    ifstream ifs(path);
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
        gvLine4(p->bottomLeft.p[0], 400 - p->topRight.p[1], p->size.p[0], p->size.p[1], gvRGB(255, 64, 64, 128));
        gvText(p->bottomLeft.p[0] + p->size.p[0] / 2.0, 400 - p->topRight.p[1] + p->size.p[1] / 2.0, 3, gvRGB(255, 64, 64, 128), p->id.c_str());
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
            auto imageColor = hasiImageAt(image, Point(x, y));
            similarity += pixeDiff(canvasColor, imageColor);
        }
    }
    similarity *= 0.005;
    return std::lround(similarity);
}

void printBlockTree(const Block& b, const std::string& indent) {
    if (b.typ == SimpleBlockType) {
        auto p = reinterpret_cast<const SimpleBlock*>(&b);
        printf("%s(S:%s) BL(%d,%d) TR(%d,%d)\n", indent.c_str(), p->id.c_str(), p->bottomLeft.p[0], p->bottomLeft.p[1], p->topRight.p[0], p->topRight.p[1]);
    }
    if (b.typ == ComplexBlockType) {
        auto p = reinterpret_cast<const ComplexBlock*>(&b);
        printf("%s(C:%s) BL(%d,%d) TR(%d,%d) Childs:%d\n", indent.c_str(), p->id.c_str(), p->bottomLeft.p[0], p->bottomLeft.p[1], p->topRight.p[0], p->topRight.p[1], (int)p->subBlocks.size());
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

int solve() {
    Canvas c = Canvas(width, height);
    gvNewTime();
    gvCanvas(c);
    gvOutput("move:%d sim:%d", c.moveCost, calcSimilarity(c));

    int step = 0;

    for (int j = 1; j < 400; j += 10) {
        for (int i = 1; i < 400; i += 10) {
            auto canvasColor = c.C(Point(i, j));
            auto imageColor = hasiImageAt(image, Point(i, j));
            auto diff = pixeDiff(canvasColor, imageColor);

            if (64 <= diff) {
                printf("diff: %.2f\n", diff);

                string baseId = c.blocks.begin()->second.get()->id;
                c.PointCut(baseId, Point(i, j));
                gvNewTime();
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.ColorMove(baseId + ".2", imageColor);
                gvNewTime();
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".0", baseId + ".1");
                gvNewTime();
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".2", baseId + ".3");
                gvNewTime();
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(to_string(c.topLevelIdCounter), to_string(c.topLevelIdCounter - 1));
                gvNewTime();
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                step++;

                // auto sim = calcSimilarity(c);
                // gvOutput("move:%d sim:%d", c.moveCost, sim);
            }
        }
    }

    auto sim = calcSimilarity(c);
    gvOutput("move:%d sim:%d", c.moveCost, sim);

    return 0;
}

void evalIsl() {
    Canvas c = Canvas(width, height);
    gvNewTime();
    gvCanvas(c);
    gvOutput("move:%d sim:%d", c.moveCost, calcSimilarity(c));

    for (auto& move : inputIsl) {
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
}

void islResolve() {
    auto islCanvas = Canvas(width, height);
    for (auto& move : inputIsl) {
        if (move.empty()) continue;
        if (move[0] == '#') continue;
        applyMove(move, islCanvas);
    }
    gvNewTime();
    gvCanvas(islCanvas);


    auto c = Canvas(width, height);
    c.writeHistory = true;

    vector<pair<int, pair<int, int> > > orderedPos;
    for (int i = 0; i < 400; i++) {
        for (int j = 0; j < 400; j++) {
            orderedPos.emplace_back(0, make_pair(i, j));
        }
    }

    int orgX = 0;
    int orgY = 0;
    for (auto& p : orderedPos) {
        int i = p.second.first;
        int j = p.second.second;
        p.first = abs(i - orgX) + abs(j - orgY);
    }

    sort(orderedPos.begin(), orderedPos.end());

    for (auto& p : orderedPos) {
        Point at(p.second.first, p.second.second);

        if (c.C(at) != islCanvas.C(at)) {
            printf("%d,%d\n", at.p[0], at.p[1]);
            string baseId = c.blocks.begin()->second.get()->id;

            // first fill
            if (p.first == 0) {
                string baseId = c.blocks.begin()->second.get()->id;
                c.ColorMove(baseId, islCanvas.C(at));

                gvNewTime();
                gvOutput("First fill");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);
            }
            else if (at.p[0] == 0) {
                c.HorizontalCutCanvas(baseId, at.p[1]);

                gvNewTime();
                gvOutput("HorizontalCut");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.ColorMove(baseId + ".1", islCanvas.C(at));
                gvNewTime();
                gvOutput("colorMove");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".0", baseId + ".1");
                gvNewTime();
                gvOutput("mergeCanvas");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);
            }
            else if (at.p[1] == 0) {
                c.VerticalCutCanvas(baseId, at.p[0]);

                gvNewTime();
                gvOutput("VerticalCut");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.ColorMove(baseId + ".1", islCanvas.C(at));
                gvNewTime();
                gvOutput("colorMove");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".0", baseId + ".1");
                gvNewTime();
                gvOutput("mergeCanvas");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);
            }
            else if (at.isStrictlyInside(Point(0, 0), Point(400, 400))) {
                c.PointCut(baseId, at);

                gvNewTime();
                gvOutput("pointCut");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.ColorMove(baseId + ".2", islCanvas.C(at));
                gvNewTime();
                gvOutput("colorMove");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".0", baseId + ".1");
                gvNewTime();
                gvOutput("mergeCanvas");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(baseId + ".2", baseId + ".3");
                gvNewTime();
                gvOutput("mergeCanvas");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);

                c.MergeCanvas(to_string(c.topLevelIdCounter), to_string(c.topLevelIdCounter - 1));
                gvNewTime();
                gvOutput("mergeCanvas");
                gvOutput("moveCost:%d", c.moveCost);
                gvCanvas(c);
            }
            else {
                assert(false);
            }
        }
    }

    for (int i = 0; i < 400; i++) {
        for (int j = 0; j < 400; j++) {
            Point at(i, j);
            assert(c.C(at) == islCanvas.C(at));
        }
    }

    printf("ok\n");

    if (c.moveCost < islCanvas.moveCost) {
        auto fileName = to_string(problemId) + "-inada1-" + to_string(c.moveCost) + ".isl";
        printf("%d -> %d output:%s", islCanvas.moveCost, c.moveCost, fileName.c_str());
        ofstream ofs(fileName);
        for (auto& line : c.history) {
            ofs << line << endl;
        }
    }
}


void readInputs() {
    problemId = 10;
    problemPamPath = "../../problems.pam/" + to_string(problemId) + ".pam";
    inputIslPath = "../../solutions.best/" + to_string(problemId) + ".isl";

    if (getenv("PROBLEM_ID")) {
        assert(getenv("PROBLEM_PAM"), "PROBLEM_PAM is not set");
        assert(getenv("INPUT_ISL"), "INPUT_ISL is not set");
        problemId = atoi(getenv("PROBLEM_ID"));
        problemPamPath = getenv("PROBLEM_PAM");
        inputIslPath = getenv("INPUT_ISL");
    }
    else {
        printf("[WARN] PROBLEM_ID is not set");
    }

    FILE* fp = fopen(problemPamPath.c_str(), "rb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    image = hasiImageRead(fp, width, height);
    fclose(fp);

    inputIsl = readIsl(inputIslPath.c_str());
}

int main(int argc, char* argv[]) {
#if LOCAL_DEBUG
    fs::current_path(R"(c:\projects\icfpc2022\ai\inada)");
#endif

    readInputs();

    islResolve();
}

