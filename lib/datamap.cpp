#include "datamap.hpp"
using std::map;
using std::string;
using std::getline;
using std::function;

#include <iostream>
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cctype>

#include "util.hpp"
using util::trim;

DataMap::DataMap(std::string defaultScope) : m_defaultScope(defaultScope),
        m_map() {
}

long DataMap::load(string file) {
    this->load(file, [](DataMap *dm, string s, DataMap::SType t){
            if(t == DataMap::Invalid)
                cerr << "DataMap::load: unidentifiable line: " << s << endl;
            return s;
        });
}
long DataMap::load(string file,
        const function<string(DataMap *, string, DataMap::SType)> &predicate) {
    // if no file was passed
    if(file.empty()) {
        cerr << "DataMap::load: can't pass empty file parameter" << endl;
        return -1;
    }

    // open the passed file
    ifstream in(file);
    if(!in.good()) {
        cerr << "DataMap::load: " << file << ": not good" << endl;
        return -2;
    }

    long addedEntries = 0;
    string scope = this->m_defaultScope;
    while(!in.eof()) {
        // read the next line
        string line;
        getline(in, line);
        if(!in.good())
            break;

        // strip whitespace, continue if line is blank
        line = trim(line);
        if(line.empty())
            continue;

        char equals = '=';

        // if it is a comment, ignore it
        if(line[0] == '#') {
            continue;
        // if it is a scope line
        } else if(line[0] == '[' && line[line.length() - 1] == ']') {
            scope = trim(line.substr(1, line.length() - 2));
            scope = predicate(this, scope, DataMap::Scope);
        // if it is a variable definition
        } else if(util::contains(line, equals)) {
            string key = trim(line.substr(0, line.find('='))),
                    val = trim(line.substr(line.find('=') + 1));
            key = predicate(this, key, DataMap::Key);
            val = predicate(this, val, DataMap::Value);
            this->m_map[scope][key] = val;
            addedEntries++;
        } else {
            predicate(this, line, DataMap::Invalid);
        }
    }
    return addedEntries;
}

string DataMap::extractScope(string scoped_variable) {
    if(scoped_variable.find('.') == string::npos)
        return "core";
    return scoped_variable.substr(0, scoped_variable.rfind('.'));
}
string DataMap::extractVariable(string scoped_variable) {
    if(scoped_variable.find('.') == string::npos)
        return scoped_variable;
    return scoped_variable.substr(scoped_variable.rfind('.') + 1);
}

bool DataMap::hasScope(string scope) {
    return util::contains(this->m_map, scope);
}
bool DataMap::has(std::string scoped_variable) {
    return this->has(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
bool DataMap::has(string scope, string variable) {
    if(!this->hasScope(scope))
        return false;
    return util::contains(this->m_map[scope], variable);
}

string DataMap::get(string scoped_variable) {
    return this->get(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
string DataMap::get(string scope, string variable) {
    if(!this->has(scope, variable))
        return this->null();
    return this->m_map[scope][variable];
}

bool DataMap::isPositive(string scoped_variable) {
    return this->isPositive(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
bool DataMap::isPositive(string scope, string variable) {
    if(!this->has(scope, variable))
        return false;
    string value = this->get(scope, variable);
    if(value.empty())
        return false;
    if((value == (string)"false") || (value == (string)"no") ||
            (value == (string)"nein") || (value == (string)"off") ||
            (value == (string)"0"))
        return false;
    return true;
}

double DataMap::asDouble(string scoped_variable) {
    return this->asDouble(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
double DataMap::asDouble(string scope, string variable) {
    if(!this->has(scope, variable))
        return 0;
    return util::fromString<double>(this->get(scope, variable));
}

int DataMap::asInteger(string scoped_variable) {
    return this->asInteger(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
int DataMap::asInteger(string scope, string variable) {
    if(!this->has(scope, variable))
        return 0;
    return util::fromString<int>(this->get(scope, variable));
}

string DataMap::null() {
    return "(null)";
}

void DataMap::invert(string scoped_variable) {
    this->invert(
            extractScope(scoped_variable), extractVariable(scoped_variable));
}
void DataMap::invert(string scope, string variable) {
    if(this->isPositive(scope, variable))
        this->m_map[scope][variable] = "false";
    else
        this->m_map[scope][variable] = "true";
}

map<string, string> &DataMap::operator[](string scope) {
    return this->m_map[scope];
}
map<string, DataMap::VMap> &DataMap::map() {
    return this->m_map;
}

map<string, DataMap::VMap>::iterator DataMap::begin() {
    return this->m_map.begin();
}
map<string, DataMap::VMap>::iterator DataMap::end() {
    return this->m_map.end();
}

DataMap::VMap::iterator DataMap::begin(string scope) {
    if(!this->hasScope(scope))
        throw ("DataMap::begin(" + scope + "): no such scope");
    return this->m_map[scope].begin();
}
DataMap::VMap::iterator DataMap::end(string scope) {
    if(!this->hasScope(scope))
        throw ("DataMap::end(" + scope + "): no such scope");
    return this->m_map[scope].end();
}

// vim:ts=4 et sw=4 sts=4
