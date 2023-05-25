#ifndef SCOREBOARDCONTROLLER_H
#define SCOREBOARDCONTROLLER_H

#include "recordhandler.h"
#include <QTableWidget>
#include <QObject>
#include <QPushButton>

class ScoreboardController : public QObject
{
    Q_OBJECT
public:
    explicit ScoreboardController(QObject *parent = nullptr, RecordHandler *rHandler = nullptr);
public slots:
    void Load();
private slots:
    void ShowLeastSteps();
    void ShowLeastTimeElapsed();
    void ScoreboardToHome();
signals:
    void BackToHomeScreen();
private:
    void Unload();
    QTableWidget *scoreBoard;
    std::vector<GameplayRecord> records;
    QPushButton *trigger_sortByStep;
    QPushButton *trigger_sortByTime;
    QPushButton *scoreBoardreturnToHome;
    RecordHandler *rHandler;
};

#endif // SCOREBOARDCONTROLLER_H
