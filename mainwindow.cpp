#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new GameController(this);
    QObject::connect(this, SIGNAL(mouseClick(QMouseEvent *)),
                     controller, SLOT(OnMouseClick(QMouseEvent *)));
    controller->LoadGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *e){
    emit mouseClick(e);
}
