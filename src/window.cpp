#include "window.hpp"
using std::string;

// TODO why isn't this defined in ncurses.h?
const int KEY_ESCAPE = 27;

Window::Window() : // TODO style, CodeBlock!
        cursesMode(false),
        row(0) {
    ESCDELAY = 0;
}

Window::~Window() {
    this->stop();
}

void Window::start() {
    if(cursesMode)
        return;
    cursesMode = true;

    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, false);
    keypad(stdscr, true);
    meta(stdscr, true);

    this->update(0, 0);
}

void Window::stop() {
    if(cursesMode) {
        endwin();
        cursesMode = false;
    }
}

int Window::update(int x, int y) {
    move(y, x);
    return refresh();
}

int Window::getKey() {
    return getch();
}

bool Window::write(string s) {
    mvprintw(this->row, 0, "%s", s.c_str());
    if((int)s.length() < COLS)
        mvprintw(this->row, s.length(), "%s",
                string(COLS - (int)s.length(), ' ').c_str());
    this->row++;
}

bool Window::write(int wRow, string s) {
    mvprintw(wRow, 0, "%s", s.c_str());
    if((int)s.length() < COLS)
        mvprintw(this->row, s.length(), "%s",
                string(COLS - (int)s.length(), ' ').c_str());
}

int Window::getHeight() const {
    return LINES;
}

int Window::getWidth() const {
    return COLS;
}

void Window::setRow(int nRow) {
    this->row = nRow;
}

// vim:ts=4 et sw=4 sts=4
