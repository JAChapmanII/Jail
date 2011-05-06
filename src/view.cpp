#include "view.hpp"
using std::string;
using std::vector;

#include <algorithm>
using std::min;

View::View(Window *win, Buffer *buf) : // TODO: again, styling on init lists
        window(win),
        buffer(buf),
        startX(0), endX(win->getWidth() - 1),
        startY(0), endY(win->getHeight() - 1) {
}

void View::view() {
    this->window->start();
    this->endX = this->window->getWidth() - 1;
    this->endY = this->window->getHeight() - 2;

    Cursor *mCursor = this->window->getCursor();
    mCursor->setBuffer(this->buffer);

    this->repaint();

    mCursor->move(0, 0);
    Cursor bottomLeft(this->window, this->window->getHeight() - 1, 0);
    string line(this->window->getWidth(), ' ');
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

            case Key::PageUp:
            case Key::CtrlB:
                mCursor->move(mCursor->getRow() - this->window->getHeight() + 1,
                        mCursor->getCol());
                break;

            case Key::PageDown:
            case Key::CtrlF:
                mCursor->move(mCursor->getRow() + this->window->getHeight() - 1,
                        mCursor->getCol());
                break;

            case Key::Escape:
                done = true;
                break;

            default:
                break;
        }
        if(!this->checkSanity())
            this->repaint();

        this->window->write(&bottomLeft, line);
        this->window->write(&bottomLeft, i);
        this->window->update(mCursor->getCol() - startX,
                mCursor->getRow() - startY);
    }
}

void View::operator()() {
    this->view();
}

void View::repaint() {
    Cursor mCursor(this->window);
    vector<string> data = this->buffer->getData();
    // incase the Window is longer than the buffer
    int end = min((int)data.size(), (int)this->endY + 1);
    for(int i = startY; i < end; ++i) {
        // if this line does not go completely across, we must erase what
        // might have been there before, so we add a section of spaces
        int viewWidth = this->endX - this->startX;
        int stringWidth = (int)data[i].length() - this->startX;
        if(stringWidth < viewWidth)
            this->window->write(&mCursor,
                    data[i].substr(this->startX, stringWidth) +
                    string(viewWidth - stringWidth, ' '));
        else
            this->window->write(&mCursor, data[i].substr(
                    this->startX, this->window->getWidth()));

        mCursor.down();
    }
}

bool View::checkSanity() {
    Cursor *mCursor = this->window->getCursor();
    // TODO left-right movement inside Buffer
    this->startX = 0;
    this->endX = this->window->getWidth() - 1;

    if(mCursor->getRow() < this->startY) {
        this->startY = mCursor->getRow();
        this->endY = this->startY + (this->window->getHeight() - 2);
        return false;
    }
    if(mCursor->getRow() > this->endY) {
        this->endY = mCursor->getRow();
        this->startY = this->endY - (this->window->getHeight() - 2);
        return false;
    }

    return true;
}

// vim:ts=4 et sw=4 sts=4
