#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setting the scene
    timer = new QTimer;
    scene = new QGraphicsScene();
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    //Dclaring the map
    tileMap = new Map(scene);

    // reading the matrix from the file
    matrixSize = 219;
    adjMatrix = new int*[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        adjMatrix[i] = new int[matrixSize];
    }

    QFile matrixFile("Matrix.txt");
    matrixFile.open(QIODevice::ReadOnly);
    QTextStream stream(&matrixFile);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            QString temp;
            stream >> temp;
            int value = temp.toInt();
            adjMatrix[i][j] = value;
        }
    }
    matrixFile.close();
    //Adding the ghosts one by one
    ghostsVector.push_back(new Ghost(11,10, tileMap->GetBoardData(), tileMap->GetPixmapSize(),
                                     tileMap->GetOffsetValues(), "BrownGhost.png", adjMatrix, matrixSize));
    ghostsVector.back()->SetIntelligence(true); // random
    scene->addItem(ghostsVector.back());

    ghostsVector.push_back(new Ghost(11,11, tileMap->GetBoardData(), tileMap->GetPixmapSize(),
                                     tileMap->GetOffsetValues(), "PurpleGhost.png", adjMatrix, matrixSize));
    ghostsVector.back()->SetIntelligence(false, true); // dumb
    scene->addItem(ghostsVector.back());

    ghostsVector.push_back(new Ghost(11,12, tileMap->GetBoardData(), tileMap->GetPixmapSize(),
                                     tileMap->GetOffsetValues(), "RedGhost.png", adjMatrix, matrixSize));
    ghostsVector.back()->SetIntelligence(); // very intellegant
    scene->addItem(ghostsVector.back());

    //Declaring the player
    Player* player = new Player(Vector2(17, 11), tileMap->GetBoardData(), tileMap->GetPixmapSize(),
                           tileMap->GetOffsetValues(), tileMap->GetPelletsVectorRef(), ghostsVector);

    scene->addItem(player);

    //Declaring the cherry object, we only need one thru the whole game even though we will eat it
    cherry = new Cherry(0, 11, tileMap->GetPixmapSize(), tileMap->GetOffsetValues());

    //Cherry spawning timer
    cherrySpawner = new QTimer();

    //Connecting the signals and slots
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));  //To advance in the scene

    //To spawn a cherry every now and then
    connect(cherrySpawner,SIGNAL(timeout()), this, SLOT(AddCherry()));

    //To change the score label
    connect(player, SIGNAL(ScoreChanged(QString)), this , SLOT(TextChange(QString)));

    //To change the life label
    connect(player, SIGNAL(LifeChange(QString)), this , SLOT(SetLifeText(QString)));

    //to change the gameover label
    connect(player, SIGNAL(ChangeGameOverLabel(QString)), this, SLOT(SetGameOverLabel(QString)));

    //To check if the player ate the cherry or not
    connect(player, SIGNAL(EatenCherry()), this, SLOT(StartCherryTimer()));
    for(int i = 0; i < ghostsVector.size(); i++)
    {
        connect(timer, SIGNAL(timeout()),ghostsVector[i], SLOT(GhostAdvance()));
    }
    cherrySpawner->start(10000); //10 seconds
    timer->start(150);
}

MainWindow::~MainWindow()
{
    delete ui;
    for (int i = 0; i < matrixSize; i++) {
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
}

void MainWindow::TextChange(QString text)
{
    ui->label_3->setText(text);
}

void MainWindow::SetLifeText(QString text)
{
    QString text2 = (QString)("Lives: ") + (QString)text;
    ui->LivesLabel->setText(text2);
}

void MainWindow::SetGameOverLabel(QString text)
{
    ui->gameOverLabel->setText(text);
}

void MainWindow::AddCherry()
{
    //we want to know where to put the cherry (either at the top or the bottom, we make it random chance)
    int chance = qrand() % 2;
    if(chance == 0)
    {
        cherry->SetRow(17);
        cherry->SetColumn(11);
    }
    else
    {
        cherry->SetRow(4);
        cherry->SetColumn(11);
    }
    //we readd the item to the screen
    scene->addItem(cherry);
    //we then stop the timer, that will rework again once it is eaten
    cherrySpawner->stop();
}

void MainWindow::StartCherryTimer()
{
    cherrySpawner->start(10000); // starting the timer with 20 seconds
}
