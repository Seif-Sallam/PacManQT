#include "ghost.h"

Ghost::Ghost(int initialRow, int initialColumn, int** data, Vector2 size, Vector2 offset,
             const QString& ghostName, int** adjMatrix, int matrixSize)
{
    //Seeding the random Function
    qsrand(time(0));

    //I only need to equate the pointers, we don't really need to do anything else with it anyway
    //so I don't need to keep copying the data around and have different copies of it for each object
    m_boardData = data;
    //Passing in the adjacincy matrix.
    m_adjMatrix = adjMatrix;
    m_matrixSize = matrixSize;
    //The steps the ghost takes
    m_steps = 0;

    //Initalizing the variables
    m_size = size;
    m_offset = offset;
    m_row = initialRow;
    m_column = initialColumn;
    m_startRow = m_row;
    m_startColumn = m_column;
    m_direction = Vector2(0, 0);

    //Loading Pixmaps
    m_originalGhostPixmap.load(ghostName);
    m_originalGhostPixmap = m_originalGhostPixmap.scaledToWidth(size.x);
    m_originalGhostPixmap = m_originalGhostPixmap.scaledToHeight(size.y);

    m_eadibleGhostPixmap.load("BlueGhost.png");
    m_eadibleGhostPixmap = m_eadibleGhostPixmap.scaledToWidth(size.x);
    m_eadibleGhostPixmap = m_eadibleGhostPixmap.scaledToHeight(size.y);

    setPixmap(m_originalGhostPixmap);

    //Setting the position according to the reversed map policy (Check the Map class)
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);

    //initializing the timers
    //m_moveTimer = new QTimer(this);
    m_deathTimer = new QTimer(this);

    //Connecting the timers to the slots
    connect(m_deathTimer, SIGNAL(timeout()),this, SLOT(StartMovement()));

    //We made a custom advance for the ghost only cause we wanted it to move SLOWER than the player.
    //connect(m_moveTimer, SIGNAL(timeout()), this, SLOT(GhostAdvance()));
    //m_moveTimer->start(200);


    //setting the states flags
    m_eatable = false;
    m_canMove = true;
}

//Setting the intellegance flag
void Ghost::SetIntelligence(bool random, bool dumb)
{
    m_random = random;
    m_dumb = dumb;
    m_intellgant = false;
    if(random)
    {
        m_intellgant = false;
        m_dumb = false;
    }
    if(!random && !dumb)
        m_intellgant = true;
}

void Ghost::SetPacCord(Vector2 position)
{
    m_pacCord = position;
}

void Ghost::GhostAdvance()
{
    if(m_canMove)
    {
        //Making it move in the correct direction
        if (m_direction.x == -1 && m_column - 1 < 0) // Left Portal
            m_column = 22;
        else if (m_direction.x == 1 && m_column + 1 > 22) // Right Portal
            m_column = 1;
        else if (m_direction.y == 1 && m_boardData[m_column][m_row + 1] != -1) // Down
            m_row++;
        else if(m_direction.y == -1 && m_boardData[m_column][m_row - 1] != -1) // Up
            m_row--;
        else if (m_direction.x == 1 && m_boardData[m_column + 1][m_row] != -1) // Right
            m_column++;
        else if(m_direction.x == -1 && m_boardData[m_column - 1][m_row] != -1) // Left
            m_column--;
        setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);

        //Update the movement direction according to the intellegince
        UpdateDirection();
    }
}



void Ghost::SetRow(int newRow)
{
    m_row = newRow;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}
void Ghost::SetColumn(int newColumn)
{
    m_column = newColumn;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}

int Ghost::GetRow()
{
    return m_row;
}

int Ghost::GetColumn()
{
    return m_column;
}

void Ghost::GotEaten()
{
    //If this function was called, the ghost will return to the house and start waiting 4.5 seconds
    //before he can get back to movement
    RetunToHouse();
    m_deathTimer->start(4500);
    m_canMove = false;
}

