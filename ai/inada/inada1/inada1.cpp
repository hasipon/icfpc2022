#define LOCAL_DEBUG 1
#define ENABLE_GV 0

#include "inada1.h"
#define GV_JS
#include "gv.hpp"
#include "simulator.hpp"
#include <limits.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <algorithm>

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

/*
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
*/

bool pointMergeCheck(int size0, int size1, int size2, int size3) {
    int cost1 = std::lround(MergeCost * height * width / std::max(size0, size1))
        + std::lround(MergeCost * height * width / std::max(size2, size3))
        + std::lround(MergeCost * height * width / std::max(size0 + size1, size2 + size3));
    int cost2 = std::lround(MergeCost * height * width / std::max(size1, size2))
        + std::lround(MergeCost * height * width / std::max(size0, size3))
        + std::lround(MergeCost * height * width / std::max(size1 + size2, size0 + size3));
    return cost1 <= cost2;
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

    vector<pair<int, pair<int, int> > > orderedPos;
    for (int i = 0; i < 400; i++) {
        for (int j = 0; j < 400; j++) {
            orderedPos.emplace_back(0, make_pair(i, j));
        }
    }

    struct Case {
        string name;
        string sortOrder;
        int startX;
        int startY;
        int hCutChild;
        int vCutChild;
        int pCutChild;
    };

    vector<Case> cases = {
        {"bottomLeft-to-topRight", "xy", 0, 0, 1, 1, 2},
        // {"bottomLeft-to-topRight2", "xy2", 0, 0, 1, 1, 2},
        {"topRight-to-bottomLeft", "xy", 400 - 1, 400 - 1, 0, 0, 0},
        {"bottomRight-to-topLeft", "xy", 400 - 1, 0, 1, 0, 3},
        {"topLeft-to-bottomRight", "xy", 0, 400 - 1, 0, 1, 1},
    };

    int bestCost = INT_MAX;
    vector<string> bestHistory;

    for (auto& testCase : cases) {
        auto c = Canvas(width, height);
        c.writeHistory = true;

        // printf("%s\n", testCase.name.c_str());

        for (auto& p : orderedPos) {
            int i = p.second.first;
            int j = p.second.second;

            if (testCase.sortOrder == "xy") {
                p.first = abs(i - testCase.startX) + abs(j - testCase.startY);
            }
            if (testCase.sortOrder == "xy2") {
                p.first = abs(i - testCase.startX) * abs(i - testCase.startX) + abs(j - testCase.startY) * abs(j - testCase.startY);
            }
            if (testCase.sortOrder == "x") {
                p.first = abs(i - testCase.startX);
            }
            if (testCase.sortOrder == "y") {
                p.first = abs(j - testCase.startY);
            }
        }

        sort(orderedPos.begin(), orderedPos.end());

        for (auto& p : orderedPos) {
            Point at(p.second.first, p.second.second);
            auto targetColor = islCanvas.C(at);

            if (c.C(at) != targetColor) {
                // printf("%d,%d\n", at.p[0], at.p[1]);
                // printf("c.C[%d,%d,%d,%d]", c.C(at)[0], c.C(at)[1], c.C(at)[2], c.C(at)[3]);
                // printf("i.C[%d,%d,%d,%d]", islCanvas.C(at)[0], islCanvas.C(at)[1], islCanvas.C(at)[2], islCanvas.C(at)[3]);
                string baseId = c.blocks.begin()->second.get()->id;
                auto hCutLine = testCase.hCutChild == 1 ? at.p[1] : at.p[1] + 1;
                auto vCutLine = testCase.vCutChild == 1 ? at.p[0] : at.p[0] + 1;
                auto pCutPoint = at;
                if (testCase.pCutChild == 0) pCutPoint = at.add(Point(1, 1));
                if (testCase.pCutChild == 1) pCutPoint = at.add(Point(0, 1));
                if (testCase.pCutChild == 3) pCutPoint = at.add(Point(1, 0));

                if (pCutPoint.isStrictlyInside(Point(0, 0), Point(400, 400))) {
                    c.PointCut(baseId, pCutPoint);

                    gvNewTime();
                    gvOutput("pointCut");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    c.ColorMove(baseId + "." + to_string(testCase.pCutChild), targetColor);
                    gvNewTime();
                    gvOutput("colorMove");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    auto id0 = baseId + ".0";
                    auto id1 = baseId + ".1";
                    auto id2 = baseId + ".2";
                    auto id3 = baseId + ".3";
                    int size0 = c.blocks[id0]->size.getScalarSize();
                    int size1 = c.blocks[id1]->size.getScalarSize();
                    int size2 = c.blocks[id2]->size.getScalarSize();
                    int size3 = c.blocks[id3]->size.getScalarSize();
                    if (pointMergeCheck(size0, size1, size2, size3)) {
                        c.MergeCanvas(id0, id1);
                        c.MergeCanvas(id2, id3);
                    }
                    else {
                        c.MergeCanvas(id1, id2);
                        c.MergeCanvas(id0, id3);
                    }

                    c.MergeCanvas(to_string(c.topLevelIdCounter), to_string(c.topLevelIdCounter - 1));
                    gvNewTime();
                    gvOutput("mergeCanvas");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    assert(c.C(at) == islCanvas.C(at));
                }
                else if (0 < hCutLine && hCutLine < 400) {
                    c.HorizontalCutCanvas(baseId, hCutLine);

                    gvNewTime();
                    gvOutput("HorizontalCut");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    c.ColorMove(baseId + "." + to_string(testCase.hCutChild), targetColor);
                    gvNewTime();
                    gvOutput("colorMove");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    c.MergeCanvas(baseId + ".0", baseId + ".1");
                    gvNewTime();
                    gvOutput("mergeCanvas");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    assert(c.C(at) == islCanvas.C(at));
                }
                else if (0 < vCutLine && vCutLine < 400) {
                    c.VerticalCutCanvas(baseId, vCutLine);

                    gvNewTime();
                    gvOutput("VerticalCut");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    c.ColorMove(baseId + "." + to_string(testCase.vCutChild), targetColor);
                    gvNewTime();
                    gvOutput("colorMove");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    c.MergeCanvas(baseId + ".0", baseId + ".1");
                    gvNewTime();
                    gvOutput("mergeCanvas");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);

                    assert(c.C(at) == islCanvas.C(at));
                }
                else {
                    c.ColorMove(baseId, targetColor);
                    // printf("default color move %d,%d\n", at.p[0], at.p[1]);
                    gvNewTime();
                    gvOutput("colorMove");
                    gvOutput("moveCost:%d", c.moveCost);
                    gvCanvas(c);
                }

                if (c.C(at) != islCanvas.C(at)) {
                    throw 1;
                }
            }
        }

        // cは壊れる事に注意
        while (c.moveHistory.back().substr(0, 5) == "merge") {
            c.moveHistory.pop_back();
            c.costHistory.pop_back();
        }
        c.moveCost = c.costHistory.back();

        printf("%s: %d->%d\n", testCase.name.c_str(), islCanvas.moveCost, c.moveCost);
        if (c.moveCost < bestCost) {
            bestCost = c.moveCost;
            bestHistory = c.moveHistory;
        }
    }

    if (bestCost < islCanvas.moveCost) {
        auto simCost = calcSimilarity(islCanvas);
        auto fileName = to_string(problemId) + "-inada1-" + to_string(bestCost + simCost) + ".isl";
        printf("%d -> %d output:%s\n", islCanvas.moveCost + simCost, bestCost + simCost, fileName.c_str());
        ofstream ofs(fileName);
        for (auto& line : bestHistory) {
            ofs << line << endl;
        }
    }
}


void readInputs() {
    problemId = 3;
    problemPamPath = "../../problems.pam/" + to_string(problemId) + ".pam";
    inputIslPath = "../../solutions.best/" + to_string(problemId) + ".isl";

    if (getenv("PROBLEM_ID")) {
        if (!getenv("PROBLEM_PAM")) {
            printf("PROBLEM_PAM is not set\n");
            exit(1);
        }
        if (!getenv("INPUT_ISL")) {
            printf("INPUT_ISL is not set\n");
            exit(1);
        }
        problemId = atoi(getenv("PROBLEM_ID"));
        problemPamPath = getenv("PROBLEM_PAM");
        inputIslPath = getenv("INPUT_ISL");
    }
    else {
        printf("[WARN] PROBLEM_ID is not set\n");
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

    // evalIsl();

    islResolve();
}

