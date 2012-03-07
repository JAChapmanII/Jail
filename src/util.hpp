#ifndef UTIL_HPP
#define UTIL_HPP

#include <map>
#include <functional>
#include <string>

namespace util {
    // An std::find wrapper that automatically searches the entire container
    template<typename K, typename V>
            typename std::map<K, V>::iterator find(std::map<K, V> &map, K &value);
    template<typename T, typename V>
            typename T::iterator find(T &container, V &value);

    // A simple util::find wrapper that checks to see if a container has a key
    template<typename T, typename V>
            bool contains(T &container, V &value);

    // A std::find_if wrapper which automatically searches the entire container
    template<typename T, typename V>
            typename T::iterator find_if(T &container, std::function<bool(V)> predicate);

    // A simple function to convert a string to a type supporting stream ops
    template<typename T>
            T fromString(std::string &str);
}

#include "util.imp"

#endif // UTIL_HPP
// vim:ts=4 et sw=4 sts=4
