#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include "player.h"
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void TextChange(QString text);
    void SetLifeText(QString text);
    void SetGameOverLabel(QString text);
    void AddCherry();
    void StartCherryTimer();
private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsRectItem* rect;
    QTimer* timer;
    Player* player;
    Map* tileMap;
    QVector<Ghost*> ghostsVector;
    Cherry* cherry;
    QTimer* cherrySpawner;
    int matrixSize;
    int** adjMatrix;
};

#endif // MAINWINDOW_H
