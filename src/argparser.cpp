#include "argparser.hpp"
using std::string;
using std::vector;
using std::pair;

ArgParser::ArgParser() : // TODO: formatting guidlines here?
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls(),
        extraArguments(),
        invocationName() { // TODO ?
}

ArgParser::ArgParser(SwitchName commandName, StringCallbackFunction function) :
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls(),
        extraArguments(),
        invocationName() { // TODO: formatting guidlines here?
    this->add(commandName, function);
}

ArgParser::ArgParser(SwitchName commandName, VoidCallbackFunction function) :
        voidCallbackMap(),
        stringCallbackMap(),
        voidCalls(),
        stringCalls(),
        extraArguments(),
        invocationName() { // TODO: formatting guidlines here?
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

ArgParser::VoidCallbackFunction *ArgParser::findVoidCBF(string switchName) {
    for(VoidCbMap::iterator i = this->voidCallbackMap.begin();
            i != this->voidCallbackMap.end(); ++i)
        if((i->first.first == switchName) ||
            (i->first.second == switchName))
            return &(i->second);
    return NULL;
}

ArgParser::StringCallbackFunction *ArgParser::findStringCBF(string switchName) {
    for(StringCbMap::iterator i = this->stringCallbackMap.begin();
            i != this->stringCallbackMap.end(); ++i)
        if((i->first.first == switchName) ||
            (i->first.second == switchName))
            return &(i->second);
    return NULL;
}

int ArgParser::parseArguments(int argc, char **argv) {
    this->invocationName = (string)argv[0];
    for(int i = 1; i < argc; ++i) {
        string cArg = (string)argv[i];
        VoidCallbackFunction *vCBF = this->findVoidCBF(cArg);
        if(vCBF != NULL)
            this->voidCalls.push_back(*vCBF);
        else {
            StringCallbackFunction *sCBF = this->findStringCBF(cArg);
            if(sCBF != NULL) {
                string arg = "";
                if(i < argc - 1)
                    // set pCommand to be the next argument, and then skip it
                    arg = (string)argv[i++ + 1];

                this->stringCalls.insert(SCMapEntry(*sCBF, arg));
            } else {
                this->extraArguments.push_back(cArg);
            }
        }
    }
}

void ArgParser::runCommands() const {
    this->runVoidCommands();
    this->runStringCommands();
}

void ArgParser::runVoidCommands() const {
    for(vector<VoidCallbackFunction>::const_iterator i = this->voidCalls.begin();
            i != this->voidCalls.end(); ++i)
        // call the stored function
        (*i)();
}

void ArgParser::runStringCommands() const {
    for(StringCallMap::const_iterator i = this->stringCalls.begin();
            i != this->stringCalls.end(); ++i) // TODO spacing here too?
        // call the stored function with its argument
        (*(i->first))(i->second);
}

vector<string> ArgParser::getExtraArguments() const {
    return this->extraArguments;
}

// vim:ts=4 et sw=4 sts=4
