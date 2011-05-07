#include "buffer.hpp"
using std::string;
using std::vector;

Buffer::Buffer() : // TODO: formatting guidelines
        length(-1),
        data() {
}

Buffer::Buffer(string contents) : // TODO: formatting guidelines
        length(contents.length()),
        data() {

    if(contents.find('\n') == string::npos) {
        this->data.push_back(contents);
        return;
    }

    unsigned long l = 0, c = 0;
    while((c = contents.find('\n', l)) != string::npos) {
        this->data.push_back(contents.substr(l, c - l));
        l = c + 1;
    }
}

Buffer::~Buffer() {
    this->clear();
}

int Buffer::getLength() const {
    return this->length;
}

string Buffer::getString() {
    string res;
    for(vector<string>::iterator i = this->data.begin(); i != this->data.end();
            ++i)
        res += (*i) + '\n';
    return res;
}

vector<string> Buffer::getData() {
    return this->data;
}

int Buffer::getSize() const {
    return this->data.size();
}

int Buffer::getRowLength(int row) const {
    if((int)this->data.size() - 1 < row)
        return -1;
    return (this->data)[row].length();
}

void Buffer::clear() {
    if(this->length < 0)
        return;

    this->data.clear();
    this->length = -1;
}

int Buffer::insert(long iRow, long iCol, char c) {
    if(c == '\n') {
        vector<string>::iterator i = this->data.begin(); i += iRow + 1;
        if(iCol > (int)this->data[iRow].length()) {
            this->data.insert(i, string());
            return 0;
        }
        this->data.insert(i, this->data[iRow].substr(iCol));
        this->data[iRow].erase(iCol);
        return 0;
    }
    this->data[iRow].insert(iCol, 1, c);
}

int Buffer::erase(long eRow, long eCol) {
    if((eCol < -1) || (eCol > (int)this->data[eRow].length()))
        return 1;
    if(eCol == -1) {
        if(eRow == 0)
            return 1;
        this->data[eRow - 1].append(this->data[eRow]);
        vector<string>::iterator i = this->data.begin();
        i += eRow;
        this->data.erase(i);
        return 0;
    }
    this->data[eRow].erase(eCol, 1);
    return 0;
}

// vim:ts=4 et sw=4 sts=4
