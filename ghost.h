#ifndef GHOST_H
#define GHOST_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>
#include "Vector2.h"
enum class Direction
{
    Right, Left, Up, Down, None
};

class Ghost :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Ghost(int initialRow, int initialColumn, int** data,
          Vector2 size, Vector2 offset, const QString& ghostName, int** adjMatrix, int matrixSize);
    void SetIntelligence(bool random = false, bool dumb = false);
    void SetPacCord(Vector2 position);
    void SetRow(int newRow);
    void SetColumn(int newColumn);
    int GetRow();
    int GetColumn();
    void GotEaten();
    void MakeEdiable(bool value);
    void RetunToHouse();
    void CanMove(bool value);
private:
    void UpdateDirection();
    void SolveDijkstra(int startingNode, int finalNode);
public slots:
    void GhostAdvance();
    void StartMovement();

private:
    //Attributes needed for the movement and changing position
    int m_row, m_column, m_startRow, m_startColumn;
    Vector2 m_size, m_offset, m_direction;

    Direction lastDir = Direction::None;
    int** m_boardData;
    //Attributes needed for the pathfinding algorithm
    int** m_adjMatrix;
    int m_matrixSize;
    int m_steps;
    QVector<int> m_walkingPath;
    Vector2 m_pacCord; // pacman's coordinates

    //Timers
    //QTimer* m_moveTimer;
    QTimer* m_deathTimer;

    //Used pixmaps
    QPixmap m_originalGhostPixmap;
    QPixmap m_eadibleGhostPixmap;

    //States
    bool m_eatable;
    bool m_canMove;
    bool m_dumb;
    bool m_random;
    bool m_intellgant;
};

#endif // GHOST_H
