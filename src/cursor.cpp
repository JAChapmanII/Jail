#include "cursor.hpp"

Cursor::Cursor(Window *iParent) :
        parent(iParent),
        row(0),
        col(0),
        buffer(NULL) {
}

Cursor::Cursor(Window *iParent, int iRow, int iCol) :
        parent(iParent),
        row(iRow),
        col(iCol),
        buffer(NULL) {
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

    if(this->buffer == NULL)
        return true;

    if(this->row > this->buffer->getSize() - 1) {
        this->row = this->buffer->getSize() - 1;
        // Return true because we should not try to scroll down
        return true;
    }

    if(this->col > this->buffer->getRowLength(this->row) - 1) {
        this->col = this->buffer->getRowLength(this->row) - 1;
        // We should just be at the start of a blank line
        if(this->buffer->getRowLength(this->row) == 0)
            this->col = 0;
        return true;
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

void Cursor::setBuffer(Buffer *nBuffer) {
    this->buffer = nBuffer;
}

// vim:ts=4 et sw=4 sts=4
