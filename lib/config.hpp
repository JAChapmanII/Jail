#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

#include "datamap.hpp"

namespace config {
    extern DataMap map;
    void init();

    std::vector<std::string> parse(std::vector<std::string> args);
    long load(std::string file = "");

    bool hasScope(std::string scope);
    bool has(std::string scoped_variable);
    bool has(std::string scope, std::string variable);

    std::string get(std::string scoped_variable);
    std::string get(std::string scope, std::string variable);

    bool isPositive(std::string scoped_variable);
    bool isPositive(std::string scope, std::string variable);

    double asDouble(std::string scoped_variable);
    double asDouble(std::string scope, std::string variable);

    int asInteger(std::string scoped_variable);
    int asInteger(std::string scope, std::string variable);

    std::string null();

    template<typename T>
            void set(std::string scoped_variable, T value);
    template<typename T>
            void set(std::string scope, std::string variable, T value);

    void invert(std::string scoped_variable);
    void invert(std::string scope, std::string variable);

    std::map<std::string, DataMap::VMap>::iterator begin();
    std::map<std::string, DataMap::VMap>::iterator end();
}

#endif // CONFIG_HPP
// vim:ts=4 et sw=4 sts=4
