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

        /// Mark the end of this controller's lifespan
        void stop();

        /// Save the underlying buffer to the backing file
        void write();

        /// Alias to run
        void operator()();

        /// Enter named command mode
        std::string getCommand();

    protected:
        std::string getModeline();
        void writeModeline(std::string mline);

        View *view;
        Window *window;
        Cursor *cursor;
        State::State state;
        bool done;
};

#endif // CONTROLLER_HPP
// vim:ts=4 et sw=4 sts=4
