#ifndef MAP_H
#define MAP_H

#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QVector>
#include <QGraphicsScene>
#include <QImage>
#include "Vector2.h"
#include "superpellet.h"

/*
    Game Array
                    -1 <- x-> +1
    |---|--|---||---||---||---||---||---||---||---||---|
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 -1 |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 ^  |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 |  |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 y  |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 |  |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 v  |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
 +1 |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |   |  |   ||   ||   ||   ||   ||   ||   ||   ||   |
    |---|--|---||---||---||---||---||---||---||---||---|
*/

// This is a class which will hold in the whole Game Map and will instantiate pellets to eat
class Map
{
//Public funcitons
public:
    //A constructor that will take the scene it will work on
    Map(QGraphicsScene* scene);

    //Initializing the Map.
    void Init(QGraphicsScene* scene);

    //A function to return the data of the board, to send it to whatever object needs it
    int** GetBoardData();

    //Deleting the memory used in the map
    void DeleteMemory();

    //Getting the size of the pixmap to determine the right scale for the rest of the project
    Vector2 GetPixmapSize();

    //Getting the offset value to determine the correct positioning of the pixmaps
    Vector2 GetOffsetValues();

    //Getting a REFERENCE to the pellets vector. I don't want to send a copy.
    //I want to change the same exact vector
    QVector<Pellet*>& GetPelletsVectorRef();

    //A destructor that calls in DeleteMemory();
    ~Map();

//Private functions
private:
    void LoadBoardData(); // To load the data of the board from the file
    void BindImages(QGraphicsScene* drawingScene); // Binding the images to the pixmaps

private:
    Vector2 m_size;
    QGraphicsPixmapItem** m_boardImages;
    int** m_boardData;
    Vector2 m_offset;
    Vector2 m_mapSize;
    bool m_initalized;
    QVector<Pellet*> m_pelletsVector;

};

#endif // MAP_H
