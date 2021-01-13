#include "superpellet.h"

//This does nothing but construct a pellet with the file name of the super pellet texutre
SuperPellet::SuperPellet(int initialRow, int initialColumn, Vector2 size, Vector2 offset, QString pixmapFile)
    : Pellet(initialRow, initialColumn, size, offset, pixmapFile)
{
}
