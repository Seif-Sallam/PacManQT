#ifndef SUPERPELLET_H
#define SUPERPELLET_H

#include "pellet.h"

//A class that derives from the Pellet class that has only a different texture
class SuperPellet : public Pellet
{
public:
    SuperPellet(int initialRow, int initialColumn, Vector2 size, Vector2 offset, QString pixmapFile = "SuperPellet.png");
};

#endif // SUPERPELLET_H
