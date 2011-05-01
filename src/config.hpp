#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

int setVerbose();
int setQuiet();
int setDump();
int setHexMode();
int setHexDump();

bool isVerbose();
bool isQuiet();
bool isDump();
bool isHexMode();

int setWidth(std::string w);
int getWidth();

#endif // CONFIG_HPP
// vim:ts=4 et sw=4 sts=4
