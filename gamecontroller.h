#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "chessboard.h"
#include "chesspiece.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMouseEvent>
#include <QTimer>
#include <QDir>
#include <QLCDNumber>
#include <QFont>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QMainWindow>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

#define CHESS_MOVE_SOUND_FILE "qrc:/sound_effects/Assets/sound_effects/move.mp3"

enum class MoveType {
    INVALID, UP, DOWN, LEFT, RIGHT
};

enum class GameMode {
    Invalid, Play, Replay
};

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = nullptr);
    void ConstructChessboard();
    bool ConstructChesspieces(QString filename);
    void ConstructUI();
    size_t      chess_count = 0;
    Chesspiece *curr_piece[DIM_X][DIM_Y] = {{0}};
    Chesspiece  **cp = nullptr;
    int         step_count = 0;
    int         curr_time_elapsed = 0;
signals:
    void MouseClick(QPoint &);
    void BackToHomeScreen();
    void GameEnded(QString name);
    void SaveProgress();
    void CaptureScene();
private slots:
    void OnFlashElapsed();
    void OnStopwatchElapsed();
    void OnBackHomeClicked();
public slots:
    void OnMouseClick(QMouseEvent *);
    void Reset();
    void LoadGame();
    void OnCaptureBtnClick();
private:
    bool AdjustChessList(Chesspiece *piece, int x, int y);
    bool AdjustByType(Chesspiece *piece, MoveType mvt);
    void GameOver();

    MoveType getMoveType(int, int);
    void SaveRecord();
    void SaveGamePlay();

    ChessBoard  *chessBoard     = nullptr;
    QTimer      *flash_timer    = nullptr;

    // the chess being chosen
    Chesspiece  *sel_chess      = nullptr;

    /* Panel on right hand side. */
    QTimer      *stopwatch      = nullptr;

    QLabel      *time_caption   = nullptr;
    QLCDNumber  *counter_disp   = nullptr;

    QLabel      *step_caption   = nullptr;
    QLCDNumber  *step_disp      = nullptr;

    QPushButton *scene_capture  = nullptr;
    QPushButton *quitButton     = nullptr;
    bool enabled = true;
    bool play_chess_move = false;
};

#endif // GAMECONTROLLER_H
