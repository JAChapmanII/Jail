#include "util.hpp"
using std::string;
using std::vector;

bool util::startsWith(string str, string start) {
    if(start.size() > str.size())
        return false;
    return str.compare(0, start.size(), start) == 0;
}
bool util::endsWith(std::string str, std::string end) {
    if(end.size() > str.size())
        return false;
    return str.compare(str.size() - end.size(), end.size(), end) == 0;
}

vector<string> util::split(string str, string on) {
    vector<string> fields;
    size_t fsep = 0;
    while((fsep = str.find_first_of(on)) != string::npos) {
        fields.push_back(str.substr(0, fsep));
        str = str.substr(fsep + 1);
    }
    if(!str.empty())
        fields.push_back(str);
    return fields;
}

string util::trim(string str, string remove) {
    if(str.find_first_not_of(remove) == string::npos)
        return "";
    return str.substr(str.find_first_not_of(remove),
            str.find_last_not_of(remove) + 1);
}

// vim:ts=4 et sw=4 sts=4
