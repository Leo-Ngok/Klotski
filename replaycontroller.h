#ifndef REPLAYCONTROLLER_H
#define REPLAYCONTROLLER_H

#include "chessboard.h"
#include "chesspiece.h"
#include <QObject>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QLCDNumber>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

#define CHESS_MOVE_SOUND_FILE "qrc:/sound_effects/Assets/sound_effects/move.mp3"
class ReplayController : public QObject
{
    Q_OBJECT
public:
    explicit ReplayController(QObject *parent = nullptr);
    void Load();
signals:
    void BackToHome();
private slots:
    void Leave();
    void PrevFrame();
    void NextFrame();

private:
    void Unload();

    void DrawChessboard();
    void ConstructUI();
    void LoadFrameAt(int i);
    ChessBoard* chessBoard      = nullptr;
    Chesspiece **chesspieces = nullptr;
    int curr_pieces_count = 0;
    QPushButton *prev = nullptr;
    QPushButton *next = nullptr;
    QPushButton *quit = nullptr;

    QLabel *time_elapsed_hint = nullptr;
    QLabel *steps_hint = nullptr;

    QLCDNumber *time_elapsed = nullptr;
    QLCDNumber *steps = nullptr;

    QJsonArray records;
    int currFrame = 0;
};

#endif // REPLAYCONTROLLER_H
