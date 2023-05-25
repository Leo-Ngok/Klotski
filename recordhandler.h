#ifndef RECORDHANDLER_H
#define RECORDHANDLER_H

#include "gamecontroller.h"
#include "chesspiece.h"
#include <QObject>
#include <vector>
#include <QFile>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>


struct ChessConfig {
    ChessType type;
    int x;
    int y;
};

struct SceneStatus {
    int count;
    int time;
    QJsonArray chesspieces;
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
    char ranking_filename[256];
    QString records_dir;
signals:
public slots:
    void Record();
    void SaveGame(QString name);
    void Reset();
    void HandleCapture();

private:
    GameController *controller;
    QJsonArray records;
};

#endif // RECORDHANDLER_H
