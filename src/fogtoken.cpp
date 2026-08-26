#include "fogtoken.h"

   FogToken :: FogToken()
    {
        position = nullptr;
    }

    void FogToken :: setPosition(Zone* z)
    {
        position = z;
    }

    Zone* FogToken :: getPosition() const
    {
        return position;
    }
    