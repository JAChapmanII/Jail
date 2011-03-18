#include "argparser.hpp"
using std::string;
using std::vector;
using std::pair;

ArgParser::ArgParser() : // TODO: formatting guidlines here?
        longCommands(),
        shortCommands(),
        hasArgs(),
        callbackFunctions(),
        toCall() { // TODO ?
}

ArgParser::ArgParser(string longCommand, string shortCommand, bool hasArg,
        ArgParser::CallbackFunction function) :
        longCommands(),
        shortCommands(),
        hasArgs(),
        callbackFunctions(),
        toCall() { // TODO: formatting guidlines here?
    this->add(longCommand, shortCommand, hasArg, function);
}

void ArgParser::add(string longCommand, string shortCommand,
        bool hasArg, ArgParser::CallbackFunction function) {
    this->longCommands.push_back(longCommand);
    this->shortCommands.push_back(shortCommand);
    this->hasArgs.push_back(hasArg);
    this->callbackFunctions.push_back(function);
}

void ArgParser::remove(string commandName) {
    // left as excersize for reader
}

void ArgParser::queue(ArgParser::CallbackFunction function, string argument) {
    if(function == NULL)
        return;
    this->toCall.insert( // TODO: formatting guidlines here?
        pair<ArgParser::CallbackFunction, string>(function, argument));
}

int ArgParser::parseArguments(int &argc, char ***argv) {
    // copy-pasta loop? XD wait why can't......
}

void ArgParser::runCommands() {
    for(ArgParser::toCallMap::iterator i = this->toCall.begin();
        i != this->toCall.end(); ++i) // TODO spacing here too?
        if(i->first != NULL)
            // call the stored function with its argument
            (*(i->first))(i->second);
}

// vim:ts=4 et sw=4 sts=4
