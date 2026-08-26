#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>

class Battle;

class SaveManager
{
public:
    static bool saveGame(const Battle& battle, const std::string& filename);
    static bool loadGame(Battle& battle, const std::string& filename);
};

#endif