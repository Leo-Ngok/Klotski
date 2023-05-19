#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamecontroller.h"
#include "statecontroller.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *);
signals:
    void mouseClick(QMouseEvent *);
private:
    Ui::MainWindow *ui;
    GameController *controller;
    StateController *stateController;
};
#endif // MAINWINDOW_H
