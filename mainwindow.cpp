#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new GameController(this);
    //controller->LoadGame();
    stateController = new StateController(this);
    stateController->setGameController(controller);
    stateController->LoadHomeScreen();
    QObject::connect(this, SIGNAL(mouseClick(QMouseEvent *)),
                     stateController, SLOT(MouseClick(QMouseEvent *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *e){
    emit mouseClick(e);
}
