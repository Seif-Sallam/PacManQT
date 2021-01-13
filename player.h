#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>
#include "Vector2.h"
#include "superpellet.h"
#include "ghost.h"
#include "cherry.h"

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(Vector2 position, int** m_boardData,
           Vector2 size, Vector2 offset, QVector<Pellet*>& pelletsVec, QVector<Ghost*>& ghostsVec);

private slots:
    void keyPressEvent(QKeyEvent *event) override;
    void StopInvencible();

signals:
    void ScoreChanged(QString);
    void LifeChange(QString);
    void ChangeGameOverLabel(QString);
    void EatenCherry();

protected:
    void advance(int phase) override;

private:
    void HandleCollision();

private:
    int** m_boardData;
    int m_row, m_col, startRow, startColumn, m_lives, m_score;
    int m_nextLifeScore;
    Vector2 m_direction, m_size, m_offset;

    QPixmap m_normalPacman;
    QPixmap m_angryPacman;

    //Reference variables for the vectors, to avoid copying
    QVector<Pellet*>& m_pelletsVector;
    QVector<Ghost*>& m_ghostsVector;

    bool m_gameEnded;
    bool m_invencible;

    QTimer m_invencibilityTimer;

};

#endif // PLAYER_H
