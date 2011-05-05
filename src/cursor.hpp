#ifndef CURSOR_HPP
#define CURSOR_HPP

// TODO better trickery here?
class Cursor;
#include "window.hpp"

/**
 * Represents a position inside of a Window.
 *
 * The internals will handle such things as wrapping if you press right at the
 * edge of the window, and ensure the cursor stays in the bounds of the Window
 * at all times.
 */
class Cursor {
    public:
        /// Construct a Cursor at 0,0
        Cursor(Window *iParent);
        /// Construct a Cursor with a starting position
        Cursor(Window *iParent, int iRow, int iCol);

        /// Move the cursor to the specified position
        bool move(int nRow, int nCol);

        /// Move the cursor to the left
        bool left();
        /// Move the cursor to the right
        bool right();
        /// Move the cursor to the up
        bool up();
        /// Move the cursor to the down
        bool down();

        /// Get the row number this cursor is in
        int getRow() const;
        /// Get the col number this cursor is in
        int getCol() const;

    protected:
        bool checkSanity();

        Window *parent;
        int row;
        int col;
};

#endif // CURSOR_HPP
// vim:ts=4 et sw=4 sts=4
