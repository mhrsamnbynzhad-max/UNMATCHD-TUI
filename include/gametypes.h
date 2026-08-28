#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <string>
class Card;
class Fighter;

struct ZoneCheckResult {
   bool allow;
   std::string blocker = "";
};

struct ExecuteOrder {
    Card* acard;
    Fighter* aowner;
    Fighter* atarget;
    Card* bcard;
    Fighter* bowner;
    Fighter* btarget;
};

#endif