#include "argparser.hpp"
using std::string;
using std::vector;

ArgParser::ArgParser(string longCommand, string shortCommand, bool hasArg,
    ArgParser::CallbackFunction function) {

}

void ArgParser::add(std::string longCommand, std::string shortCommand,
    bool hasArg, ArgParser::CallbackFunction function) {
}

void ArgParser::remove(std::string commandName) {
}

int ArgParser::parseArguments(int &argc, char ***argv) {
}

void ArgParser::runCommands() {
}
