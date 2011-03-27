#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "argparser.hpp"
#include "fileio.hpp"

int parseCommand(string command);
int printHelp();
int printVersion();
void dump(vector<string> fileNames);

// TODO: Singleton config class with trampoline methods to static class members?
bool beVerbose = false;
bool beQuiet = false;
bool doDump = false;

int setVerbose() {
    beVerbose = true;
    beQuiet = false;
}
int setQuiet() {
    beQuiet = true;
    beVerbose = false;
}
int setDump() {
    doDump = true;
}

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    ArgParser mArgParser;
    mArgParser.setDefault(&printHelp);
    mArgParser.add(ArgParser::SwitchName("--help", "-h"), &printHelp);
    mArgParser.add(ArgParser::SwitchName("--version", ""), &printVersion);
    mArgParser.add(ArgParser::SwitchName("--verbose", "-v"), &setVerbose);
    mArgParser.add(ArgParser::SwitchName("--quiet", "-q"), &setQuiet);

    mArgParser.add(ArgParser::SwitchName("--parse-command", ""), &parseCommand);
    mArgParser.add(ArgParser::SwitchName("--dump", ""), &setDump);

    mArgParser.parseArguments(argc, argv);
    mArgParser.runCommands();

    cout << "We were invoked as: " << mArgParser.getInvocationName() << "\n";

    vector<string> extraArguments = mArgParser.getExtraArguments();
    if(extraArguments.size() > 0) {
        cout << "These arguments were left over after switch parsing: \n";
        for(vector<string>::iterator i = extraArguments.begin();
                i != extraArguments.end(); ++i)
            cout << ": " + (*i) << "\n";
    }

    cout << endl;

    if(beVerbose)
        cout << "We're being verbose!" << endl;
    if(beQuiet)
        cout << "Sssh... we're hunting wabbits..." << endl;

    if(doDump)
        dump(mArgParser.getExtraArguments());

    return 0;
}

int parseCommand(string command) {
    cout << command;
    switch(command[0]) {
        case ':':
            cout << " looks like an ex command to me!";
            break;
        case '/':
            cout << " looks like a search to me!";
            break;
        default:
            cout << " looks like a fine command to me!";
    }
    cout << endl;
    return 0;
}

int printHelp() {
    cout << "jail alpha -- Soon to be able to read files!\n"
        << "\n"
        << "jail [switches] <file>\n"
        << "\t--dump: dumps to contents of <file> to std out\n"
        << "\t--parse-command <command>: try to output what jail would do if"
        << " this command were used interactively\n"
        << "\t--help|-h: display this extremely helpful message\n"
        << "\t<nothing>: same as --help\n";
    cout << endl;
    return 0;
}

int printVersion() {
    cout << "version: Alpha!" << endl;
    return 0;
}

void dump(vector<string> fileNames) {
    if(fileNames.size() < 1)
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);
    file.open();
    int length = file.read();

    if(length < 0) {
        cerr << "There was a problem reading the file \""
            << fileName << "\"." << endl;
        return;
    }

    cout << "File \"" << fileName << "\" is " << length << " bytes long.\n";
    cout << file.getData() << endl;
    return;
}

// vim:ts=4 et sw=4 sts=4
