#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>

#include "window.hpp"
#include "view.hpp"
#include "cursor.hpp"

namespace State {
    enum State { Command, Insert };
}

/**
 * Handles input and keeps track of mode
 */
class Controller {
    public:
        /// Construct a Controller
        Controller(View *view);

        /// Let this take control of IO and the screen
        void run();

        /// Alias to run
        void operator()();

    protected:
        std::string getCommand();
        std::string getModeline();

        View *view;
        Window *window;
        Cursor *cursor;
        State::State state;
};

#endif // CONTROLLER_HPP
// vim:ts=4 et sw=4 sts=4
