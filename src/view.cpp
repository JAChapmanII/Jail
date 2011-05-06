#include "view.hpp"
using std::string;
using std::vector;

#include <algorithm>
using std::min;

View::View(Window *win, Buffer *buf) : // TODO: again, styling on init lists
        window(win),
        buffer(buf),
        row(0), col(0) {
}

void View::view() {
    this->window->start();
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
                if(!mCursor->down()) {
                    this->row++;
                    this->repaint();
                }
                break;

            case 'k':
            case Key::Up:
                if(!mCursor->up()) {
                    this->row--;
                    this->repaint();
                }
                break;

            case 'h':
            case Key::Left:
                if(!mCursor->left()) {
                    this->col--;
                    this->repaint();
                }
                break;

            case 'l':
            case Key::Right:
                if(!mCursor->right()) {
                    this->col++;
                    this->repaint();
                }
                break;

            case Key::CtrlB:
                this->row -= this->window->getHeight();
                this->repaint();
                break;

            case Key::CtrlF:
                this->row += this->window->getHeight();
                this->repaint();
                break;

            case Key::Escape:
                done = true;
                break;

            default:
                break;
        }
        this->window->write(&bottomLeft, line);
        this->window->write(&bottomLeft, i);
        this->window->update();
    }
}

void View::operator()() {
    this->view();
}

void View::repaint() {
    // TODO left-right movement inside Buffer
    this->col = 0;

    vector<string> data = this->buffer->getData();

    if(this->row > (int)data.size() - (this->window->getHeight() - 1))
        this->row = (int)data.size() - (this->window->getHeight() - 1);
    if(this->row < 0)
        this->row = 0;

    Cursor mCursor(this->window);
    // incase the Window is longer than the buffer
    int end = min((int)data.size() - (int)this->row,
            this->window->getHeight() - 1);
    for(int i = 0; i < end; ++i) {
        // if this line does not go completely across, we must erase what
        // might have been there before, so we add a section of spaces
        if((int)data[i + this->row].length() < this->window->getWidth())
            this->window->write(&mCursor, data[i + this->row].substr(
                    this->col, this->window->getWidth()) +
                    string(this->window->getWidth() -
                        data[i + this->row].length(), ' '));
        else
            this->window->write(&mCursor, data[i + this->row].substr(
                    this->col, this->window->getWidth()));

        mCursor.down();
    }
}

// vim:ts=4 et sw=4 sts=4
