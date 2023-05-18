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

enum class MoveType {
    INVALID, UP, DOWN, LEFT, RIGHT
};
class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = nullptr);
    void ConstructChessboard();
    void ConstructChesspieces();
    void LoadInitFile(const char*);
    void LoadGame();
    Chesspiece *curr_piece[DIM_X][DIM_Y] = {{0}};
signals:
    void MouseClick(QPoint &);
private slots:
    void OnFlashElapsed();
public slots:
    void OnMouseClick(QMouseEvent *);
private:
    bool AdjustChessList(Chesspiece *piece, int x, int y);
    bool AdjustByType(Chesspiece *piece, MoveType mvt);
    MoveType getMoveType(int, int);
    ChessBoard  *chessBoard;
    Chesspiece  **cp;
    QFile       *chessboardinitconfig;
    QTimer      *flash_timer;
    Chesspiece  *sel_chess; // the chess being chosen

};

#endif // GAMECONTROLLER_H
