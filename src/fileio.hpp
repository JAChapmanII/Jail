#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <fstream>
#include <string>

/**
 * FileIO is used to read and write to and from files on disk.
 */
class FileIO {
    public:
        /// Construct a FileIO with a backing file
        FileIO(std::string iFileName);
        /// Safely destroy a FileIO
        ~FileIO();

        /// Open backing file for manipulation
        void open();
        /// Close backing file, locking manipulation operations
        void close();

        /// Read in the backing file
        int read();
        /// Get the length of the currently read data buffer
        int getLength() const;
        /// Get a pointer to the internal representation of the read data
        char *getData();

        /// Clear the data buffer
        void clear();

        /// Get the backing filename
        std::string getFileName() const;
        /// Set this FileIO to use a different backing file
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
