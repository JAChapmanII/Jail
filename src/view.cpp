#include "view.hpp"
using std::string;
using std::vector;

#include <algorithm>
using std::min;

#include <sstream>
using std::stringstream;

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

    this->cursor->move(0, 0);
    string line(this->window->getWidth(), ' ');
    bool done = false;
    while(!done) {
        int i = this->window->getKey();
        switch(i) {
            case 'j':
            case Key::Down:
                this->cursor->down();
                break;

            case 'k':
            case Key::Up:
                this->cursor->up();
                break;

            case 'h':
            case Key::Left:
                this->cursor->left();
                break;

            case 'l':
            case Key::Right:
                this->cursor->right();
                break;

            case Key::PageUp:
            case Key::CtrlB:
                this->cursor->move(
                        this->cursor->getRow() - this->window->getHeight() + 1,
                        this->cursor->getCol());
                break;

            case Key::PageDown:
            case Key::CtrlF:
                this->cursor->move(
                        this->cursor->getRow() + this->window->getHeight() - 1,
                        this->cursor->getCol());
                break;

            case Key::Escape:
                done = true;
                break;

            default:
                break;
        }
        if(!this->checkSanity())
            this->repaint();

        stringstream ss; ss << ": k" << i;
        this->window->write(this->window->getHeight() - 1, ss.str());
        this->window->update(this->cursor->getCol() - startX,
                this->cursor->getRow() - startY);
    }
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

// vim:ts=4 et sw=4 sts=4
