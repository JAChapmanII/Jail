#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {
    int setVerbose();
    int setQuiet();
    int setDump();
    int setHexMode();
    int setHexDump();
    int setReadOnly();

    bool isVerbose();
    bool isQuiet();
    bool isDump();
    bool isHexMode();
    bool isReadOnly();

    int setWidth(std::string w);
    int getWidth();

    std::string getVersion();
}

#endif // CONFIG_HPP
// vim:ts=4 et sw=4 sts=4
