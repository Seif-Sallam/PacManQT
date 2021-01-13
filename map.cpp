#include "map.h"

Map::Map(QGraphicsScene* scene)
{
    m_initalized = false;
    m_mapSize = Vector2(23,23);
    m_size = Vector2(25, 25);
    m_offset = Vector2(0,0);
    m_boardData = nullptr;
    m_boardImages = nullptr;
    Init(scene);
}

void Map::Init(QGraphicsScene *scene)
{
    //If it was already inialized then skip this
    if(!m_initalized)
    {
        LoadBoardData();
        BindImages(scene);
        m_initalized = true;
    }
}

void Map::LoadBoardData()
{
    //Allocating memory for the board data
    m_boardData = new int*[m_mapSize.x];

    //Allocating memory to the array to make it a 2D Array
    for (int i = 0; i < m_mapSize.x; i++) {
        m_boardData[i] = new int[m_mapSize.y];
    }

    //Opening the file and checking if it was opened or not
    QFile boardFile("Board.txt");
    boardFile.open(QFile::ReadOnly);
    if(!boardFile.isOpen())
    {
        qDebug() << "File Was not opened!";
        std::exit(0);
    }

    QTextStream stream(&boardFile);
    QString tempString;
    for (int i = 0; i < m_mapSize.x; i++) {
        for (int j = 0; j < m_mapSize.y; j++) {
            stream >> tempString;
            m_boardData[i][j] = tempString.toInt();
        }
    }
    boardFile.close();

    //By far, this means that the data in the board are existant and can be used
    /*
        The board data was initally read REVERSED as the board was REVERESED.
        This made us change the perspective when we were dealing with any positioning using Rows & Columns.
        We had to interchange the rows and the columns.
    */
}

void Map::BindImages(QGraphicsScene* drawingScene)
{
    //ALlocating memory for the PixmapItem array
    m_boardImages = new QGraphicsPixmapItem*[m_mapSize.x];
    for(int i = 0; i < m_mapSize.x; i++)
    {
        m_boardImages[i] = new QGraphicsPixmapItem[m_mapSize.y];
    }

    //Declaring the pixmap on the spot
    //P.S: Textures taken from this link:
    //https://opengameart.org/content/all-textures-for-the-labyrinth-of-the-fertile-void
    QPixmap wall("Wall.png");
    QPixmap walkPath("WalkPath.png");
    wall = wall.scaledToWidth(m_size.x);
    wall = wall.scaledToHeight(m_size.y);
    walkPath = walkPath.scaledToWidth(m_size.x);
    walkPath = walkPath.scaledToHeight(m_size.y);

    //Going through the whole map to add the textures to the pixmap and then to the scene
    for(int i = 0; i < m_mapSize.x; i++)
    {
        for(int j = 0; j < m_mapSize.y; j++)
        {
            //If the data == -1 we add a wall, other than that, it is a walking Path
            if(m_boardData[j][i] == -1)
            {
                m_boardImages[j][i].setPixmap(wall);
            }
            else
            {
                m_boardImages[j][i].setPixmap(walkPath);
            }
            //Setting the correct position of the pixmap
            m_boardImages[j][i].setPos(m_offset.x + j * m_size.x, m_offset.y + i * m_size.y);
            drawingScene->addItem(&m_boardImages[j][i]);

            //Adding the pellets and the super pellets to the scene
            if(m_boardData[j][i] >= 0)
            {
                Vector2 pos = {i , j};
                if((pos == Vector2(17, 19)) || (pos == Vector2(17, 3)) || (pos == Vector2(4, 2)) || (pos == Vector2(4, 20)))
                {
                    m_pelletsVector.push_back(new SuperPellet(j, i, m_size, m_offset));
                    drawingScene->addItem(m_pelletsVector.back());
                }
                else if(pos != Vector2(11, 10) && pos != Vector2(11, 11) && pos != Vector2(11, 12) && pos != Vector2(10,11))
                {
                    m_pelletsVector.push_back(new Pellet(j, i, m_size, m_offset));
                    drawingScene->addItem(m_pelletsVector.back());
                }
            }

            //P.S.: we added the pellets AFTER adding the map texutres to the scene because we wanted
            //      to see the pellets on TOP of the map texutres, so we had to draw it LAST.
        }
    }
}

//Returning the boardData
//This will return a nullptr if it was not initialized
int** Map::GetBoardData()
{
    return m_boardData;
}

//Deleting all the used memory and nullifying the pointers
void Map::DeleteMemory()
{
    for(int i = 0; i < m_mapSize.x; i++)
    {
        delete[] m_boardImages[i];
        m_boardImages[i] = nullptr;

        delete[] m_boardData[i];
        m_boardData[i] = nullptr;
    }

    delete[] m_boardImages;
    m_boardImages = nullptr;

    delete[] m_boardData;
    m_boardData = nullptr;

    m_pelletsVector.clear();
    m_initalized = false;
}

Vector2 Map::GetPixmapSize()
{
    return m_size;
}

Vector2 Map::GetOffsetValues()
{
    return m_offset;
}

QVector<Pellet *> &Map::GetPelletsVectorRef()
{
    return m_pelletsVector;
}

Map::~Map()
{
    DeleteMemory();
}
