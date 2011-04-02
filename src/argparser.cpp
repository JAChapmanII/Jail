#include "argparser.hpp"
using std::string;
using std::vector;
using std::pair;

#include <algorithm>

#include <iostream>
using std::cout;
using std::endl;

ArgParser::ArgParser() : // TODO: formatting guidlines here?
        voidCallbackMap(),
        stringCallbackMap(),
        helpIsEnabled(false),
        helpMap(),
        doPrintHelp(false),
        helpHeader(),
        voidCalls(),
        stringCalls(),
        defaultCall(),
        extraArguments(),
        invocationName() { // TODO ?
}

ArgParser::ArgParser(VoidCallbackFunction function) :
        voidCallbackMap(),
        stringCallbackMap(),
        helpIsEnabled(false),
        helpMap(),
        doPrintHelp(false),
        helpHeader(),
        voidCalls(),
        stringCalls(),
        defaultCall(),
        extraArguments(),
        invocationName() { // TODO: formatting guidlines here?
    this->setDefault(function);
}

void ArgParser::add(SwitchName commandName, StringCallbackFunction function) {
    this->stringCallbackMap.insert(SCbMapEntry(commandName, function));
}

void ArgParser::add(SwitchName commandName, StringCallbackFunction function,
        string help) {
    this->add(commandName, function);
    this->addHelp(commandName, help);
}

void ArgParser::add(SwitchName commandName, VoidCallbackFunction function) {
    this->voidCallbackMap.insert(VCbMapEntry(commandName, function));
}

void ArgParser::add(SwitchName commandName, VoidCallbackFunction function,
        std::string help) {
    this->add(commandName, function);
    this->addHelp(commandName, help);
}

void ArgParser::setDefault(VoidCallbackFunction function) {
    this->defaultCall = function;
}

void ArgParser::addHelp(SwitchName commandName, string help) {
    this->helpMap.insert(HelpMapEntry(commandName, help));
}

void ArgParser::setHelp(bool nval) {
    this->helpIsEnabled = nval;
}

bool ArgParser::getHelp() const {
    return this->helpIsEnabled;
}

void ArgParser::setHelpHeader(string nheader) {
    this->helpHeader = nheader;
}

string ArgParser::getHelpHeader() const {
    return this->helpHeader;
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
        if(this->helpIsEnabled &&
            (cArg == (string)"--help" || cArg == (string)"-h")) {
            this->doPrintHelp = true;
            continue;
        }
        VoidCallbackFunction *vCBF = this->findVoidCBF(cArg);
        if(vCBF != NULL) {
            // only queue a void callback function once
            if(find(this->voidCalls.begin(), this->voidCalls.end(), *vCBF) == this->voidCalls.end())
                this->voidCalls.push_back(*vCBF);
        } else {
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
    if(this->doPrintHelp)
        this->printHelp();
    if(this->voidCalls.size() + this->stringCalls.size() > 0) {
        this->runVoidCommands();
        this->runStringCommands();
    } else
        this->runDefaultCommand();
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

void ArgParser::runDefaultCommand() const {
    if(this->defaultCall != NULL)
        // run default callback function
        (*(this->defaultCall))();
    else if(this->helpIsEnabled)
        this->printHelp();
}

void ArgParser::printHelp() const {
    if(this->helpHeader != "")
        cout << this->helpHeader << endl;
    for(HelpMap::const_iterator i = this->helpMap.begin();
            i != this->helpMap.end(); ++i) {
        cout << "\t" << i->first.first;
        if(i->first.second != (string)"")
            cout << "|" << i->first.second;
        cout << ": " << i->second << endl;
    }
}

vector<string> ArgParser::getExtraArguments() const {
    return this->extraArguments;
}

string ArgParser::getInvocationName() const {
    return this->invocationName;
}

// vim:ts=4 et sw=4 sts=4
