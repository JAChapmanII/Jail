#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <fstream>
#include <string>

class FileIO {
    public:
        FileIO(std::string iFileName);
        ~FileIO();

        void open();
        void close();

        int read();
        int getLength() const;
        char *getData();

        void clear();

        std::string getFileName() const;
        void setFileName(std::string nFileName);

    protected:
        // TODO: implement these, or not?
        FileIO(const FileIO &rhs);
        FileIO &operator=(const FileIO &rhs);

        std::string fileName;
        std::fstream file;

        int length;
        char *data;
};

#endif // FILEIO_HPP
// vim:ts=4 et sw=4 sts=4
