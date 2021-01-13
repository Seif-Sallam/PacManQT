#ifndef CHERRY_H
#define CHERRY_H

#include <QGraphicsPixmapItem>
#include "Vector2.h"

class Cherry : public QGraphicsPixmapItem
{
public:
    Cherry(int initialRow, int initialColumn, Vector2 size, Vector2 offset);
    void SetRow(int newRow);
    void SetColumn(int newColumn);
private:
    int m_row, m_column;
    Vector2 m_size;
    Vector2 m_offset;
};

#endif // CHERRY_H
