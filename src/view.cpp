#include "view.hpp"
using std::string;
using std::vector;

#include <algorithm>
using std::min;

View::View(Window *win, Buffer *buf) : // TODO: again, styling on init lists
        window(win),
        buffer(buf) {
}

void View::view() const {
    this->window->start();
    Cursor *mCursor = this->window->getCursor();

    vector<string> data = this->buffer->getData();
    // TODO this should be done in a loop somewhere XD
    int end = min((int)data.size(), this->window->getHeight());
    for(int i = 0; i < end; ++i) {
        if((int)data[i].length() > this->window->getWidth() - 1)
            this->window->write(data[i].substr(0, this->window->getWidth() - 1));
        else
            this->window->write(data[i]);
        mCursor->down();
    }


    mCursor->move(0, 0);
    Cursor bottomLeft(this->window, this->window->getHeight() - 1, 0);
    bool done = false;
    while(!done) {
        //ncurses::getyx(stdscr, r, c);
        int i = this->window->getKey();
        switch(i) {
            case 'j':
            case Key::Down:
                mCursor->down();
                break;

            case 'k':
            case Key::Up:
                mCursor->up();
                break;

            case 'h':
            case Key::Left:
                mCursor->left();
                break;

            case 'l':
            case Key::Right:
                mCursor->right();
                break;

            case Key::Escape:
                done = true;
                break;

            default:
                break;
        }
        this->window->write(&bottomLeft, i);
        this->window->update();
    }
}

void View::operator()() const {
    this->view();
}

// vim:ts=4 et sw=4 sts=4
