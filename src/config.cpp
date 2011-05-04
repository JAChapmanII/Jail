#include "config.hpp"
using std::string;

#include <sstream>
using std::istringstream;

namespace Config {
    bool beVerbose = false;
    bool beQuiet = false;
    bool doDump = false;
    bool hexMode = false;
    int outWidth = 80;
#include "version.hpp"

    int setVerbose() {
        beVerbose = true;
        beQuiet = false;
    }

    int setQuiet() {
        beQuiet = true;
        beVerbose = false;
    }

    int setDump() {
        doDump = true;
    }

    int setHexMode() {
        hexMode = true;
    }

    int setHexDump() {
        hexMode = true;
        doDump = true;
    }

    bool isVerbose() {
        return beVerbose;
    }
    bool isVuiet() {
        return beQuiet;
    }
    bool isDump() {
        return doDump;
    }
    bool isHexMode() {
        return hexMode;
    }

    int setWidth(string w) {
        istringstream ss(w);
        int tmp = 0;
        if(ss >> tmp)
            return outWidth = (tmp > 3) ? tmp : 3;
        else
            return outWidth;
    }
    int getWidth() {
        return outWidth;
    }

    string getVersion() {
        return version;
    }
}

// vim:ts=4 et sw=4 sts=4
