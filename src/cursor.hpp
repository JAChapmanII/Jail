#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "buffer.hpp"

/**
 * Represents a position inside of a Buffer.
 *
 * The internals will handle such things as wrapping if you press right at the
 * edge of a line, and ensure the cursor stays in the bounds of the Buffer at
 * all times.
 */
class Cursor {
    public:
        /// Construct a Cursor at 0,0
        Cursor(Buffer *iBuffer);
        /// Construct a Cursor with a starting position
        Cursor(Buffer *iBuffer, long iRow, long iCol);

        /// Move the cursor to the specified position
        bool move(long nRow, long nCol);
        /// Set's this cursor to be in a certain column
        bool setColumn(long nCol);
        /// Set's this cursor to be in a certain row
        bool setRow(long nRow);

        /// Moves cursor to beginning of line
        bool toBeginningOfLine();
        /// Moves cursor to end of line
        bool toEndOfLine();

        /// Moves cursor to beginning of buffer
        bool toBeginningOfBuffer();
        /// Moves cursor to end of buffer
        bool toEndOfBuffer();

        /// Move the cursor to the left
        bool left();
        /// Move the cursor to the right
        bool right();
        /// Move the cursor to the up
        bool up();
        /// Move the cursor to the down
        bool down();

        /// Get the row number this cursor is in
        long getRow() const;
        /// Get the col number this cursor is in
        long getCol() const;

        /// Sets the backing buffer to be a new Buffer object
        void setBuffer(Buffer *nBuffer);

        /// Inserts a character into the buffer at the current position
        int insert(char c);
        /// Delete the character before the cursor
        int backspace();
        /// Delete the character after the cursor
        int erase();
        /// Combines a line with the one after it (inserts a space)
        int combineLines();
        /// Deletes the entire line the cursor is on
        int deleteLine();

    protected:
        bool checkSanity();

        Buffer *buffer;
        long row;
        long col;
};

#endif // CURSOR_HPP
// vim:ts=4 et sw=4 sts=4
