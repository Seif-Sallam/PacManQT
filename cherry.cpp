#include "cherry.h"


Cherry::Cherry(int initialRow, int initialColumn, Vector2 size, Vector2 offset)
{
    m_row = initialRow;
    m_column = initialColumn;
    QPixmap image("Cherry.png");
    image = image.scaledToWidth(size.x);
    image = image.scaledToHeight(size.y);
    setPixmap(image);
    setPos(offset.x + m_column * size.x, offset.y + m_row * size.y);
    m_size = size;
    m_offset = offset;
}

void Cherry::SetRow(int newRow)
{
    m_row = newRow;
    setPos(m_offset.x + m_column * m_size.x, m_offset.y + m_row * m_size.y);
}

void Cherry::SetColumn(int newColumn)
{
    m_column = newColumn;
    setPos(m_offset.x + m_column * m_size.x, m_offset.y + m_row * m_size.y);
}
