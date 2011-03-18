#include <string>
#include <vector>

class ArgParser {
    public:
        typedef int (*CallbackFunction)(std::string);
        ArgParser(std::string longCommand, std::string shortCommand,
            bool hasArg, CallbackFunction function);

        void add(std::string longCommand, std::string shortCommand,
            bool hasArg, ArgParser::CallbackFunction function);
        void remove(std::string commandName);

        int parseArguments(int &argc, char ***argv);
        void runCommands();

    protected:
        std::vector<std::string> longCommands;
        std::vector<std::string> shortCommands;
        std::vector<bool> hasArgs;
        std::vector<ArgParser::CallbackFunction> callbackFunctions;
};

// vim:ts=4 et sw=4 sts=4
