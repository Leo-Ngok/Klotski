#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "configcontroller.h"
#include "gamecontroller.h"
#include "recordhandler.h"
#include "replaycontroller.h"
#include "scoreboardcontroller.h"
#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
#include <QTableView>
#include <QTableWidget>

#define BGM_SOUND_FILE "qrc:/sound_effects/Assets/sound_effects/bgm.mp3"

enum class State {
    Invalid,
    Home,
    Game,
    Scoreboard,
    Settings,
    Replay,
    End
};

class StateController : public QObject
{
    Q_OBJECT
public:
    explicit StateController(QObject *parent = nullptr);
signals:
    //void MouseClicked(QMouseEvent *);
public slots:
    void LoadHomeScreen();
    void LoadGameScreen();
    void LoadScoreBoard();
    void LoadReplay();
    void LoadConfigurations();
    void exit();
    void MouseClick(QMouseEvent *);
private:
    void UnloadHomeScreen();
private:
    State currState;
    GameController *gameController;
    ScoreboardController *scoreboardController;
    ConfigController *cfgController;
    ReplayController *replayController;

    QPushButton *playGame;
    QPushButton *replay;
    QPushButton *quit;
    QPushButton *loadSettings;
    QPushButton *showRankings;
    RecordHandler *rHandler;


    QMediaPlayer* player;
    QAudioOutput *aoutput;
};

#endif // STATECONTROLLER_H
