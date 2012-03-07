#include "buffer.hpp"
using std::string;
using std::vector;

Buffer::Buffer(FileIO *iFile, bool iReadOnly) : // TODO: formatting guidelines
        readOnly(iReadOnly),
        file(iFile),
        length(-1),
        data() {

    this->file->open();
    int fLength = this->file->read();

    if(fLength < 0)
        throw fLength;

    string contents;
    if(fLength > 0)
        contents = (string)this->file->getData();

    this->length = contents.length();

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
    // TODO: invalidate by lots of stuff
    return this->length;
}

string Buffer::getString() {
    string res;
    for(auto line : this->data)
        res += line + '\n';
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

int Buffer::condenseSpace(long eRow, long eCol) {
    if((eCol < -1) || (eCol >= (long)this->data[eRow].length()))
        return 1;
    if((eRow < 0) || (eRow >= (long)this->data.size()))
        return 1;
    // TODO: is space command
    // TODO: I don't think tabs work in jl right now at all
    eCol++;
    while((eCol < (long)this->data[eRow].length()) && (this->data[eRow][eCol] == ' '))
        this->data[eRow].erase(eCol, 1);
    // TODO: we could find out how many spaces there are and then do one erase
    return 0;
}

int Buffer::combineLines(long eRow) {
    if((eRow < 0) || (eRow >= (long)this->data.size() - 1))
        return 1;
    this->data[eRow].append(this->data[eRow + 1]);
    return this->deleteLine(eRow + 1);
}

int Buffer::deleteLine(long eRow) {
    this->data.erase(this->data.begin() + eRow);
    if(this->data.empty())
        this->data.push_back("");
    return 0;
}

int Buffer::save() {
    if(this->readOnly)
        return -2;
    return this->file->write(this->getString());
}

bool Buffer::isReadOnly() const {
    return this->readOnly;
}

// vim:ts=4 et sw=4 sts=4
