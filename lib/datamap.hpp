#ifndef DATAMAP_HPP
#define DATAMAP_HPP

#include <map>
#include <string>

class DataMap {
    public:
        typedef std::map<std::string, std::string> VMap;

        DataMap(std::string defaultScope = "core");

        long load(std::string file);

        static std::string extractScope(std::string scoped_variable);
        static std::string extractVariable(std::string scoped_variable);

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

        VMap &operator[](std::string scope);
        std::map<std::string, VMap> &map();

        std::map<std::string, VMap>::iterator begin();
        std::map<std::string, VMap>::iterator end();

        VMap::iterator begin(std::string scope);
        VMap::iterator end(std::string scope);
    protected:
        std::string m_defaultScope;
        std::map<std::string, VMap> m_map;
};

#endif // DATAMAP_HPP
// vim:ts=4 et sw=4 sts=4
