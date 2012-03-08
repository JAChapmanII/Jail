#include "config.hpp"
using std::string;
using std::vector;

#include <iostream>
using std::cerr;
using std::endl;

#include "util.hpp"
using util::startsWith;

DataMap config::map;
static bool config_inited = false;
static vector<string> config_args;

void config::init() {
    if(config_inited)
        return;
#include "defines.cpp"
    config_inited = true;
}

vector<string> config::parse(vector<string> args) {
    config::init();
    if(args.empty())
        return args;
    vector<string> unparsed;

    string switchStart = "--";
    string enableStart = switchStart + "enable-",
            disableStart = switchStart + "disable-";

    while(!args.empty()) {
        string arg = args[0];
        args.erase(args.begin());

        // if is not an argument
        if(!startsWith(arg, switchStart)) {
            unparsed.push_back(arg);
            continue;
        }
        config_args.push_back(arg);

        // enable a flag
        if(startsWith(arg, enableStart)) {
            string flag = arg.substr(enableStart.length());
            config::map["core"][flag] = "true";
        // disable a flag
        } else if(startsWith(arg, disableStart)) {
            string flag = arg.substr(disableStart.length());
            config::map["core"][flag] = "false";
        // otherwise it is a variable setting
        } else {
            string scope_key = arg.substr(switchStart.length()),
                    scope = DataMap::extractScope(scope_key),
                    key = DataMap::extractVariable(scope_key);
            if(args.empty()) {
                cerr << "config::parse: " << key << ": no value passed" << endl;
                continue;
            }
            string val = args[0];
            args.erase(args.begin());

            config::map[scope][key] = val;
            config_args.push_back(val);
        }
    }
    return unparsed;
}
long config::load(string file) {
    config::init();
    // if no file was passed
    if(file.empty()) {
        // try to get it out of the config
        file = get("core", "config");
        // if we failed, return 0
        if(file == null()) {
            cerr << "config::load: unable to find default config" << endl;
            return -10;
        }
    }

    long addedEntries = config::map.load(file);
    config::parse(config_args);

    return addedEntries;
}

bool config::hasScope(string scope) {
    config::init();
    return config::map.hasScope(scope);
}
bool config::has(string scoped_variable) {
    config::init();
    return config::map.has(scoped_variable);
}
bool config::has(string scope, string variable) {
    config::init();
    return config::map.has(scope, variable);
}

string config::get(string scoped_variable) {
    config::init();
    return config::map.get(scoped_variable);
}
string config::get(string scope, string variable) {
    config::init();
    return config::map.get(scope, variable);
}

bool config::isPositive(std::string scoped_variable) {
    config::init();
    return config::map.isPositive(scoped_variable);
}
bool config::isPositive(std::string scope, std::string variable) {
    config::init();
    return config::map.isPositive(scope, variable);
}

double config::asDouble(std::string scoped_variable) {
    config::init();
    return config::map.asDouble(scoped_variable);
}
double config::asDouble(std::string scope, std::string variable) {
    config::init();
    return config::map.asDouble(scope, variable);
}

int config::asInteger(std::string scoped_variable) {
    config::init();
    return config::map.asInteger(scoped_variable);
}
int config::asInteger(std::string scope, std::string variable) {
    config::init();
    return config::map.asInteger(scope, variable);
}

string config::null() {
    config::init();
    return config::map.null();
}

void config::invert(string scoped_variable) {
    config::init();
    config::map.invert(scoped_variable);
}
void config::invert(string scope, string variable) {
    config::init();
    config::map.invert(scope, variable);
}

std::map<string, DataMap::VMap>::iterator config::begin() {
    return config::map.begin();
}
std::map<string, DataMap::VMap>::iterator config::end() {
    return config::map.end();
}

// vim:ts=4 et sw=4 sts=4
