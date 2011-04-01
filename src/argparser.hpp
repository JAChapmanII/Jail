#include <string>
#include <vector>
#include <map>

/**
 * Parse arguments given to the program in standard form
 */
// TODO: template-ize this thing?
class ArgParser {
    public:
        /// A long/short switch name pair
        typedef std::pair<std::string, std::string> SwitchName;

        /// Type for a void callback function invoked on different switches
        typedef int (*VoidCallbackFunction)();

        /// Type for a string callback function invoked on different switches
        typedef int (*StringCallbackFunction)(std::string);
        /// Typedef for the map of (string) functions to run after parsing
        typedef std::multimap<StringCallbackFunction, std::string> StringCallMap;
        /// An entry in a StringCallMap
        typedef std::pair<StringCallbackFunction, std::string> SCMapEntry;

        /// A map between switch name and a string callback function
        typedef std::map<SwitchName, StringCallbackFunction> StringCbMap;
        /// An entry in the stringCallbackMap
        typedef std::pair<SwitchName, StringCallbackFunction> SCbMapEntry;

        /// A map between switch name and a void callback function
        typedef std::map<SwitchName, VoidCallbackFunction> VoidCbMap;
        /// An entry in the voidCallbackMap
        typedef std::pair<SwitchName, VoidCallbackFunction> VCbMapEntry;

        /// A map between switch name and help description
        typedef std::map<SwitchName, std::string> HelpMap;
        /// An entry in the helpMap
        typedef std::pair<SwitchName, std::string> HelpMapEntry;

        /// Construct an empty ArgParser
        ArgParser();
        /// Construct an ArgParser with a default function
        ArgParser(VoidCallbackFunction function);

        /// Add another (string) switch to look for
        void add(SwitchName commandName, StringCallbackFunction function);
        /// Add another (string) switch to look for, with help
        void add(SwitchName commandName, StringCallbackFunction function,
                std::string help);

        /// Add another (void) switch to look for
        void add(SwitchName commandName, VoidCallbackFunction function);
        /// Add another (void) switch to look for, with help
        void add(SwitchName commandName, VoidCallbackFunction function,
                std::string help);

        /// Set the default callback function
        void setDefault(VoidCallbackFunction function);

        /// Sets the help of a function
        void addHelp(SwitchName commandName, std::string help);

        /// Sets whether help is enabled or not
        void setHelp(bool nval = true);
        /// Tests whether help is enabled or not
        bool getHelp() const;

        /// Do not look for the specified switch any longer
        void remove(SwitchName commandName);
        /// Remove a switch, looking in only one map
        void remove(SwitchName commandName, bool takesArgument);

        /// Add a (string) function to run directly into the call map
        void queue(StringCallbackFunction function, std::string argument);
        /// Add a (void) function to run directly into the call map
        void queue(VoidCallbackFunction function);

        /// Find a switch in the void function call map
        VoidCallbackFunction *findVoidCBF(std::string switchName);
        /// Find a switch in the string function call map
        StringCallbackFunction *findStringCBF(std::string switchName);

        /// Parse the arguments given the the main function
        int parseArguments(int argc, char **argv);

        /// Run the commands this has found need to be executed
        void runCommands() const;
        /// Run the commands which take an argument
        void runStringCommands() const;
        /// Run the commands which take no arguments
        void runVoidCommands() const;
        /// Run the default command
        void runDefaultCommand() const;

        /// Prints help message
        void printHelp() const;

        /// Returns the leftover arguments
        std::vector<std::string> getExtraArguments() const;
        /// Returns the program invocation name
        std::string getInvocationName() const;

    protected:
        /// Map of names to functions that don't take an argument
        VoidCbMap voidCallbackMap;
        /// Map of names to functions that take a string
        StringCbMap stringCallbackMap;

        /// Whether or not help is enabled
        bool helpIsEnabled;
        /// Map between switch names and help messages
        HelpMap helpMap;

        /// vector of (void) functions to call on runCommands invocation
        std::vector<VoidCallbackFunction> voidCalls;
        /// map of (string) functions to call on runCommands invocation
        StringCallMap stringCalls;
        /// default function to call
        VoidCallbackFunction defaultCall;

        /// vector of leftover arguments
        std::vector<std::string> extraArguments;
        /// invocation name
        std::string invocationName;
};

// vim:ts=4 et sw=4 sts=4
