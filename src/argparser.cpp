#include "argparser.hpp"
using std::string;
using std::vector;

ArgParser::ArgParser(string longCommand, string shortCommand, bool hasArg,
        ArgParser::CallbackFunction function) {
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

int ArgParser::parseArguments(int &argc, char ***argv) {
    // copy-pasta loop? XD wait why can't......
}

void ArgParser::runCommands() {
}

// vim:ts=4 et sw=4 sts=4