void Ghost::StartMovement()
{
    //This slot gets called after 4.5 seconds from calling Ghost::GotEaten()
    m_deathTimer->stop();
    m_canMove = true;
}

void Ghost::MakeEdiable(bool value)
{
    //Chceks if the ghost should be eadiable or not and setting the Pixmap to the correct one
    if (value)
    {
        m_eatable = true;
        setPixmap(m_eadibleGhostPixmap);
    }
    else
    {
        m_eatable = false;
        setPixmap(m_originalGhostPixmap);
    }
}

//Setting the values to the initial values for the row and the column
void Ghost::RetunToHouse(){
    m_row = m_startRow;
    m_column = m_startColumn;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}

void Ghost::CanMove(bool value)
{
    m_canMove = value;
}

void Ghost::UpdateDirection()
{
    if(m_dumb)
    {
        //increase the steps every frame.
        m_steps++;
        if(m_steps == 11 || m_steps >= m_walkingPath.size())
        {
            m_steps = 1; // it has to equal to 1 to start from the NEXT position not the one the ghost is at.
            m_walkingPath.clear();
            int startingNode = m_boardData[m_column][m_row]; // Current node index
            int finalNode = m_boardData[m_pacCord.x][m_pacCord.y]; // Final Node index
            //To make it choose a differnet place away from pac man
            if(m_eatable)
            {
                finalNode = finalNode - 20;
                if(finalNode < 0)
                {
                    finalNode = rand() % 20;
                }
            }
            // finding the shortest path using Dijkstra
            SolveDijkstra(startingNode, finalNode);
            //Pushing back the final node again so that the vector wouldnot go out of range when it reaches pacman
            m_walkingPath.push_back(finalNode);
        }

        //Determining the next direction according to the next node and current node.
        int nextNode = m_walkingPath[m_steps];
        int currentNode = m_boardData[m_column][m_row];
        // going to portal left and then going left
        if((currentNode == 1 && nextNode == 0) || (currentNode == 0 && nextNode == 218) || (currentNode == 218 && nextNode == 217))
            m_direction = Vector2(-1, 0);
        // going to portal right and then going right
        else if((currentNode == 217 && nextNode == 218) || (currentNode == 218 && nextNode == 0) || (currentNode == 0 && nextNode == 1))
            m_direction = Vector2(1, 0);
        else if(currentNode - nextNode == 1) // up
            m_direction = Vector2(0, -1);
        else if(currentNode - nextNode == -1) // down
            m_direction = Vector2(0, 1);
        else if(currentNode - nextNode > 0) // left
            m_direction = Vector2(-1, 0);
        else if(currentNode - nextNode < 0) // right
            m_direction = Vector2(1, 0);
        else if(currentNode - nextNode == 0) // stop if on the node
            m_direction = Vector2(0,0);
    }
    if(m_intellgant)
    {
        //Finds the shortest path every single frame
        m_walkingPath.clear();
        //Finding the starting node (current Node)
        int startingNode = m_boardData[m_column][m_row];
        //Finding the destiniation node
        int finalNode = m_boardData[m_pacCord.x][m_pacCord.y];
        //To make it choose a differnet place away from pac man
        if(m_eatable)
        {
            finalNode = finalNode - 20;
            if(finalNode < 0)
            {
                finalNode = rand() % 20;
            }
        }

        SolveDijkstra(startingNode, finalNode);
        m_walkingPath.push_back(finalNode);

        int nextNode = m_walkingPath[1];
        int currentNode = m_boardData[m_column][m_row];
        // going to portal left and then going left
        if((currentNode == 1 && nextNode == 0) || (currentNode == 0 && nextNode == 218) || (currentNode == 218 && nextNode == 217))
            m_direction = Vector2(-1, 0);
        // going to portal right and then going right
        else if((currentNode == 217 && nextNode == 218) || (currentNode == 218 && nextNode == 0) || (currentNode == 0 && nextNode == 1))
            m_direction = Vector2(1, 0);
        else if(currentNode - nextNode == -1) // down
            m_direction = Vector2(0, 1);
        else if(currentNode - nextNode == 1) // up
            m_direction = Vector2(0, -1);
        else if(currentNode - nextNode > 0) // left
            m_direction = Vector2(-1, 0);
        else if(currentNode - nextNode < 0) // right
            m_direction = Vector2(1, 0);
        else if(currentNode - nextNode == 0) // stop if on the node
            m_direction = Vector2(0,0);
    }
    if(m_random)
    {
        if(m_column > 0 && m_column < 22)
        {
            QVector<Direction> availableTiles;
            if(m_boardData[m_column][m_row + 1] != -1 && lastDir != Direction::Up)
            {
                availableTiles.push_back(Direction::Down);
            }
            if(m_boardData[m_column][m_row - 1] != -1 && lastDir != Direction::Down)
            {
                availableTiles.push_back(Direction::Up);
            }
            if(m_boardData[m_column + 1][m_row] != -1 && lastDir != Direction::Left)
            {
                availableTiles.push_back(Direction::Right);
            }
            if(m_boardData[m_column - 1][m_row] != -1 && lastDir != Direction::Right)
            {
                availableTiles.push_back(Direction::Left);
            }
            int nextDir;
            if(availableTiles.size() > 0)
            {
                 nextDir = qrand() % availableTiles.size();
            }
            else
            {
                nextDir = 0;
                if(lastDir == Direction::Left)
                    availableTiles.push_back(Direction::Right);
                else if(lastDir == Direction::Right)
                    availableTiles.push_back(Direction::Left);
                else if(lastDir == Direction::Up)
                    availableTiles.push_back(Direction::Down);
                else if(lastDir == Direction::Down)
                    availableTiles.push_back(Direction::Up);
            }

            if(availableTiles[nextDir] == Direction::Up)
                m_direction = Vector2(0, -1);
            else if(availableTiles[nextDir] == Direction::Down)
                m_direction = Vector2(0, 1);
            else if(availableTiles[nextDir] == Direction::Left)
                m_direction = Vector2(-1, 0);
            else if(availableTiles[nextDir] == Direction::Right)
                m_direction = Vector2(1, 0);

            lastDir = availableTiles[nextDir];
        }
    }

}

