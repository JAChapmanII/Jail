#include "view.hpp"
using std::string;
using std::vector;

#include <algorithm>
using std::min;

View::View(Window *win, Buffer *buf) : // TODO: again, styling on init lists
        window(win),
        buffer(buf),
        startX(0), endX(win->getWidth() - 1),
        startY(0), endY(win->getHeight() - 1),
        cursor(NULL) {
    this->cursor = new Cursor(buf);
}

void View::view() {
    this->window->start();
    this->endX = this->window->getWidth() - 1;
    this->endY = this->window->getHeight() - 2;

    this->repaint();
}

void View::operator()() {
    this->view();
}

void View::repaint() {
    vector<string> data = this->buffer->getData();
    // incase the Window is longer than the buffer
    int end = min((int)data.size(), (int)this->endY + 1);
    this->window->setRow(0);
    for(int i = startY; i < end; ++i) {
        int stringWidth = (int)data[i].length() - this->startX;
        this->window->write(data[i].substr(this->startX, stringWidth));
    }
}

bool View::checkSanity() {
    // TODO left-right movement inside Buffer
    this->startX = 0;
    this->endX = this->window->getWidth() - 1;

    if(this->cursor->getRow() < this->startY) {
        this->startY = this->cursor->getRow();
        this->endY = this->startY + (this->window->getHeight() - 2);
        return false;
    }
    if(this->cursor->getRow() > this->endY) {
        this->endY = this->cursor->getRow();
        this->startY = this->endY - (this->window->getHeight() - 2);
        return false;
    }

    return true;
}

Cursor *View::getCursor() {
    return this->cursor;
}

Window *View::getWindow() {
    return this->window;
}

long View::getStartX() const {
    return this->startX;
}
long View::getStartY() const {
    return this->startY;
}

// vim:ts=4 et sw=4 sts=4
