#include <iostream>
#include <sstream>

#include <cstdio>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <cmath>

constexpr int ColorCost = 5;
constexpr int VerticalCutCost = 7;
constexpr int HorizontalCutCost = 7;
constexpr int PointCutCost = 10;
constexpr int SwapCost = 3;
constexpr int MergeCost = 1;

using Color = std::array<uint8_t, 4>;
inline double pixeDiff(Color lhs, Color rhs) {
    int sum = 0;
    for (size_t i = 0; i < std::tuple_size<Color>::value; ++i) {
        sum += (int(lhs[i]) - int(rhs[i])) * (int(lhs[i]) - int(rhs[i]));
    }
    return std::sqrt(sum);
}

class Point {
public:
    Point();
    Point(uint16_t, uint16_t);

    std::array<uint16_t, 2> p;

    Point getDiff(Point) const;
    bool isStrictlyInside(Point, Point) const;
    bool isOnBoundary(Point, Point) const;
    bool isInside(Point, Point) const;
    int getScalarSize() const;
    Point add(Point otherPoint) const;
    Point subtract(Point otherPoint) const;
};

Point::Point() : p({0, 0}) {}

Point::Point(uint16_t x, uint16_t y) : p({x, y}) {}

Point Point::getDiff(Point other) const {
    uint16_t newX = this->p[0] - other.p[0];
    uint16_t newY = this->p[1] - other.p[1];

    if (newX < 0) {
        newX = 0;
    }
    if (newY < 0) {
        newY = 0;
    }

    return Point{newX, newY};
}

bool Point::isStrictlyInside(Point bottomLeft, Point topRight) const {
    return bottomLeft.p[0] < this->p[0] &&
           this->p[0] < topRight.p[0] &&
           bottomLeft.p[1] < this->p[1] &&
           this->p[1] < topRight.p[1];
}

bool Point::isOnBoundary(Point bottomLeft, Point topRight) const {
    return (bottomLeft.p[0] == this->p[0] && bottomLeft.p[1] <= this->p[1] && this->p[1] <= topRight.p[1])
           || (topRight.p[0] == this->p[0] && bottomLeft.p[1] <= this->p[1] && this->p[1] <= topRight.p[1])
           || (bottomLeft.p[1] == this->p[1] && bottomLeft.p[0] <= this->p[0] && this->p[0] <= topRight.p[0])
           || (topRight.p[1] == this->p[1] && bottomLeft.p[0] <= this->p[0] && this->p[0] <= topRight.p[0]);
}

bool Point::isInside(Point bottomLeft, Point topRight) const {
    return this->isStrictlyInside(bottomLeft, topRight) || this->isOnBoundary(bottomLeft, topRight);
}

int Point::getScalarSize() const {
    return this->p[0] * this->p[1];
}

Point Point::add(Point otherPoint) const {
    return Point(this->p[0] + otherPoint.p[0], this->p[1] + otherPoint.p[1]);
}

Point Point::subtract(Point otherPoint) const {
    return Point(this->p[0] - otherPoint.p[0], this->p[1] - otherPoint.p[1]);
}

enum BlockType {
    SimpleBlockType = 0,
    ComplexBlockType = 1
};

class Block {
public:
    std::string id;
    BlockType typ;
    Point bottomLeft;
    Point topRight;
    Point size;

    virtual std::vector<Block*> getChildren() { return {}; };
    virtual ~Block() {}
};

class SimpleBlock : public Block {
public:
    Color color;

    SimpleBlock(std::string id, Point bottomLeft, Point topRight, Color color);
    virtual std::vector<Block*> getChildren();
};

