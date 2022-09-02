#include <vector>
#include <string>
#include <array>

using Color = std::array<uint32_t, 4>;
using BlockId = int;
using BlockIds = std::vector<BlockId>;


class Block {
public:
    Block(BlockId block_id, int x, int y, int x_offset, int y_offset, Color c_);

    void LineCut(BlockIds block_ids, char orientation, int offset, int child_id);
    void PointCut(BlockIds block_ids, int x, int y, int child_id);
    void ColorMove(BlockIds block_ids, Color c, int child_id);

    bool IsIn(int x, int y);

    std::vector<Block> children_;
    int x_size_;
    int y_size_;

    // absolute coordinate
    int x_offset_;
    int y_offset_;

    Color c_;
};

class Canvas {
public:
    Canvas(int x, int y);

    void LineCut(BlockIds block_ids, char orientation, int offset);
    void PointCut(BlockIds block_ids, int x, int y);
    void ColorMove(BlockIds block_ids, Color c);
    void SwapMove(BlockIds a, BlockIds b);
    void MergeMove(BlockIds a, BlockIds b);

    std::vector<Block> children_;
    const int x_;
    const int y_;
};

Canvas::Canvas(int x, int y) : x_(x), y_(y), children_({Block(0, x, y, 0, 0, Color{0,0,0,0})}) {}

Block::Block(BlockId block_id, int x, int y, int x_offset, int y_offset, Color c) : x_size_(x), y_size_(y), x_offset_(x_offset), y_offset_(y_offset), c_(c) {
    printf("block_id %d x=%d y=%d x_offset=%d, y_offset=%d\n", block_id, x, y, x_offset, y_offset);
}


void Canvas::LineCut(BlockIds block_ids, char orientation, int offset) {
    children_[block_ids[0]].LineCut(block_ids, orientation, offset, 1);
}

void Canvas::PointCut(BlockIds block_ids, int x, int y) {
    children_[block_ids[0]].PointCut(block_ids, x, y, 1);
}

void Canvas::ColorMove(BlockIds block_ids, Color c) {
    children_[block_ids[0]].ColorMove(block_ids, c, 1);
}

void Canvas::SwapMove(BlockIds a, BlockIds b) {
    std::vector<Block> *a_vector_ref_ = &children_;
    for (size_t i = 0; i < (a.size() - 1); ++i) {
        a_vector_ref_ = &(*a_vector_ref_)[a[i]].children_;
    }

    std::vector<Block> *b_vector_ref_ = &children_;
    for (size_t i = 0; i < (b.size() - 1); ++i) {
        b_vector_ref_ = &(*b_vector_ref_)[b[i]].children_;
    }

    Block *a_ptr = &((*a_vector_ref_)[a.back()]);
    Block *b_ptr = &((*b_vector_ref_)[b.back()]);

    if (a_ptr->x_size_ != b_ptr->x_size_ || a_ptr->y_size_ != b_ptr->y_size_) {
        printf("SwapMove: invalid size (%d,%d) != (%d,%d)\n", a_ptr->x_size_, a_ptr->y_size_, b_ptr->x_size_, b_ptr->y_size_);
        exit(EXIT_FAILURE);
    }

    if (!a_ptr->children_.empty()) {
        printf("SwapMove: children detected (a)\n");
        exit(EXIT_FAILURE);
    }

    if (!b_ptr->children_.empty()) {
        printf("SwapMove: children detected (b)\n");
        exit(EXIT_FAILURE);
    }

    auto backup = *a_ptr;
    *a_ptr = *b_ptr;
    *b_ptr = backup;
}

void Canvas::MergeMove(BlockIds a, BlockIds b) {
    std::vector<Block> *a_vector_ref_ = &children_;
    for (size_t i = 0; i < (a.size() - 1); ++i) {
        a_vector_ref_ = &(*a_vector_ref_)[a[i]].children_;
    }

    std::vector<Block> *b_vector_ref_ = &children_;
    for (size_t i = 0; i < (b.size() - 1); ++i) {
        b_vector_ref_ = &(*b_vector_ref_)[b[i]].children_;
    }

    Block *a_ptr = &((*a_vector_ref_)[a.back()]);
    Block *b_ptr = &((*b_vector_ref_)[b.back()]);

    if ((a_ptr->x_size_ && b_ptr->x_size_) &&
     (a_ptr->x_offset_ + a_ptr->x_size_ == b_ptr->x_offset_) || (b_ptr->x_offset_ + b_ptr->x_size_ == a_ptr->x_offset_)) {
        // x_adjoint

    }
}

void Block::LineCut(BlockIds block_ids, char orientation, int offset, int child_id) {
    if (child_id < block_ids.size()) {
        return children_[block_ids[child_id]].LineCut(block_ids, orientation, offset, child_id + 1);
    }

    if (!children_.empty()) {
        printf("LineCut: already cut");
        exit(EXIT_FAILURE);
    }

    switch (orientation) {
        case 'x':
        case 'X':
            children_.emplace_back(0, offset, y_size_, x_offset_, y_offset_, c_);
            children_.emplace_back(1, x_offset_ + x_size_ - offset, y_size_, x_offset_ + offset, y_offset_, c_);

            break;

        case 'y':
        case 'Y':
            children_.emplace_back(0, x_size_, offset, x_offset_, y_offset_, c_);
            children_.emplace_back(1, x_size_, y_offset_ + y_size_ - offset, x_offset_, y_offset_ + offset, c_);

            break;
    }
}

void Block::PointCut(BlockIds block_ids, int x, int y, int child_id) {
    if (child_id < block_ids.size()) {
        return children_[block_ids[child_id]].PointCut(block_ids, x, y, child_id + 1);
    }

    if (!children_.empty()) {
        printf("PointCut: already cut");
        exit(EXIT_FAILURE);
    }

    children_.emplace_back(0, x, y, x_offset_, y_offset_, c_);
    children_.emplace_back(1, x_size_ - x, y, x_offset_ + x, y_offset_, c_);
    children_.emplace_back(2, x_size_ - x, y_size_ - y, x_offset_ + x, y_offset_ + y, c_);
    children_.emplace_back(3, x, y_size_ - y, x_offset_, y_offset_ + y, c_);
}

void Block::ColorMove(BlockIds block_ids, Color c, int child_id) {
    if (child_id < block_ids.size()) {
        return children_[block_ids[child_id]].ColorMove(block_ids, c, child_id + 1);
    }

    if (!children_.empty()) {
        printf("ColorMove: already cut");
        exit(EXIT_FAILURE);
    }

    c_ = c;
}


int main() {
    Canvas c = Canvas(150, 100);

    c.LineCut({0}, 'x', 50);
    c.LineCut({0, 1}, 'x', 100);

    c.ColorMove({0, 0}, {1, 1, 1, 1});
    c.ColorMove({0, 1, 0}, {2, 2, 2, 2});
    c.ColorMove({0, 1, 1}, {3, 3, 3, 3});

    printf("Color: %d\n", c.children_[0].children_[0].c_[0]);
    printf("Color: %d\n", c.children_[0].children_[1].children_[0].c_[0]);
    printf("Color: %d\n", c.children_[0].children_[1].children_[1].c_[0]);

    c.SwapMove({0, 0}, {0, 1, 1});

    printf("Color: %d\n", c.children_[0].children_[0].c_[0]);
    printf("Color: %d\n", c.children_[0].children_[1].children_[0].c_[0]);
    printf("Color: %d\n", c.children_[0].children_[1].children_[1].c_[0]);

    /*
    c.LineCut({0}, 'x', 40);
    c.LineCut({0}, 'Y', 60);
    */
}
