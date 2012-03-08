#ifndef UTIL_HPP
#define UTIL_HPP

#include <map>
#include <functional>
#include <string>
#include <vector>

namespace util {
    // An std::find wrapper that automatically searches the entire container
    template<typename K, typename V>
            typename std::map<K, V>::iterator find(
                    std::map<K, V> &map, K &value);
    template<typename T, typename V>
            typename T::iterator find(T &container, V &value);

    // A simple util::find wrapper that checks to see if a container has a key
    template<typename T, typename V>
            bool contains(T &container, V &value);

    // A std::find_if wrapper which automatically searches the entire container
    template<typename T, typename V>
            typename T::iterator find_if(T &container,
                    const std::function<bool(V)> &predicate);

    // A simple function to convert a string to a type supporting stream ops
    template<typename T>
            T fromString(const std::string &str);

    // Returns true if a string starts with a prefix
    bool startsWith(std::string str, std::string start);

    // Split a string into a list of separated fields
    std::vector<std::string> split(std::string str, std::string on);
    // Join a vector of strings into a single
    template<typename T>
            std::string join(std::vector<T> fields, std::string separator = ",");

    // Removes contents of remove from either side of str
    std::string trim(const std::string &str, std::string remove = " \t\r\n");
}

#include "util.imp"

#endif // UTIL_HPP
// vim:ts=4 et sw=4 sts=4
