#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {
    void setVerbose();
    void setQuiet();
    void setDump();
    void setHexMode();
    void setHexDump();
    void setReadOnly();

    bool isVerbose();
    bool isQuiet();
    bool isDump();
    bool isHexMode();
    bool isReadOnly();

    void setWidth(std::string w);
    int getWidth();

    std::string getVersion();
}

#endif // CONFIG_HPP
// vim:ts=4 et sw=4 sts=4
