#include "statecontroller.h"
#include "qapplication.h"
#include <algorithm>
#include <cstdlib>

StateController::StateController(QObject *parent)
    : QObject{parent}
{
    rHandler = new RecordHandler(parent);
    rHandler->ranking_filename = "Rankings.json";
    scoreBoard = nullptr;
}

void StateController::setGameController(GameController *gc) {
    gameController = gc;
    QObject::connect(this, SIGNAL(GameLoad()), gc, SLOT(LoadGame()));
    QObject::connect(gc, SIGNAL(BackToHomeScreen()), this, SLOT(LoadHomeScreen()));
    QObject::connect(gc, SIGNAL(GameEnded(QString)), rHandler, SLOT(SaveGame(QString)));
    QObject::connect(this, SIGNAL(MouseClicked(QMouseEvent*)), gc, SLOT(OnMouseClick(QMouseEvent*)));
    rHandler->setController(gc);
}

void StateController::LoadGameScreen() {
    currState = State::Game;
    UnloadHomeScreen();
    emit GameLoad();
}

void StateController::LoadHomeScreen() {
    currState = State::Home;
    playGame = new QPushButton(dynamic_cast<QWidget *>(parent()));
    quit = new QPushButton(dynamic_cast<QWidget *>(parent()));
    loadSettings = new QPushButton(dynamic_cast<QWidget *>(parent()));
    showRankings = new QPushButton(dynamic_cast<QWidget *>(parent()));
    dynamic_cast<QWidget *>(parent())->setGeometry(100,100,800, 600);
    playGame->    setGeometry(310,50,180,80);
    playGame->    setText("开始游戏");
    playGame->show();
    QObject::connect(playGame, SIGNAL(clicked(bool)), this, SLOT(LoadGameScreen()));
    quit->        setGeometry(310,200,180,80);
    quit->        setText("退出");
    quit->show();
    QObject::connect(quit, SIGNAL(clicked(bool)), this, SLOT(exit()));
    loadSettings->setGeometry(310,350,180,80);
    loadSettings->setText("设置");
    loadSettings->show();
    showRankings->setGeometry(310,500,180,80);
    showRankings->setText("排行榜");
    showRankings->show();
    QObject::connect(showRankings, SIGNAL(clicked(bool)), this, SLOT(LoadScoreBoard()));
    //QApplication::quit();
}

void StateController::UnloadHomeScreen() {

    playGame->deleteLater();
    QObject::disconnect(playGame, SIGNAL(clicked(bool)), this, SLOT(LoadGameScreen()));
    quit->deleteLater();
    QObject::disconnect(quit, SIGNAL(clicked(bool)),this, SLOT(exit()));
    loadSettings->deleteLater();
    showRankings->deleteLater();
    QObject::disconnect(showRankings, SIGNAL(clicked(bool)), this, SLOT(LoadScoreBoard()));
}

void StateController::exit() {
    QApplication::quit();
}


void StateController::MouseClick(QMouseEvent *e) {
    if(currState == State::Game) {
        emit MouseClicked(e);
        //gameController->OnMouseClick(e);
    }
}


