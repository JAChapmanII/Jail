#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::dec;

#include <iomanip>
using std::setw;
using std::setfill;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "argparser.hpp"
#include "fileio.hpp"
#include "config.hpp"

#include "buffer.hpp"

int parseCommand(string command);
int printHelp();
int printVersion();
void dump(vector<string> fileNames);
void view(vector<string> fileNames);
void edit(vector<string> fileNames);

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    ArgParser mArgParser;
    mArgParser.setHelp();
    mArgParser.setHelpHeader("jail [switches] <file>\n");
    mArgParser.setHelpFooter(
            "\nYou may also wish to check the README for more information\n");

    mArgParser.add(ArgParser::SwitchName("--version", ""), &printVersion,
            "Print the version of jail which is running");
    mArgParser.add(ArgParser::SwitchName("--verbose", "-v"),
            &Config::setVerbose, "Set the output to be verbose");
    mArgParser.add(ArgParser::SwitchName("--quiet", "-q"), &Config::setQuiet,
            "Set the output to be quiet");

    mArgParser.add(ArgParser::SwitchName("--parse-command", ""), &parseCommand,
            "Attempt to say what jail would do with a command");
    mArgParser.add(ArgParser::SwitchName("--dump", ""), &Config::setDump,
            "Dump the contents of a file to the screen");
    mArgParser.add(ArgParser::SwitchName("--hex", ""), &Config::setHexMode,
            "When dumping, output in hexadecimal");
    mArgParser.add(ArgParser::SwitchName("--hex-dump", ""),
            &Config::setHexDump, "Combination of --hex and --dump");
    mArgParser.add(ArgParser::SwitchName("--width", "-w"), &Config::setWidth,
            "Sets the maximum width in columns of the hex dump output");
    mArgParser.add(ArgParser::SwitchName("", "-R"), &Config::setReadOnly,
            (string)"Puts jail into read-only mode, so that you cannot" +
            (string)"overwrite the backing file");

    mArgParser.parseArguments(argc, argv);
    mArgParser.runCommands();

    if(Config::isVerbose()) {
        cout << "Invoked as: " << mArgParser.getInvocationName() << "\n";

        vector<string> extraArguments = mArgParser.getExtraArguments();
        if(extraArguments.size() > 0) {
            cout << "These arguments were left over after switch parsing: \n";
            for(vector<string>::iterator i = extraArguments.begin();
                    i != extraArguments.end(); ++i)
                cout << ": " + (*i) << "\n";
        }
        cout << endl;
    }

    if(Config::isVerbose())
        cout << "We're being verbose!" << endl;

    if(Config::isDump())
        dump(mArgParser.getExtraArguments());
    else {
        if(Config::isReadOnly())
            view(mArgParser.getExtraArguments());
        else
            edit(mArgParser.getExtraArguments());
    }

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

int printVersion() {
    cout << "version: " << Config::getVersion() << endl;
    return 0;
}

void dump(vector<string> fileNames) {
    if(fileNames.empty())
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);
    file.open();
    int length = file.read();

    if(length < 0) {
        cerr << "There was a problem reading the file \""
            << fileName << "\" for dumping." << endl;
        return;
    }

    if(Config::isVerbose())
        cout << "File \"" << fileName << "\" is " << length << " bytes long.\n";

    if(!Config::isHexMode()) {
        cout << file.getData();
    } else {
        char *data = file.getData();
        int stop = (Config::getWidth() + 1) / 3;
        cout << hex;
        for(int i = 0; i < length; ++i) {
            cout << setw(2) << setfill('0') << (unsigned int)data[i] << ' ';
            if(((i+1) % stop) == 0)
                cout << '\n';
        }
        cout << dec;
    }
    cout << endl;

    return;
}

void view(vector<string> fileNames) {
    if(fileNames.empty())
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);
    file.open();
    int length = file.read();

    if(length < 0) {
        cerr << "There was a problem reading the file \""
            << fileName << "\" for viewing." << endl;
        return;
    }

    char *data = file.getData();
    Buffer mBuffer((string)data);
}

void edit(vector<string> fileNames) {
    cout << "We can't yet edit files, sorry!" << endl;
    return;
}

// vim:ts=4 et sw=4 sts=4
