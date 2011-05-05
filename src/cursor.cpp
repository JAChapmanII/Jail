#include "cursor.hpp"

Cursor::Cursor(Window *iParent) :
        parent(iParent),
        row(0),
        col(0) {
}

Cursor::Cursor(Window *iParent, int iRow, int iCol) :
        parent(iParent),
        row(iRow),
        col(iCol) {
}

bool Cursor::move(int nRow, int nCol) {
    // TODO sanity checks
    this->row = nRow;
    this->col = nCol;
    return this->checkSanity();
}

bool Cursor::left() {
    this->col--;
    return this->checkSanity();
}
bool Cursor::right() {
    this->col++;
    return this->checkSanity();
}

bool Cursor::up() {
    this->row--;
    return this->checkSanity();
}
bool Cursor::down() {
    this->row++;
    return this->checkSanity();
}

bool Cursor::checkSanity() {
    if(this->row < 0) {
        this->row = 0;
        return false;
    }
    if(this->row > this->parent->getHeight() - 1) {
        this->row = this->parent->getHeight() - 1;
        return false;
    }

    if(this->col < 0) {
        this->col = 0;
        return false;
    }
    if(this->col > this->parent->getWidth() - 1) {
        this->col = this->parent->getWidth() - 1;
        return false;
    }

    return true;
}

// TODO these are constant, but I fear they may provide inconsistent state if
// TODO the backing window is resized without having any mutating methods being
// TODO called in between.
int Cursor::getRow() const {
    return this->row;
}

int Cursor::getCol() const {
    return this->col;
}

// vim:ts=4 et sw=4 sts=4