void StateController::LoadScoreBoard() {
    currState = State::Scoreboard;
    UnloadHomeScreen();
    //QTableWidget *table;
    //QTableWidgetItem *items;
    scoreBoard = new QTableWidget(dynamic_cast<QWidget *>(parent()));

    // Initialize table headers and configurations
    scoreBoard->setColumnCount(4);
    scoreBoard->setSelectionMode(QAbstractItemView::SingleSelection);
    scoreBoard->setSelectionBehavior(QAbstractItemView::SelectRows);
    scoreBoard->setHorizontalHeaderLabels(QStringList() <<
                                          "用户名" << "日期" << "用时" << "步数");


    QFile *rankings = new QFile(rHandler->ranking_filename);
    QJsonArray ranking_list;
    if(rankings->exists()) {
        rankings->open(QFile::ReadOnly);
        QJsonDocument rankingDoc = QJsonDocument
                ::fromJson(rankings->readAll());
        rankings->close();
        rankings->deleteLater();
        rankings = nullptr;
        ranking_list = rankingDoc.array();
    }
    GameplayRecord record;
    for(auto && ranking_item : ranking_list) {
        auto item = ranking_item.toObject();
        if(item.contains("name") && item["name"].isString()) {
            record.name = item["name"].toString();
        } else {
            record.name = tr("unnamed");
        }
        if(item.contains("step") && item["step"].isDouble()) {
            record.step = item["step"].toInt();
        } else {
            record.step = 10000;
        }
        if(item.contains("time") && item["time"].isDouble()) {
            record.time_elapsed = item["time"].toInt();
        } else {
            record.time_elapsed = 10000;
        }
        if(item.contains("date") && item["date"].isString()) {
            record.date = item["date"].toString();
        } else {
            record.date = tr("1970-01-01");
        }
        records.push_back(record);
    }
    ShowLeastSteps();
    trigger_sortByStep = new QPushButton(dynamic_cast<QWidget *>(parent()));
    trigger_sortByTime = new QPushButton(dynamic_cast<QWidget *>(parent()));
    scoreBoardreturnToHome = new QPushButton(dynamic_cast<QWidget *>(parent()));

    trigger_sortByStep->setGeometry(550, 40, 180, 40);
    trigger_sortByStep->setText("按步长排序");
    trigger_sortByStep->show();
    QObject::connect(trigger_sortByStep, SIGNAL(clicked(bool)), this, SLOT(ShowLeastSteps()));


    trigger_sortByTime->setGeometry(550, 100, 180, 40);
    trigger_sortByTime->setText("按用时排序");
    trigger_sortByTime->show();
    QObject::connect(trigger_sortByTime, SIGNAL(clicked(bool)), this, SLOT(ShowLeastTimeElapsed()));


    scoreBoardreturnToHome->setGeometry(550, 160, 180, 40);
    scoreBoardreturnToHome->setText("退出");
    scoreBoardreturnToHome->show();
    QObject::connect(scoreBoardreturnToHome, SIGNAL(clicked(bool)), this, SLOT(ScoreboardToHome()));
}


void StateController::ShowLeastSteps() {
    std::sort(records.begin(), records.end(), [](const GameplayRecord &a, const GameplayRecord &b) {
        return a.step < b.step;
    });
    size_t n = records.size();
    if(n > 5) n = 5;
    for(size_t k = 0; k < n; ++k) {
        scoreBoard->removeRow(k);
        scoreBoard->insertRow(k);
        scoreBoard->setItem(k, 0, new QTableWidgetItem(records[k].name));
        scoreBoard->setItem(k, 1, new QTableWidgetItem(records[k].date));
        scoreBoard->setItem(k, 2, new QTableWidgetItem(QString(std::to_string(records[k].time_elapsed).c_str())));
        scoreBoard->setItem(k, 3, new QTableWidgetItem(QString(std::to_string(records[k].step).c_str())));
    }
    scoreBoard->setGeometry(40, 40, 500, 200);
    scoreBoard->show();
}
void StateController::ShowLeastTimeElapsed() {
    std::sort(records.begin(), records.end(), [](const GameplayRecord &a, const GameplayRecord &b) {
        return a.time_elapsed < b.time_elapsed;
    });
    size_t n = records.size();
    if(n > 5) n = 5;

    for(size_t k = 0; k < n; ++k) {
        scoreBoard->removeRow(k);
        scoreBoard->insertRow(k);
        scoreBoard->setItem(k, 0, new QTableWidgetItem(records[k].name));
        scoreBoard->setItem(k, 1, new QTableWidgetItem(records[k].date));
        scoreBoard->setItem(k, 2, new QTableWidgetItem(QString(std::to_string(records[k].time_elapsed).c_str())));
        scoreBoard->setItem(k, 3, new QTableWidgetItem(QString(std::to_string(records[k].step).c_str())));
    }
    scoreBoard->setGeometry(40, 40, 500, 200);
    scoreBoard->show();
}


void StateController::ScoreboardToHome() {
    scoreBoard->deleteLater();
    scoreBoard = nullptr;
    records.clear();
    trigger_sortByStep->deleteLater();
    QObject::disconnect(trigger_sortByStep, SIGNAL(clicked(bool)), this, SLOT(ShowLeastSteps()));
    trigger_sortByStep = nullptr;
    trigger_sortByTime->deleteLater();
    QObject::disconnect(trigger_sortByTime, SIGNAL(clicked(bool)), this, SLOT(ShowLeastTimeElapsed()));
    trigger_sortByTime = nullptr;
    scoreBoardreturnToHome->deleteLater();
    QObject::disconnect(scoreBoardreturnToHome, SIGNAL(clicked(bool)), this, SLOT(ScoreboardToHome()));
    scoreBoardreturnToHome = nullptr;
    currState = State::Home;
    LoadHomeScreen();
}

