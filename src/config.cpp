#include "config.hpp"

bool beVerbose = false;
bool beQuiet = false;
bool doDump = false;
bool hexMode = false;

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

// vim:ts=4 et sw=4 sts=4
