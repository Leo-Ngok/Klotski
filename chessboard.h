#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
// Dimensions and sizes configurations
#define TOP_LEFT_X 40
#define TOP_LEFT_Y 50
#define DIM_X 4
#define DIM_Y 5
#define EDGE_LEN 120

// Background color configurations
#define BG_R 249
#define BG_G 246
#define BG_B 239

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
signals:

};

#endif // CHESSBOARD_H
