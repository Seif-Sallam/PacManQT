#ifndef PELLET_H
#define PELLET_H

#include <QGraphicsPixmapItem>
#include "Vector2.h"

//A class for the Pellet object
class Pellet : public QGraphicsPixmapItem
{
//Public functions;
public:
    Pellet(int initialRow, int initialColumn, Vector2 size, Vector2 offset, QString pixmapFile = "Pellet.png");
protected:
    int m_row;
    int m_col;
};



#endif // PELLET_H
