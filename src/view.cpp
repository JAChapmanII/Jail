#include "view.hpp"
using std::string;
using std::vector;

#include <sstream>
using std::stringstream;

#include <ncurses.h>

View::View(Buffer *buf) : // TODO: once again, styling on init lists
        buffer(buf) {
}

void View::view() const {
    // TODO ncurses interface class?
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, false);
    keypad(stdscr, true);

    stringstream oss; oss << "%." << (COLS-1) << "s";
    vector<string> data = this->buffer->getData();
    // TODO this should be done in a loop somewhere XD
    for(unsigned int i = 0; i < data.size() && (int)i < LINES; ++i)
        mvprintw(i, 0, oss.str().c_str(), data[i].c_str());

    // TODO cursor class?
    int r = 0, c = 0;
    move(0, 0);

    bool done = false;
    while(!done) {
        getyx(stdscr, r, c);
        int i = getch();
        switch(i) {
            case 'j': if(r < LINES) r++; break;
            case 'k': if(r >     0) r--; break;
            case 'h': if(c >     0) c--; break;
            case 'l': if(c <  COLS) c++; break;
            default:
                done = true;
        }
        refresh();
        move(r, c);
    }
    endwin();
}

void View::operator()() const {
    this->view();
}

// vim:ts=4 et sw=4 sts=4
