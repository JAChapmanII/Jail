#ifndef VIEW_HPP
#define VIEW_HPP

#include "buffer.hpp"
#include "window.hpp"

/**
 * An ncurses view of a Buffer
 */
class View {
    public:
        /// Construct a View with a backing Buffer object
        View(Window *win, Buffer *buf);

        /// Display an ncurses view of the backing Buffer
        void view() const;

        /// Alias to view function
        void operator()() const;

    protected:
        Window *window;
        Buffer *buffer;
};

#endif // VIEW_HPP
// vim:ts=4 et sw=4 sts=4
