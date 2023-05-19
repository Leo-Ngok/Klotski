#ifndef RECORDHANDLER_H
#define RECORDHANDLER_H

#include "gamecontroller.h"
#include "chesspiece.h"
#include <QObject>
#include <vector>
#include <QFile>


struct ChessConfig {
    ChessType type;
    int x;
    int y;
};

struct SceneStatus {
    int count;
    int time;
    std::vector<ChessConfig> chesspieces;
};

struct GameplayRecord{
    QString name;
    int step;
    int time_elapsed;
    QString date;
};

class RecordHandler : public QObject
{
    Q_OBJECT
public:
    explicit RecordHandler(QObject *parent = nullptr);
    void setController(GameController *gc);
    //void setRankingFile(QFile *file);
    const char *ranking_filename;
signals:
public slots:
    void Record();
    void SaveGame(QString name);
    void Reset();
private:
    GameController *controller;
    std::vector<SceneStatus> records;
    QFile *ranking_file;
};

#endif // RECORDHANDLER_H
