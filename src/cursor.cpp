#include "cursor.hpp"

Cursor::Cursor(Buffer *iBuffer) :
        buffer(iBuffer),
        row(0),
        col(0) {
}

Cursor::Cursor(Buffer *iBuffer, long iRow, long iCol) :
        buffer(iBuffer),
        row(iRow),
        col(iCol) {
}

bool Cursor::move(long nRow, long nCol) {
    // TODO sanity checks
    this->row = nRow;
    this->col = nCol;
    return this->checkSanity();
}

bool Cursor::setColumn(long nCol) {
    this->col = nCol;
    return this->checkSanity();
}

bool Cursor::setRow(long nRow) {
    this->row = nRow;
    return this->checkSanity();
}

bool Cursor::toBeginningOfLine() {
    return this->setColumn(0);
}

bool Cursor::toEndOfLine() {
    return this->setColumn(this->buffer->getRowLength(this->row));
}

bool Cursor::toBeginningOfBuffer() {
    return this->setRow(0);
}

bool Cursor::toEndOfBuffer() {
    return this->setRow(this->buffer->getSize());
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
    if(this->col < 0) {
        this->col = 0;
        return false;
    }

    if(this->buffer == NULL)
        return true;

    if(this->row > this->buffer->getSize() - 1) {
        this->row = this->buffer->getSize() - 1;
        return false;
    }

    if(this->col > this->buffer->getRowLength(this->row)) {
        this->col = this->buffer->getRowLength(this->row);
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
long Cursor::getRow() const {
    return this->row;
}

long Cursor::getCol() const {
    return this->col;
}

void Cursor::setBuffer(Buffer *nBuffer) {
    this->buffer = nBuffer;
}

int Cursor::insert(char c) {
    this->buffer->insert(this->row, this->col, c);
    if(c == '\n') {
        this->col = 0;
        return this->down();
    }
    return this->right();
}

int Cursor::backspace() {
    if(this->col == 0) {
        if(this->row == 0)
            return 1;
        long r = this->row, c = this->col;
        this->col = this->buffer->getRowLength(this->row - 1);
        this->up();
        this->buffer->erase(r, c - 1);
        return 0;
    }
    this->buffer->erase(this->row, this->col - 1);
    return this->left();
}

int Cursor::deleteLine() {
    this->buffer->deleteLine(this->getRow());
    return this->checkSanity();
}

// vim:ts=4 et sw=4 sts=4
