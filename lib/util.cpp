#include "util.hpp"
using std::string;

bool util::startsWith(string str, string start) {
    return str.compare(0, start.size(), start) == 0;
}

string util::trim(const string &str, string remove) {
    if(str.find_first_not_of(remove) == string::npos)
        return "";
    return str.substr(str.find_first_not_of(remove),
            str.find_last_not_of(remove) + 1);
}

// vim:ts=4 et sw=4 sts=4