SimpleBlock::SimpleBlock(std::string id, Point bottomLeft, Point topRight, Color color) {
    this->typ = SimpleBlockType;
    this->id = id;
    this->bottomLeft = bottomLeft;
    this->topRight = topRight;
    this->size = topRight.getDiff(bottomLeft);
    this->color = color;

    /*
    std::cout << id << "(" << bottomLeft.p[0] << "," <<bottomLeft.p[1] <<")("<< topRight.p[0] << "," <<  topRight.p[1] << ")" << std::endl;
    std::cout << "    " << int(color[0]) << "\t" << int(color[1]) << "\t" << int(color[2]) << "\t" << int(color[3]) << "\t" << std::endl;
    */

    if(this->bottomLeft.p[0] > this->topRight.p[0] || this->bottomLeft.p[1] > this->topRight.p[1]) {
        printf("Invalid Block");
        __debugbreak();
    }

    if (this->size.getScalarSize() == 0 ){
        printf("Block size cannot be 0!");
        __debugbreak();
    }
}

std::vector<Block *> SimpleBlock::getChildren() {
    return std::vector<Block *>{this};
}

class ComplexBlock : public Block {
public:
    std::vector<SimpleBlock> subBlocks;

    ComplexBlock(std::string id, Point bottomLeft, Point topRight, std::vector<SimpleBlock> subBlocks);
    virtual std::vector<Block*> getChildren();
    std::vector<SimpleBlock> offsetChildren(Point) const;
};

ComplexBlock::ComplexBlock(std::string id, Point bottomLeft, Point topRight, std::vector<SimpleBlock> subBlocks) {
    this->typ = ComplexBlockType;
    this->id = id;
    this->bottomLeft = bottomLeft;
    this->topRight = topRight;
    this->size = topRight.getDiff(bottomLeft);
    this->subBlocks = subBlocks;

    if (this->size.getScalarSize() == 0) {
        printf("Block size cannot be 0!");
        exit(EXIT_FAILURE);
    }
}

std::vector<Block *> ComplexBlock::getChildren() {
    std::vector<Block *> res;
    for (SimpleBlock &b: subBlocks) {
        res.push_back(&b);
    }

    return res;
}
std::vector<SimpleBlock> ComplexBlock::offsetChildren(Point newBottomLeft) const {
    std::vector<SimpleBlock> newChildren;

    for (const auto &block: subBlocks) {
        newChildren.emplace_back(
                "child",
                block.bottomLeft.add(newBottomLeft).subtract(bottomLeft),
                block.topRight.add(newBottomLeft).subtract(bottomLeft),
                block.color
        );
    }

    return newChildren;
}

class Canvas {
public:
    Canvas(int x, int y) : width(x), height(y) {
        blocks["0"] = std::unique_ptr<Block>(
                new SimpleBlock("0", Point(0, 0), Point(width, height), Color({255, 255, 255, 255}))
        );
    }

    void ColorMove(std::string blockId, Color color);
    void PointCut(std::string blockId, Point point);
    void VerticalCutCanvas(std::string blockId, int lineNumber);
    void HorizontalCutCanvas(std::string blockId, int lineNumber);
    void SwapCanvas(std::string blockId1, std::string blockId2);
    void MergeCanvas(std::string blockId1, std::string blockId2);

    Color C(Point at) const;

    std::unordered_map<std::string, std::unique_ptr<Block>> blocks;

    int topLevelIdCounter = 0;
    int moveCost = 0;
    int width;
    int height;
};

