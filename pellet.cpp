#include "pellet.h"

Pellet::Pellet(int initialRow, int initialColumn, Vector2 size, Vector2 offset, QString pixmapFile)
{
    // Setting the pixmap and resizing it to the correct size
    QPixmap pelletImage(pixmapFile);
    pelletImage = pelletImage.scaledToWidth(size.x);
    pelletImage = pelletImage.scaledToHeight(size.y);
    setPixmap(pelletImage);

    //Setting its position to whatever the position provided was according to the reversed map policy (Check the Map class)
    setPos(offset.x + size.x * initialRow, offset.y + size.y * initialColumn);
    m_row = initialRow;
    m_col = initialColumn;
}
