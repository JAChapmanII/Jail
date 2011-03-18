#include "argparser.hpp"
using std::string;
using std::vector;
using std::pair;

// TODO: can't using these, still need prefix?
//using ArgParser::SwitchName;
//using ArgParser::VoidCallbackFunction;
//using ArgParser::StringCallbackFunction;

ArgParser::ArgParser() : // TODO: formatting guidlines here?
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls() { // TODO ?
}

ArgParser::ArgParser(SwitchName commandName, StringCallbackFunction function) :
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls() { // TODO: formatting guidlines here?
    this->add(commandName, function);
}

ArgParser::ArgParser(SwitchName commandName, VoidCallbackFunction function) :
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls() { // TODO: formatting guidlines here?
    this->add(commandName, function);
}

void ArgParser::add(SwitchName commandName, StringCallbackFunction function) {
    this->stringCallbackMap.insert(SCbMapEntry(commandName, function));
}

void ArgParser::add(SwitchName commandName, VoidCallbackFunction function) {
    this->voidCallbackMap.insert(VCbMapEntry(commandName, function));
}

void ArgParser::remove(SwitchName commandName) {
    // left as excersize for reader
}

void ArgParser::remove(SwitchName commandName, bool takesArgument) {
    // left as excersize for reader
}

void ArgParser::queue(StringCallbackFunction function, string argument) {
    if(function == NULL)
        return;
    this->stringCalls.insert(SCMapEntry(function, argument));
}

void ArgParser::queue(VoidCallbackFunction function) {
    if(function == NULL)
        return;
    this->voidCalls.push_back(function);
}


int ArgParser::parseArguments(int argc, char **argv) {
    // TODO implement
    /*
    for(int i = 0; i < argc; ++i) {
        if((string)argv[i] == (string)"--parse-command") {
            isParseCommand = true;
            if(i < argc - 1)
                // set pCommand to be the next argument, and then skip it
                pCommand = argv[i++ + 1];
        }
    }
    */
}

/* ------------------------------------------------------- */

void ArgParser::runCommands() {
    this->runVoidCommands();
    this->runStringCommands();
}

void ArgParser::runVoidCommands() {
    for(vector<VoidCallbackFunction>::iterator i = this->voidCalls.begin();
            i != this->voidCalls.end(); ++i)
        // call the stored function
        (*i)();
}

void ArgParser::runStringCommands() {
    for(StringCallMap::iterator i = this->stringCalls.begin();
            i != this->stringCalls.end(); ++i) // TODO spacing here too?
        // call the stored function with its argument
        (*(i->first))(i->second);
}

// vim:ts=4 et sw=4 sts=4