Color Canvas::C(Point at) const {
    if (width < at.p[0] || height < at.p[1]) {
        printf("at (%d, %d) is out-of-bound (width: %d, height: %d)\n", at.p[0], at.p[1], width, height);
        exit(EXIT_FAILURE);
    }

    for (const auto &block: blocks) {
        if (block.second->typ == SimpleBlockType) {
            const auto &simpleBlockRef = *dynamic_cast<SimpleBlock *>(block.second.get());
            if (simpleBlockRef.bottomLeft.p[0] <= at.p[0] && at.p[0] < simpleBlockRef.topRight.p[0] &&
                simpleBlockRef.bottomLeft.p[1] <= at.p[1] && at.p[1] < simpleBlockRef.topRight.p[1]) {
                return simpleBlockRef.color;
            }
        } else {
            const auto &complexBlockRef = *dynamic_cast<ComplexBlock *>(block.second.get());
            for (const auto &simpleBlockRef: complexBlockRef.subBlocks) {
                if (simpleBlockRef.bottomLeft.p[0] <= at.p[0] && at.p[0] < simpleBlockRef.topRight.p[0] &&
                    simpleBlockRef.bottomLeft.p[1] <= at.p[1] && at.p[1] < simpleBlockRef.topRight.p[1]) {
                    return simpleBlockRef.color;
                }
            }
        }

        /*
        if (at.isInside(block.second->bottomLeft, block.second->topRight)) {
            if (block.second->typ == SimpleBlockType) {
                const auto &simpleBlockRef = *dynamic_cast<SimpleBlock *>(block.second.get());
                std::cout << simpleBlockRef.id << " ";
                return simpleBlockRef.color;
            } else {
                const auto &complexBlockRef = *dynamic_cast<ComplexBlock *>(block.second.get());
                for (const auto &c: complexBlockRef.subBlocks) {
                    if (at.isInside(c.bottomLeft, c.topRight)) {
                        return c.color;
                    }
                }
                printf("at (%d, %d) is out-of-bound block %s (bottomLeft: (%d,%d), topRight: (%d,%d))\n", at.p[0], at.p[1],
                       block.second->id.c_str(),
                       block.second->bottomLeft.p[0], block.second->bottomLeft.p[1],
                       block.second->topRight.p[0], block.second->topRight.p[1]
                );
                exit(EXIT_FAILURE);
            }
        }
        */
    }

    printf("at (%d, %d) is out-of-bound (strict) (width: %d, height: %d)\n", at.p[0], at.p[1], width, height);
    exit(EXIT_FAILURE);
}

void Canvas::ColorMove(std::string blockId, Color color) {
    auto &block = blocks[blockId];
    moveCost += std::lround(ColorCost * height * width / double(block->size.getScalarSize()));

    if (block->typ == SimpleBlockType) {
        SimpleBlock &actualBlock = *dynamic_cast<SimpleBlock *>(block.get());
        actualBlock.color = color;
        return;
    }

    if (block->typ == ComplexBlockType) {
        auto newBlock = std::make_unique<SimpleBlock>(block->id, block->bottomLeft, block->topRight, color);
        blocks.erase(blockId);
        blocks[blockId] = std::move(newBlock);
        return;
    }
}

