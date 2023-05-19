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
    void ConstructChesspieces();
    void ConstructUI();
    void LoadInitFile(const char*);
    size_t      chess_count = 0;
    Chesspiece *curr_piece[DIM_X][DIM_Y] = {{0}};
    Chesspiece  **cp;
    int         step_count = 0;
    int         curr_time_elapsed = 0;
signals:
    void MouseClick(QPoint &);
    void BackToHomeScreen();
    void GameEnded(QString name);
private slots:
    void OnFlashElapsed();
    void OnStopwatchElapsed();
public slots:
    void OnMouseClick(QMouseEvent *);
    void Reset();
    void LoadGame();
private:
    bool AdjustChessList(Chesspiece *piece, int x, int y);
    bool AdjustByType(Chesspiece *piece, MoveType mvt);
    void GameOver();

    MoveType getMoveType(int, int);
    void SaveRecord();
    void SaveGamePlay();

    ChessBoard  *chessBoard;
    QFile       *chessboardinitconfig;
    QTimer      *flash_timer;
    Chesspiece  *sel_chess; // the chess being chosen

    QTimer      *stopwatch;

    QLabel      *time_caption;
    QLCDNumber  *counter_disp;

    QLabel      *step_caption;
    QLCDNumber  *step_disp;
    QPushButton *quitButton;
};

#endif // GAMECONTROLLER_H
