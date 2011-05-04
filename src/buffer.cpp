#include "buffer.hpp"
using std::string;

Buffer::Buffer() : // TODO: formatting guidelines
        length(-1),
        data(NULL) {
}

Buffer::Buffer(string contents) : // TODO: formatting guidelines
        length(contents.length()),
        data(NULL) {
    this->data = new char[this->length];
    contents.copy(this->data, this->length);
}

Buffer::~Buffer() {
    this->clear();
}

int Buffer::getLength() const {
    return this->length;
}

string Buffer::getData() {
    return (string)this->data;
}

void Buffer::clear() {
    if(this->length < 0)
        return;

    delete[] this->data;
    this->length = -1;
}

// vim:ts=4 et sw=4 sts=4