void Canvas::PointCut(std::string blockId, Point point) {
    auto &block = blocks[blockId];
    moveCost += std::lround(PointCutCost * height * width / double(block->size.getScalarSize()));

    if (!point.isStrictlyInside(block->bottomLeft, block->topRight)) {
        printf("Point (%d, %d) is out of the block %s (%d, %d) to (%d, %d)\n", point.p[0], point.p[1], blockId.c_str(),
               block->bottomLeft.p[0], block->bottomLeft.p[1], block->topRight.p[0], block->topRight.p[1]);
        exit(EXIT_FAILURE);
    }

    if (block->typ == SimpleBlockType) {
        auto bottomLeftBlock = std::make_unique<SimpleBlock>(
                blockId + ".0",
                block->bottomLeft,
                point,
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        auto bottomRightBlock = std::make_unique<SimpleBlock>(
                blockId + ".1",
                Point(point.p[0], block->bottomLeft.p[1]),
                Point(block->topRight.p[0], point.p[1]),
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        auto topRightBlock = std::make_unique<SimpleBlock>(
                blockId + ".2",
                point,
                block->topRight,
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        auto topLeftBlock = std::make_unique<SimpleBlock>(
                blockId + ".3",
                Point(block->bottomLeft.p[0], point.p[1]),
                Point(point.p[0], block->topRight.p[1]),
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        blocks.erase(blockId);
        blocks[bottomLeftBlock->id] = std::move(bottomLeftBlock);
        blocks[bottomRightBlock->id] = std::move(bottomRightBlock);
        blocks[topRightBlock->id] = std::move(topRightBlock);
        blocks[topLeftBlock->id] = std::move(topLeftBlock);

        return;
    }

    if (block->typ == ComplexBlockType) {
        std::vector<SimpleBlock> bottomLeftBlocks, bottomRightBlocks, topRightBlocks, topLeftBlocks;
        auto &subBlocks = dynamic_cast<ComplexBlock *>(block.get())->subBlocks;
        for (const auto &subBlock: subBlocks) {
            /**
             * __________________________
             * |        |       |       |
             * |   1    |   2   |   3   |
             * |________|_______|_______|
             * |        |       |       |
             * |   4    |   5   |  6    |
             * |________|_______|_______|
             * |        |       |       |
             * |   7    |   8   |   9   |
             * |________|_______|_______|
             */
            // Case 2
            if (subBlock.bottomLeft.p[0] >= point.p[0] && subBlock.bottomLeft.p[1] >= point.p[1]) {
                topRightBlocks.push_back(subBlock);
                continue;
            }
            // Case 7
            if (subBlock.topRight.p[0] <= point.p[0] && subBlock.topRight.p[1] <= point.p[1]) {
                bottomLeftBlocks.push_back(subBlock);
                continue;
            }
            // Case 1
            if (subBlock.topRight.p[0] <= point.p[0] && subBlock.bottomLeft.p[1] >= point.p[1]) {
                topLeftBlocks.push_back(subBlock);
                continue;
            }
            // Case 9
            if (subBlock.bottomLeft.p[0] >= point.p[0] && subBlock.topRight.p[1] <= point.p[1]) {
                bottomRightBlocks.push_back(subBlock);
                continue;
            }
            // Case 5
            if (point.isStrictlyInside(subBlock.bottomLeft, subBlock.topRight)) {
                bottomLeftBlocks.emplace_back(
                        "bl_child",
                        subBlock.bottomLeft,
                        point,
                        subBlock.color
                );
                bottomRightBlocks.emplace_back(
                        "br_child",
                        Point(point.p[0], subBlock.bottomLeft.p[1]),
                        Point(subBlock.topRight.p[0], point.p[1]),
                        subBlock.color
                );
                topRightBlocks.emplace_back(
                        "tr_child",
                        point,
                        subBlock.topRight,
                        subBlock.color
                );
                topLeftBlocks.emplace_back(
                        "tl_child",
                        Point(subBlock.bottomLeft.p[0], point.p[1]),
                        Point(point.p[0], subBlock.topRight.p[1]),
                        subBlock.color
                );
                continue;
            }

            // Case 2
            if (subBlock.bottomLeft.p[0] <= point.p[0]
                && point.p[0] <= subBlock.topRight.p[0]
                && point.p[1] <= subBlock.bottomLeft.p[1]) {
                topLeftBlocks.emplace_back(
                        "case2_tl_child",
                        subBlock.bottomLeft,
                        Point(point.p[0], subBlock.topRight.p[1]),
                        subBlock.color
                );
                topRightBlocks.emplace_back(
                        "case2_tr_child",
                        Point(point.p[0], subBlock.bottomLeft.p[1]),
                        subBlock.topRight,
                        subBlock.color
                );
                continue;
            }
            // Case 8
            if (subBlock.bottomLeft.p[0] <= point.p[0]
                && point.p[0] <= subBlock.topRight.p[0]
                && point.p[1] >= subBlock.topRight.p[1]) {
                bottomLeftBlocks.emplace_back(
                        "case8_bl_child",
                        subBlock.bottomLeft,
                        Point(point.p[0], subBlock.topRight.p[1]),
                        subBlock.color
                );
                bottomRightBlocks.emplace_back(
                        "case8_br_child",
                        Point(point.p[0], subBlock.bottomLeft.p[1]),
                        subBlock.topRight,
                        subBlock.color
                );
                continue;
            }
            // Case 4
            if (subBlock.bottomLeft.p[1] <= point.p[1]
                && point.p[1] <= subBlock.topRight.p[1]
                && point.p[0] <= subBlock.bottomLeft.p[0]) {
                bottomRightBlocks.emplace_back(
                        "case4_br_child",
                        subBlock.bottomLeft,
                        Point(subBlock.topRight.p[0], point.p[1]),
                        subBlock.color
                );
                topRightBlocks.emplace_back(
                        "case4_tr_child",
                        Point(subBlock.bottomLeft.p[0], point.p[1]),
                        subBlock.topRight,
                        subBlock.color
                );
                continue;
            }
            // Case 6
            if (subBlock.bottomLeft.p[1] <= point.p[1]
                && point.p[1] <= subBlock.topRight.p[1]
                && point.p[0] >= subBlock.topRight.p[0]) {
                bottomLeftBlocks.emplace_back(
                        "case6_bl_child",
                        subBlock.bottomLeft,
                        Point(subBlock.topRight.p[0], point.p[1]),
                        subBlock.color
                );
                topLeftBlocks.emplace_back(
                        "case6_br_child",
                        Point(subBlock.bottomLeft.p[0], point.p[1]),
                        subBlock.topRight,
                        subBlock.color
                );
                continue;
            }
        };
        auto bottomLeftBlock = std::make_unique<ComplexBlock>(
                blockId + ".0",
                block->bottomLeft,
                point,
                bottomLeftBlocks
        );
        auto bottomRightBlock = std::make_unique<ComplexBlock>(
                blockId + ".1",
                Point(point.p[0], block->bottomLeft.p[1]),
                Point(block->topRight.p[0], point.p[1]),
                bottomRightBlocks
        );
        auto topRightBlock = std::make_unique<ComplexBlock>(
                blockId + ".2",
                point,
                block->topRight,
                topRightBlocks
        );
        auto topLeftBlock = std::make_unique<ComplexBlock>(
                blockId + ".3",
                Point(block->bottomLeft.p[0], point.p[1]),
                Point(point.p[0], block->topRight.p[1]),
                topLeftBlocks
        );
        blocks.erase(blockId);
        blocks[blockId + ".0"] = std::move(bottomLeftBlock);
        blocks[blockId + ".1"] = std::move(bottomRightBlock);
        blocks[blockId + ".2"] = std::move(topRightBlock);
        blocks[blockId + ".3"] = std::move(topLeftBlock);

        return;
    }
}

void Canvas::VerticalCutCanvas(std::string blockId, int lineNumber) {
    auto &block = blocks[blockId];
    moveCost += std::lround(VerticalCutCost * height * width / double(block->size.getScalarSize()));

    if (!(block->bottomLeft.p[0] <= lineNumber && lineNumber <= block->topRight.p[0])) {
        printf("Line number is out of the [%s]! Block is from (%d, %d) to (%d, %d), point is at %d!", blockId.c_str(),
               block->bottomLeft.p[0], block->bottomLeft.p[1], block->topRight.p[0], block->topRight.p[1], lineNumber);
        exit(EXIT_FAILURE);
    }

    if (block->typ == SimpleBlockType) {
        auto leftBlock = std::make_unique<SimpleBlock>(
                blockId + ".0",
                block->bottomLeft,
                Point(lineNumber, block->topRight.p[1]),
                dynamic_cast<SimpleBlock *>(block.get())->color
        );
        auto rightBlock = std::make_unique<SimpleBlock>(
                blockId + ".1",
                Point(lineNumber, block->bottomLeft.p[1]),
                block->topRight,
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        blocks.erase(blockId);
        blocks[blockId + ".0"] = std::move(leftBlock);
        blocks[blockId + ".1"] = std::move(rightBlock);
        return;
    }

    if (block->typ == ComplexBlockType) {
        std::vector<SimpleBlock> leftBlocks, rightBlocks;
        auto &subBlocks = dynamic_cast<ComplexBlock *>(block.get())->subBlocks;
        for (const auto &subBlock: subBlocks) {
            if (subBlock.bottomLeft.p[0] >= lineNumber) {
                rightBlocks.push_back(subBlock);
                continue;
            }
            if (subBlock.topRight.p[0] <= lineNumber) {
                leftBlocks.push_back(subBlock);
                continue;
            }
            leftBlocks.emplace_back(
                    "child",
                    subBlock.bottomLeft,
                    Point(lineNumber, subBlock.topRight.p[1]),
                    subBlock.color
            );
            rightBlocks.emplace_back(
                    "child",
                    Point(lineNumber, subBlock.bottomLeft.p[1]),
                    subBlock.topRight,
                    subBlock.color
            );
        }

        auto leftBlock = std::make_unique<ComplexBlock>(
                blockId + ".0",
                block->bottomLeft,
                Point(lineNumber, block->topRight.p[1]),
                leftBlocks
        );
        auto rightBlock = std::make_unique<ComplexBlock>(
                blockId + ".1",
                Point(lineNumber, block->bottomLeft.p[1]),
                block->topRight,
                rightBlocks
        );

        blocks.erase(blockId);
        blocks[blockId + ".0"] = std::move(leftBlock);
        blocks[blockId + ".1"] = std::move(rightBlock);
        return;
    }
}

void Canvas::HorizontalCutCanvas(std::string blockId, int lineNumber) {
    auto &block = blocks[blockId];
    moveCost += std::lround(HorizontalCutCost * height * width / double(block->size.getScalarSize()));

    if (!(block->bottomLeft.p[1] <= lineNumber && lineNumber <= block->topRight.p[1])) {
        printf("Line number is out of the [%s]! Block is from (%d, %d) to (%d, %d), point is at %d!", blockId.c_str(),
               block->bottomLeft.p[0], block->bottomLeft.p[1], block->topRight.p[0], block->topRight.p[1], lineNumber);
        exit(EXIT_FAILURE);
    }

    if (block->typ == SimpleBlockType) {
        auto bottomBlock = std::make_unique<SimpleBlock>(
                blockId + ".0",
                block->bottomLeft,
        Point(block->topRight.p[0], lineNumber),
                dynamic_cast<SimpleBlock *>(block.get())->color
        );
        auto topBlock = std::make_unique<SimpleBlock>(
                blockId + ".1",
        Point(block->bottomLeft.p[0], lineNumber),
                block->topRight,
                dynamic_cast<SimpleBlock *>(block.get())->color
        );

        blocks.erase(blockId);
        blocks[blockId + ".0"] = std::move(bottomBlock);
        blocks[blockId + ".1"] = std::move(topBlock);
        return;
    }

    if (block->typ == ComplexBlockType) {
        std::vector<SimpleBlock> bottomBlocks, topBlocks;
        auto &subBlocks = dynamic_cast<ComplexBlock *>(block.get())->subBlocks;
        for (const auto &subBlock: subBlocks) {
            if (subBlock.bottomLeft.p[1] >= lineNumber) {
                topBlocks.push_back(subBlock);
                continue;
            }
            if (subBlock.topRight.p[1] <= lineNumber) {
                bottomBlocks.push_back(subBlock);
                continue;
            }
            bottomBlocks.emplace_back(
                    "child",
                    subBlock.bottomLeft,
                    Point(subBlock.topRight.p[0], lineNumber),
                    subBlock.color
            );
            topBlocks.emplace_back(
                    "child",
                    Point(subBlock.bottomLeft.p[0], lineNumber),
                    subBlock.topRight,
                    subBlock.color
            );
        }

        auto bottomBlock = std::make_unique<ComplexBlock>(
                blockId + ".0",
                block->bottomLeft,
                Point(block->topRight.p[0], lineNumber),
                bottomBlocks
        );
        auto topBlock = std::make_unique<ComplexBlock>(
                blockId + ".1",
                Point(block->bottomLeft.p[0], lineNumber),
                block->topRight,
                topBlocks
        );

        blocks.erase(blockId);
        blocks[blockId + ".0"] = std::move(bottomBlock);
        blocks[blockId + ".1"] = std::move(topBlock);
        return;
    }
}

void Canvas::SwapCanvas(std::string blockId1, std::string blockId2) {
    auto &block1 = blocks[blockId1];
    auto &block2 = blocks[blockId2];
    moveCost += std::lround(SwapCost * height * width / double(block1->size.getScalarSize()));

    if (block1->size.p[0] == block2->size.p[0] && block1->size.p[1] == block2->size.p[1]) {
        Block *newBlock1 = nullptr, *newBlock2 = nullptr;

        if (block1->typ == SimpleBlockType) {
            newBlock2 = new SimpleBlock(
                    blockId1,
                    block2->bottomLeft,
                    block2->topRight,
                    dynamic_cast<SimpleBlock *>(block1.get())->color
            );
        } else {
            newBlock2 = new ComplexBlock(
                    blockId1,
                    block2->bottomLeft,
                    block2->topRight,
                    dynamic_cast<ComplexBlock *>(block1.get())->offsetChildren(block2->bottomLeft)
            );
        }

        if (block2->typ == SimpleBlockType) {
            newBlock1 = new SimpleBlock(
                    blockId2,
                    block1->bottomLeft,
                    block1->topRight,
                    dynamic_cast<SimpleBlock *>(block2.get())->color
            );
        } else {
            newBlock1 = new ComplexBlock(
                    blockId2,
                    block1->bottomLeft,
                    block1->topRight,
                    dynamic_cast<ComplexBlock *>(block2.get())->offsetChildren(block1->bottomLeft)
            );
        }

        blocks.erase(blockId1);
        blocks.erase(blockId2);

        blocks[newBlock1->id] = std::unique_ptr<Block>(newBlock1);
        blocks[newBlock2->id] = std::unique_ptr<Block>(newBlock2);
    } else {
        printf("Blocks are not the same size, %s has size (%d, %d) while %s has size (%d, %d).", block1->id.c_str(),
               block1->size.p[0], block1->size.p[1], block2->id.c_str(), block2->size.p[0], block2->size.p[1]);
        exit(EXIT_FAILURE);
    }
}

void Canvas::MergeCanvas(std::string blockId1, std::string blockId2) {
    moveCost += MergeCost;
    auto &block1 = blocks[blockId1];
    auto &block2 = blocks[blockId2];
    moveCost += std::lround(SwapCost * height * width / std::max(double(block1->size.getScalarSize()), double(block2->size.getScalarSize())));

    const bool bottomToTop = (block1->bottomLeft.p[1] == block2->topRight.p[1] ||
                              block1->topRight.p[1] == block2->bottomLeft.p[1]) &&
                             block1->bottomLeft.p[0] == block2->bottomLeft.p[0] &&
                             block1->topRight.p[0] == block2->topRight.p[0];
    if (bottomToTop) {
        topLevelIdCounter++;
        Point newBottomLeft, newTopRight;
        if (block1->bottomLeft.p[1] < block2->bottomLeft.p[1]) {
            newBottomLeft = block1->bottomLeft;
            newTopRight = block2->topRight;
        } else {
            newBottomLeft = block2->bottomLeft;
            newTopRight = block1->topRight;
        }
        // TODO: avoid copy
        std::vector<SimpleBlock> children;
        for (auto b: block1->getChildren()) {
            children.emplace_back(*dynamic_cast<SimpleBlock *>(b));
        }
        for (auto b: block2->getChildren()) {
            children.emplace_back(*dynamic_cast<SimpleBlock *>(b));
        }
        auto newBlock = std::make_unique<ComplexBlock>(
                std::to_string(topLevelIdCounter),
                newBottomLeft,
                newTopRight,
                std::move(children)
        );

        blocks[newBlock->id] = std::move(newBlock);
        blocks.erase(blockId1);
        blocks.erase(blockId2);

        return;
    }

    const bool leftToRight = (block1->bottomLeft.p[0] == block2->topRight.p[0] ||
                              block1->topRight.p[0] == block2->bottomLeft.p[0]) &&
                             block1->bottomLeft.p[1] == block2->bottomLeft.p[1] &&
                             block1->topRight.p[1] == block2->topRight.p[1];
    if (leftToRight) {
        topLevelIdCounter++;
        Point newBottomLeft, newTopRight;
        if (block1->bottomLeft.p[0] < block2->bottomLeft.p[0]) {
            newBottomLeft = block1->bottomLeft;
            newTopRight = block2->topRight;
        } else {
            newBottomLeft = block2->bottomLeft;
            newTopRight = block1->topRight;
        }
        // TODO: avoid copy
        std::vector<SimpleBlock> children;
        for (auto b: block1->getChildren()) {
            children.emplace_back(*dynamic_cast<SimpleBlock *>(b));
        }
        for (auto b: block2->getChildren()) {
            children.emplace_back(*dynamic_cast<SimpleBlock *>(b));
        }
        auto newBlock = std::make_unique<ComplexBlock>(
                std::to_string(topLevelIdCounter),
                newBottomLeft,
                newTopRight,
                std::move(children)
        );

        blocks[newBlock->id] = std::move(newBlock);
        blocks.erase(blockId1);
        blocks.erase(blockId2);

        return;
    }

    if (!(bottomToTop || leftToRight)) {
        printf("%s and %s are not mergeable.", blockId1.c_str(), blockId2.c_str());
        exit(EXIT_FAILURE);
    }
}

std::string read_block(std::stringstream &ss) {
    std::string block;
    ss >> block;

    return block.substr(1, block.size() - 2);
}

Point read_point(std::string p) {
    int x, y;
    std::sscanf(p.c_str(), "[%d, %d]", &x, &y);

    return Point(x, y);
}

int read_line_number(std::stringstream &ss) {
    std::string block;
    ss >> block;

    return std::stoi(block.substr(1, block.size() - 2));
}

Color read_color(std::stringstream &ss) {
    std::string all;
    ss >> all;

    while( all.back() != ']' ) {
        std::string next;
        ss >> next;
        all += next;
    }

    int R, G, B, A;
    std::sscanf(all.c_str(), "[%d, %d, %d, %d]", &R, &G, &B, &A);

    return Color ({Color::value_type(R),Color::value_type(G),Color::value_type(B),Color::value_type(A)});
}

void interpretIsl(std::istream &in, Canvas &c) {
    std::string line;

    while(std::getline(in, line) ) {
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
            } else if (orient_or_point == "[Y]" || orient_or_point == "[y]") {
                int line_number = read_line_number(ss);
                c.HorizontalCutCanvas(block, line_number);
            } else {
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
}

uint8_t* hasiImageRead(FILE *fp, int &width, int &height) {
    char buf[1000];
    char name[1000];

    if (!fgets(buf, sizeof(buf), fp)) throw 1;
    for (;;) {
        if (!fgets(buf, sizeof(buf), fp)) throw 1;
        if (std::string(buf) == "ENDHDR\n") break;
        if (std::string(buf) == "TUPLTYPE RGB_ALPHA\n") continue;
        int val;
        sscanf(buf, "%s %d", name, &val);
        auto n = std::string(name);
        if (n == "WIDTH") {
            width = val;
        } else if (n == "HEIGHT") {
            height = val;
        } else if (n == "DEPTH") {
            if (val != 4) throw 1;
        } else if (n == "MAXVAL") {
            if (val != 255) throw 1;
        }
    }

    auto image = new uint8_t[width*height*4];
    if ((int)fread(image, 1, width*height*4, fp) != width*height*4) throw 1;

    return image;
}

Color hasiImageAt(const uint8_t *image, int width, int height, Point at) {
    auto R = image[((height - 1 - at.p[1]) * width + at.p[0]) * 4 + 0];
    auto G = image[((height - 1 - at.p[1]) * width + at.p[0]) * 4 + 1];
    auto B = image[((height - 1 - at.p[1]) * width + at.p[0]) * 4 + 2];
    auto A = image[((height - 1 - at.p[1]) * width + at.p[0]) * 4 + 3];

    return Color({R, G, B, A});
}

