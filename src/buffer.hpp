#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>

class Buffer {
    public:
        Buffer(std::string contents);
        Buffer(char *contents);
        ~Buffer();

        int getLength() const;
        std::string getData();

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
