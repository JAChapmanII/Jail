#include "config.hpp"
using std::string;

#include <sstream>
using std::istringstream;

#include "util.hpp"
using util::fromString;

namespace config {
    bool beVerbose = false;
    bool beQuiet = false;
    bool doDump = false;
    bool hexMode = false;
    // TODO: This should probably be per buffer
    bool readOnly = false;
    int outWidth = 80;
#include "version.hpp"

    DataMap map;
    static bool inited = false;

    void init() {
        if(inited)
            return;
#include "default_config.cpp"
        inited = true;
    }


    void setVerbose() {
        beVerbose = true;
        beQuiet = false;
    }

    void setQuiet() {
        beQuiet = true;
        beVerbose = false;
    }

    void setDump() {
        doDump = true;
    }

    void setHexMode() {
        hexMode = true;
    }

    void setHexDump() {
        hexMode = true;
        doDump = true;
    }

    void setReadOnly() {
        readOnly = true;
    }

    bool isVerbose() {
        return beVerbose;
    }

    bool isQuiet() {
        return beQuiet;
    }

    bool isDump() {
        return doDump;
    }

    bool isHexMode() {
        return hexMode;
    }

    bool isReadOnly() {
        return readOnly;
    }

    void setWidth(string w) {
        outWidth = fromString<int>(w);
        if(outWidth < 3)
            outWidth = 3;
    }
    int getWidth() {
        return outWidth;
    }

    string getVersion() {
        return version;
    }
}

// vim:ts=4 et sw=4 sts=4
