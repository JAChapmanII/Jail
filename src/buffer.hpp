#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <vector>

#include "fileio.hpp"

/**
 * Buffer is an in-memory representation of a file.
 */
class Buffer {
    public:
        /// Contruct a Buffer with a FileIO backing store
        Buffer(FileIO *iFile, bool iReadOnly = false);

        /// Destroy a Buffer safely
        ~Buffer();

        /// Return the length of this buffer's contents
        int getLength() const;
        /// Return a string representation of this buffer's contents
        std::string getString();
        /// Return a vector of strings of this buffer's contents
        std::vector<std::string> getData();

        /// Returns the number of rows in this Buffer
        int getSize() const;

        /// Returns the length of a row, if it exists
        int getRowLength(int row) const;

        /// Clear this buffer's data
        void clear();

        /// Insert a character into specified row and column
        int insert(long iRow, long iCol, char c);
        /// Erase a character out of the Buffer
        int erase(long eRow, long eCol);
        /// Starting at eRow, eCol condense all spaces into one
        int condenseSpace(long eRow, long eCol);
        /// Combines a line with the line after it
        int combineLines(long eRow);
        /// Delete an entire line
        int deleteLine(long eRow);

        /// Save this buffer to its backing store
        int save();

        /// Checks to see if this buffer is read only
        bool isReadOnly() const;

    protected:
        // TODO: implement these, or not?
        Buffer(const Buffer &rhs);
        Buffer &operator=(const Buffer &rhs);

        bool readOnly;
        FileIO *file;
        int length;
        // TODO data type here? Change-able later
        std::vector<std::string> data;
};

#endif // BUFFER_HPP
// vim:ts=4 et sw=4 sts=4
