#include "fileio.hpp"
using std::string;
using std::fstream;
using std::ios;

FileIO::FileIO(string iFileName) : // TODO: formatting guidelines
        fileName(iFileName),
        file(),
        length(-1),
        data(NULL) {
}

FileIO::~FileIO() {
    this->clear();
    this->close();
}

void FileIO::open() {
    this->clear();
    this->close();
    this->file.open(this->fileName.c_str(), fstream::in | fstream::out);
}

void FileIO::close() {
    this->file.close();
}

int FileIO::read() {
    if(!this->file.good())
        return -1;

    this->file.seekg(0, ios::end);
    this->length = this->file.tellg();
    this->file.seekg(0, ios::beg);

    // TODO: these things could fail
    this->data = new char[this->length];
    this->file.read(this->data, this->length);

    return this->length;
}

int FileIO::getLength() const {
    return this->length;
}

char *FileIO::getData() {
    return this->data;
}

int FileIO::write(string nContent) {
    // TODO: implement
    this->clear();
}

void FileIO::clear() {
    if(this->length < 0)
        return;

    delete[] this->data;
    this->length = -1;
}

string FileIO::getFileName() const {
    return this->fileName;
}

void FileIO::setFileName(string nFileName) {
    this->clear();
    this->close();
    this->fileName = nFileName;
}

// vim:ts=4 et sw=4 sts=4
