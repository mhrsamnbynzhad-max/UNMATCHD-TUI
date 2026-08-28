#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "battle.h"
#include <string>

class SaveManager
{
public:
    static bool saveGame(Battle& battle, const std::string& filename);
    static bool loadGame(Battle& battle, const std::string& filename);
};

#endif