void Ghost::SolveDijkstra(int startingNode, int finalNode)
{
    bool* vistedNodes = new bool[m_matrixSize];
    int* previousNodes = new int[m_matrixSize];
    int* pathCosts = new int[m_matrixSize];
    for (int i = 0; i < m_matrixSize; i++)
    {
        vistedNodes[i] = false;
        previousNodes[i] = startingNode;
        pathCosts[i] = m_adjMatrix[startingNode][i];
    }

    vistedNodes[startingNode] = true;
    pathCosts[startingNode] = 0;
    int minDistance = 100;
    int minIndex = 0;
    for (int c = 1; c < m_matrixSize; c++)
    {
        minDistance = 100;
        for (int j = 0; j < m_matrixSize; j++)
        {
            if (!vistedNodes[j] && pathCosts[j] < minDistance)
            {
                minDistance = pathCosts[j];
                minIndex = j;
            }
        }
        vistedNodes[minIndex] = true;
        for (int i = 0; i < m_matrixSize; i++)
        {
            if (!vistedNodes[i])
            {
                int distance = m_adjMatrix[minIndex][i] + pathCosts[minIndex];

                if (distance < pathCosts[i])
                {
                    previousNodes[i] = minIndex;
                    pathCosts[i] = distance;
                }
            }
        }
    }

    int j = finalNode;
    m_walkingPath.push_back(finalNode);
    while (j != startingNode)
    {
        j = previousNodes[j];
        m_walkingPath.push_back(j);
    }
    std::reverse(m_walkingPath.begin(), m_walkingPath.end());

    delete[] previousNodes;
    delete[] pathCosts;
    delete[] vistedNodes;
}
