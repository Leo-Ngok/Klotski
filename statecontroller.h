#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "gamecontroller.h"
#include "recordhandler.h"
#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
#include <QTableView>
#include <QTableWidget>
enum class State {
    Invalid,
    Home,
    Game,
    Scoreboard,
    Settings,
    End
};

class StateController : public QObject
{
    Q_OBJECT
public:
    explicit StateController(QObject *parent = nullptr);
    void setGameController(GameController *gc);
signals:
    void GameLoad();
    void MouseClicked(QMouseEvent *);
public slots:
    void LoadHomeScreen();
    void LoadGameScreen();
    void LoadScoreBoard();
    void exit();
    void MouseClick(QMouseEvent *);
private:
    void UnloadHomeScreen();
private slots:
    void ShowLeastSteps();
    void ShowLeastTimeElapsed();
    void ScoreboardToHome();
private:
    State currState;
    GameController *gameController;
    QPushButton *playGame;
    QPushButton *quit;
    QPushButton *loadSettings;
    QPushButton *showRankings;
    RecordHandler *rHandler;

    QTableWidget *scoreBoard;
    std::vector<GameplayRecord> records;
    QPushButton *trigger_sortByStep;
    QPushButton *trigger_sortByTime;
    QPushButton *scoreBoardreturnToHome;
};

#endif // STATECONTROLLER_H
