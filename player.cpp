#include "player.h"

Player::Player(Vector2 position, int** boardData, Vector2 size, Vector2 offset,
               QVector<Pellet*>& pelletsVec, QVector<Ghost*>& ghostsVec)
    : m_size(size), m_offset(offset), m_pelletsVector(pelletsVec), m_ghostsVector(ghostsVec)
{
    m_invencible = false;
    m_gameEnded = false;
    m_score = 0;
    m_lives = 3;
    m_nextLifeScore = 3500;
    //I only need to equate the pointers, we don't really need to do anything else with it anyway
    //so I don't need to keep copying the data around and have different copies of it for each object
    this->m_boardData = boardData;

    m_row = position.x;
    m_col = position.y;
    startRow = m_row;
    startColumn = m_col;

    m_direction = Vector2();

    //Loading the pixmaps
    m_normalPacman.load("Pacman.png");
    m_normalPacman = m_normalPacman.scaledToWidth(size.x);
    m_normalPacman = m_normalPacman.scaledToHeight(size.y);

    m_angryPacman.load("AngryPacman.png");
    m_angryPacman = m_angryPacman.scaledToWidth(size.x);
    m_angryPacman = m_angryPacman.scaledToHeight(size.y);
    setPixmap(m_normalPacman);

    //Setting the position according to the reversed map policy (Check the Map class)
    setPos(m_offset.x + m_size.x * m_col, m_offset.y + m_size.y * m_row);

    //Setting the flag of the pixmap to be focusable
    setFlag(ItemIsFocusable);

    //Making the player have the fcous
    setFocus();

    //Connecting the invincibilityTimer's stoping with the stopinvencibility slot
    this->connect(&m_invencibilityTimer, SIGNAL(timeout()), this ,SLOT(StopInvencible()));
}

void Player::advance(int phase)
{
    //If the game didn't end
    if (!m_gameEnded)
    {
        //Check the direction and according to it, advance
        if(!phase || m_row < 0 || m_row > 22) return;

        //if the player hits the very left portel, move to the other part of the map
        if(m_direction.x == -1 && m_col - 1 < 0)
        {
            m_col = 22;
        }
        //if the player hits the very right portel, move to the other part of the map
        else if(m_direction.x == 1 && m_col + 1 > 22)
        {
            m_col = 0;
        }
        else if(m_direction.y == 1 && m_boardData[m_col][m_row + 1] > -1 && m_boardData[m_col][m_row + 1] != 108) // down
        {
            m_row++;
        }
        else if (m_direction.y == -1 && m_boardData[m_col][m_row - 1] > -1) // up
        {
            m_row--;
        }
        else if (m_direction.x == -1  && m_boardData[m_col - 1][m_row] > -1) // left
        {
            m_col--;
        }
        else if (m_direction.x == 1 && m_boardData[m_col + 1][m_row] > -1) // right
        {
            m_col++;
        }

        //Setting the position after the change
        setPos(m_offset.x + m_size.x * m_col, m_offset.y + m_size.y * m_row);

        //This is just for how QT works, sometimes pacman can lose focus if you click OUTSIDE the graphics scene
        //so We check if he lost the focus, we give it back to him
        if(!hasFocus())
        {
            setFocus();
        }

        //We handle collisions here
        HandleCollision();

        //if the number of pellets is 0, then we finish the game
        if(m_pelletsVector.size() == 0)
        {
            emit ChangeGameOverLabel(QString("GAME ENDED :D"));
            m_gameEnded = true;
            //the game ended so we make them go back to their house all the time
            for(int i = 0; i < m_ghostsVector.size(); i++)
            {
                m_ghostsVector[i]->MakeEdiable(false);
                m_ghostsVector[i]->RetunToHouse();
                m_ghostsVector[i]->CanMove(false);
            }
        }
        //adding one life after every 2000
        if(m_score >= m_nextLifeScore)
        {
            m_nextLifeScore += 5500;
            m_lives++;
            emit LifeChange(QString::number(m_lives));
        }
        // giving the ghosts the players position every time it moves
        for(int i = 0; i < m_ghostsVector.size(); i++)
        {
            m_ghostsVector[i]->SetPacCord(Vector2(m_col, m_row));
        }
    }
}

