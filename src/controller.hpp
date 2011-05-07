#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "window.hpp"
#include "view.hpp"
#include "cursor.hpp"

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
        View *view;
        Window *window;
        Cursor *cursor;
};

#endif // CONTROLLER_HPP
// vim:ts=4 et sw=4 sts=4
