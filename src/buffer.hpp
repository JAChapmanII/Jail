#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <vector>

/**
 * Buffer is an in-memory representation of a file.
 */
class Buffer {
    public:
        /// Construct an empty Buffer
        Buffer();
        /// Contruct a Buffer with already existing content
        Buffer(std::string contents);

        /// Destroy a Buffer safely
        ~Buffer();

        /// Return the length of this buffer's contents
        int getLength() const;
        /// Return a string representation of this buffer's contents
        std::string getString();
        /// Return a vector of strings of this buffer's contents
        std::vector<std::string> getData();

        /// Clear this buffer's data
        void clear();

    protected:
        // TODO: implement these, or not?
        Buffer(const Buffer &rhs);
        Buffer &operator=(const Buffer &rhs);

        int length;
        // TODO data type here? Change-able later
        std::vector<std::string> data;
};

#endif // BUFFER_HPP
// vim:ts=4 et sw=4 sts=4
