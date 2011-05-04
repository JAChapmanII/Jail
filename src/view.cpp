#include "view.hpp"
using std::string;
using std::vector;

#include <sstream>
using std::stringstream;

#include <ncurses.h>
// TODO why isn't this defined in ncurses.h?
const int KEY_ESCAPE = 27;

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

    ESCDELAY = 0;
    bool done = false;
    while(!done) {
        getyx(stdscr, r, c);
        int i = getch();
        switch(i) {
            case 'j':
            case KEY_DOWN:
                if(r < LINES) r++;
                break;

            case 'k':
            case KEY_UP:
                if(r >     0) r--;
                break;

            case 'h':
            case KEY_LEFT:
                if(c >     0) c--;
                break;

            case 'l':
            case KEY_RIGHT:
                if(c <  COLS) c++;
                break;

            case KEY_ESCAPE:
                done = true;
                break;

            default:
                break;
        }
        mvprintw(LINES - 1, 0, ":%4d", i);
        move(r, c);
        refresh();
    }
    endwin();
}

void View::operator()() const {
    this->view();
}

// vim:ts=4 et sw=4 sts=4
