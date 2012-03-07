#include "argparser.hpp"
using std::string;
using std::vector;
using std::pair;

#include <iostream>
using std::cout;
using std::endl;

#include "util.hpp"
using util::contains;

ArgParser::ArgParser() : // TODO: formatting guidlines here?
        voidCallbackMap(),
        stringCallbackMap(),
        helpIsEnabled(false),
        helpMap(),
        doPrintHelp(false),
        helpHeader(),
        helpFooter(),
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
        helpFooter(),
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
        string help) {
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

void ArgParser::setHelpFooter(string nfooter) {
    this->helpFooter = nfooter;
}

string ArgParser::getHelpFooter() const {
    return this->helpFooter;
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
    for(auto it = this->voidCallbackMap.begin();
            it != this->voidCallbackMap.end(); ++it)
        if((it->first.first == switchName) || (it->first.second == switchName))
            return &(it->second);
    //for(auto it : this->voidCallbackMap)
        //if((it.first.first == switchName) || (it.first.second == switchName))
            //return &(it.second);
    return NULL;
}

ArgParser::StringCallbackFunction *ArgParser::findStringCBF(string switchName) {
    for(auto it = this->stringCallbackMap.begin();
            it != this->stringCallbackMap.end(); ++it)
        if((it->first.first == switchName) || (it->first.second == switchName))
            return &(it->second);
    //for(auto sci : this->stringCallbackMap)
        //if((sci.first.first == switchName) || (sci.first.second == switchName))
            //return &(sci.second);
    return NULL;
}

int ArgParser::parseArguments(int argc, char **argv) {
    this->invocationName = (string)argv[0];
    for(int i = 1; i < argc; ++i) {
        string cArg = (string)argv[i];
        // Break out of normal argument parsing
        if(cArg == (string)"--") {
            for(int j = i + 1; j < argc; ++j) {
                cArg = (string)argv[j];
                this->extraArguments.push_back(cArg);
            }
            break;
        }
        if(this->helpIsEnabled &&
            (cArg == (string)"--help" || cArg == (string)"-h")) {
            this->doPrintHelp = true;
            continue;
        }

        VoidCallbackFunction *vCBF = this->findVoidCBF(cArg);
        if(vCBF != NULL) {
            // only queue a void callback function once
            if(!contains(this->voidCalls, *vCBF))
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
    for(auto vc : this->voidCalls)
        // call the stored function
        vc();
}

void ArgParser::runStringCommands() const {
    for(auto sc : this->stringCalls)
        // call the stored function with its argument
        (*sc.first)(sc.second);
}

void ArgParser::runDefaultCommand() const {
    if(this->defaultCall != NULL)
        // run default callback function
        (*(this->defaultCall))();
    else if(this->helpIsEnabled && !this->doPrintHelp)
        this->printHelp();
}

void ArgParser::printHelp() const {
    if(this->helpHeader != "")
        cout << this->helpHeader << endl;
    for(auto hme : this->helpMap) {
        cout << "\t";
        if(!hme.first.first.empty())
            cout << hme.first.first;
        if(!hme.first.second.empty()) {
            if(!hme.first.first.empty())
                cout << "|";
            cout << hme.first.second;
        }
        cout << ": " << hme.second << endl;
    }
    if(!this->helpFooter.empty())
        cout << this->helpFooter << endl;
}

vector<string> ArgParser::getExtraArguments() const {
    return this->extraArguments;
}

string ArgParser::getInvocationName() const {
    return this->invocationName;
}

// vim:ts=4 et sw=4 sts=4