void Player::HandleCollision()
{
    //List of the colliding items
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (int i = 0; i < collidingItems.size(); i++)
    {
        //if it is a pellet we up the score by 10
        if(typeid(*collidingItems[i]) == typeid(Pellet))
        {
            //We don't care about the specific pellet, so we can just remove ANY pellet
            m_pelletsVector.erase(m_pelletsVector.begin());
            scene()->removeItem(collidingItems[i]);
            m_score += 50;
            //We change the score label by emiting the ScoreChanged signal
            emit ScoreChanged(QString::number(m_score));
        }
        //if it is a super pellet we up the score by 50 and make the player invincible for 10 seconds
        else if(typeid(*collidingItems[i]) == typeid(SuperPellet))
        {
            m_pelletsVector.erase(m_pelletsVector.begin());
            scene()->removeItem(collidingItems[i]);
            m_score += 100;
            emit ScoreChanged(QString::number(m_score));

            m_invencible = true;
            //We stop timer and then start it again,
            //because starting an already working timer doesn't override its start
            m_invencibilityTimer.stop();
            m_invencibilityTimer.start(10000);
            setPixmap(m_angryPacman);
            //Making the ghosts Blue and ediable
            for(int i = 0; i < m_ghostsVector.size(); i++)
            {
                m_ghostsVector[i]->MakeEdiable(true);
            }
        }
        //If the player hits a ghost
        else if(typeid(*collidingItems[i]) == typeid(Ghost))
        {
            //If the player is invencible, then eat the ghost and move him to the house and score +200
            if(m_invencible == true)
            {
                //move ghost to initial position
                m_score += 200;
                emit ScoreChanged(QString::number(m_score));
                //Casting to a Ghost pointer to be able to manipulate the current ghost
                Ghost * ghost = dynamic_cast<Ghost*>(collidingItems[i]);
                ghost->GotEaten();
            }
            else{
                //else. the lives are less by 1, the position is back to the start position and the ghosts return to their houses
                --m_lives;
                m_row = startRow;
                m_col = startColumn;
                setPos(m_offset.x + m_size.x * m_col, m_offset.y + m_size.y * m_row);
                emit LifeChange(QString::number(m_lives));
                for(int i = 0; i < m_ghostsVector.size(); i++)
                {
                    m_ghostsVector[i]->RetunToHouse();
                }
                //Check if the lives == 0
                if(m_lives == 0)
                {
                    //game over
                    for(int i = 0; i < m_ghostsVector.size(); i++)
                    {
                        m_ghostsVector[i]->RetunToHouse();
                        m_ghostsVector[i]->CanMove(false);
                    }
                    emit ChangeGameOverLabel(QString("GAME ENDED :D"));
                    m_gameEnded = true;
                    break;
                }
            }
        }
        else if(typeid(*collidingItems[i]) == typeid(Cherry))
        {
            //remove the item from the screen
            scene()->removeItem(collidingItems[i]);
            //adding 250 score to the player
            m_score += 250;
            emit ScoreChanged(QString::number(m_score));
            emit EatenCherry();
        }
    }
}

//Handling key presses
void Player::keyPressEvent(QKeyEvent *event)
{
    //If the game is still working we take the movement keys
    if (!m_gameEnded)
    {
        //This is a transform object that I will use to change the transformation of the player to rotate how I want
        QTransform pacManTransform;
        if (event->key() == Qt::Key_Up)
        {
            //Change the direction vector
            m_direction = Vector2(0, -1);

            //translate makes the positioning gets addressed from the CENTER of the pixmap
            pacManTransform.translate(this->pixmap().width()/2.0 , this->pixmap().height()/2.0);

            //rotating the pixmap
            pacManTransform.rotate(270);

            //making us address the pixmap from the top left corner again
            pacManTransform.translate(-this->pixmap().width()/2.0 , -this->pixmap().height()/2.0);
        }
        else if (event->key() == Qt::Key_Down)
        {
            m_direction = Vector2(0, 1);

            pacManTransform.translate(this->pixmap().width()/2.0 , this->pixmap().height()/2.0);
            pacManTransform.rotate(90);
            pacManTransform.translate(-this->pixmap().width()/2.0 , -this->pixmap().height()/2.0);
        }
        else if (event->key() == Qt::Key_Right)
        {
            m_direction = Vector2(1,0);

            pacManTransform.translate(this->pixmap().width()/2.0 , this->pixmap().height()/2.0);
            pacManTransform.rotate(0);
            pacManTransform.translate(-this->pixmap().width()/2.0 , -this->pixmap().height()/2.0);
        }
        else if (event->key() == Qt::Key_Left)
        {
            m_direction = Vector2(-1,0);

            pacManTransform.translate(this->pixmap().width()/2.0 , this->pixmap().height()/2.0);
            pacManTransform.rotate(180);
            pacManTransform.translate(-this->pixmap().width()/2.0 , -this->pixmap().height()/2.0);
        }
        //Setting the transform
        setTransform(pacManTransform);
    }

    //If we pressed on ESCAPE, we exit the game
    if(event->key() == Qt::Key_Escape)
    {
        exit(0);
    }
}

void Player::StopInvencible()
{
    //To stop the invencibility state
    m_invencibilityTimer.stop();
    m_invencible = false;
    setPixmap(m_normalPacman);
    for(int i = 0; i < m_ghostsVector.size(); i++)
    {
        m_ghostsVector[i]->MakeEdiable(false);
    }
}
