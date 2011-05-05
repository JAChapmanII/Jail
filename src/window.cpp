#include "window.hpp"
using std::string;

// TODO why isn't this defined in ncurses.h?
const int KEY_ESCAPE = 27;

Window::Window() : // TODO style, CodeBlock!
        cursesMode(false),
        cursor() {
    ESCDELAY = 0;
    this->cursor = new Cursor(this);
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

    this->update();
}

void Window::stop() {
    if(cursesMode) {
        endwin();
        cursesMode = false;
    }
}

int Window::update() {
    return refresh();
}

int Window::getKey() {
    return getch();
}

bool Window::write(string s) {
    return this->write(this->cursor, s);
}

bool Window::write(Cursor *c, string s) {
    mvprintw(c->getRow(), c->getCol(), "%s", s.c_str());
}

bool Window::write(Cursor *c, int i) {
    mvprintw(c->getRow(), c->getCol(), "%4i", i);
}

int Window::getHeight() const {
    return LINES;
}

int Window::getWidth() const {
    return COLS;
}

Cursor *Window::getCursor() {
    return this->cursor;
}

// vim:ts=4 et sw=4 sts=4
