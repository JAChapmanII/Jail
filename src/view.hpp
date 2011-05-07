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
        void view();

        /// Alias to view function
        void operator()();

        /// Repaints Buffer contents into Window
        void repaint();
        /// make sure the view has the cursor in it
        bool checkSanity();

        /// Returns a pointer to this's cursor
        Cursor *getCursor();
        /// Returns a pointer to this's window
        Window *getWindow();

        /// Get's this's startX value
        long getStartX() const;
        /// Get's this's startY value
        long getStartY() const;

    protected:
        Window *window;
        Buffer *buffer;
        long startX, endX;
        long startY, endY;

        Cursor *cursor;
};

#endif // VIEW_HPP
// vim:ts=4 et sw=4 sts=4
