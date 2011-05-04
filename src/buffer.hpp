#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>

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
        std::string getData();

        /// Clear this buffer's data
        void clear();

    protected:
        // TODO: implement these, or not?
        Buffer(const Buffer &rhs);
        Buffer &operator=(const Buffer &rhs);

        int length;
        char *data;
};

#endif // BUFFER_HPP
// vim:ts=4 et sw=4 sts=4
