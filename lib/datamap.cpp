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
    return this->load(file, [](DataMapState *dms) -> void {
            switch(dms->type) {
                case DMSType::Invalid:
                    cerr << "DataMap::load: unidentifiable line: "
                        << dms->line << endl;
                    break;
                case DMSType::Scope:
                case DMSType::Entry:
                default:
                    break;
            }
        });
}
long DataMap::load(string file, const function<void(DataMapState *)> &predicate) {
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
    DataMapState dms = {
        this, DMSType::Invalid, this->m_defaultScope, "", "", "", 0 };
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

        dms.line = line;
        dms.result = 0;

        char equals = '=';

        // if it is a comment, ignore it
        if(line[0] == '#') {
            continue;
        // if it is a scope line
        } else if(line[0] == '[' && line[line.length() - 1] == ']') {
            dms.type = DMSType::Scope;
            dms.value = trim(line.substr(1, line.length() - 2));
        // if it is a variable definition
        } else if(util::contains(line, equals)) {
            dms.type = DMSType::Entry;
            dms.key = trim(line.substr(0, line.find('=')));
            dms.value = trim(line.substr(line.find('=') + 1));
        } else {
            dms.type = DMSType::Invalid;
        }

        predicate(&dms);
        switch(dms.type) {
            case DMSType::Scope:
                dms.scope = dms.value;
                break;
            case DMSType::Entry:
                this->m_map[dms.scope][dms.key] = dms.value;
                addedEntries++;
                break;
            case DMSType::Invalid:
                break;
        }
        addedEntries += dms.result;
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
