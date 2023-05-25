#include "statecontroller.h"
#include "qapplication.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>

StateController::StateController(QObject *parent)
    : QObject{parent}
{
    rHandler = new RecordHandler(parent);
    memset(rHandler->ranking_filename, 0, sizeof(rHandler->ranking_filename)); // "Rankings.json";

    gameController = new GameController(parent);
    QObject::connect(gameController, SIGNAL(BackToHomeScreen()), this, SLOT(LoadHomeScreen()));
    QObject::connect(gameController, SIGNAL(SaveProgress()), rHandler, SLOT(Record()));
    QObject::connect(gameController, SIGNAL(GameEnded(QString)), rHandler, SLOT(SaveGame(QString)));
    QObject::connect(gameController, SIGNAL(CaptureScene()), rHandler, SLOT(HandleCapture()));
    rHandler->setController(gameController);

    scoreboardController = new ScoreboardController(parent, rHandler);
    QObject::connect(scoreboardController, SIGNAL(BackToHomeScreen()), this, SLOT(LoadHomeScreen()));

    cfgController = new ConfigController(parent);
    QObject::connect(cfgController, SIGNAL(ReturnHome()), this, SLOT(LoadHomeScreen()));

    replayController = new ReplayController(parent);
    QObject::connect(replayController, SIGNAL(BackToHome()), this, SLOT(LoadHomeScreen()));
    player = new QMediaPlayer;
    aoutput = new QAudioOutput;
    player->setSource(QUrl::fromLocalFile((BGM_SOUND_FILE)));
    player->setAudioOutput(aoutput);
    aoutput->setVolume(0.1f);
    player->setLoops(QMediaPlayer::Infinite);
}


void StateController::LoadGameScreen() {
    currState = State::Game;
    UnloadHomeScreen();
    gameController->LoadGame();
}

void StateController::LoadScoreBoard() {
    currState = State::Scoreboard;
    UnloadHomeScreen();
    scoreboardController->Load();
}

void StateController::LoadConfigurations() {
    currState =  State::Settings;
    UnloadHomeScreen();
    cfgController->Load();
}

void StateController::LoadReplay() {
    currState = State::Replay;
    UnloadHomeScreen();
    replayController->Load();
}

void StateController::LoadHomeScreen() {
    currState = State::Home;

    playGame     = new QPushButton(dynamic_cast<QWidget *>(parent()));
    quit         = new QPushButton(dynamic_cast<QWidget *>(parent()));
    loadSettings = new QPushButton(dynamic_cast<QWidget *>(parent()));
    showRankings = new QPushButton(dynamic_cast<QWidget *>(parent()));
    replay       = new QPushButton(dynamic_cast<QWidget *>(parent()));
    QObject::connect(playGame,      SIGNAL(clicked(bool)), this, SLOT(LoadGameScreen()));
    QObject::connect(quit,          SIGNAL(clicked(bool)), this, SLOT(exit()));
    QObject::connect(loadSettings,  SIGNAL(clicked(bool)), this, SLOT(LoadConfigurations()));
    QObject::connect(showRankings,  SIGNAL(clicked(bool)), this, SLOT(LoadScoreBoard()));
    QObject::connect(replay,        SIGNAL(clicked(bool)), this ,SLOT(LoadReplay()));
    playGame->      setGeometry(210,10,180,80);
    playGame->      setText("开始游戏");
    playGame->      show();

    replay->        setGeometry(210, 100, 180, 80);
    replay->        setText("回放");
    replay->show();

    quit->          setGeometry(210,190,180,80);
    quit->          setText("退出");
    quit->          show();

    loadSettings->  setGeometry(210,280,180,80);
    loadSettings->  setText("设置");
    loadSettings->  show();

    showRankings->  setGeometry(210,370,180,80);
    showRankings->  setText("排行榜");
    showRankings->  show();

    dynamic_cast<QMainWindow *>(parent())->resize(640, 480);
    dynamic_cast<QMainWindow *>(parent())->setWindowTitle("华容道");
    QFile *cfg_json = new QFile("Config.json", parent());

    cfg_json->open(QFile::ReadOnly);
    QJsonDocument cfgdoc = QJsonDocument::fromJson(cfg_json->readAll());
    cfg_json->close();
    cfg_json->deleteLater();
    cfg_json = nullptr;
    QJsonObject cfg_obj = cfgdoc.object();
    if(!strcmp(rHandler->ranking_filename, "")) {
        strncpy(rHandler->ranking_filename, cfg_obj["rankings_file"].toString().toStdString().c_str(), 256);
    }
    if(cfg_obj["play_bgm"].toBool()) {
        player->play();
    } else {
        player->stop();
    }
}

void StateController::UnloadHomeScreen() {

    QObject::disconnect(playGame,     SIGNAL(clicked(bool)), this, SLOT(LoadGameScreen()));
    QObject::disconnect(quit,         SIGNAL(clicked(bool)), this, SLOT(exit()));
    QObject::disconnect(loadSettings, SIGNAL(clicked(bool)), this, SLOT(LoadConfigurations()));
    QObject::disconnect(showRankings, SIGNAL(clicked(bool)), this, SLOT(LoadScoreBoard()));
    QObject::disconnect(replay,       SIGNAL(clicked(bool)), this ,SLOT(LoadReplay()));

    playGame->deleteLater();
    playGame = nullptr;

    quit->deleteLater();
    quit = nullptr;

    loadSettings->deleteLater();
    loadSettings = nullptr;

    showRankings->deleteLater();
    showRankings = nullptr;

    replay->deleteLater();
    replay = nullptr;
}

void StateController::exit() {
    QApplication::quit();
}


void StateController::MouseClick(QMouseEvent *e) {
    if(currState == State::Game) {
        gameController->OnMouseClick(e);
    }
}




