#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QWidget>
#include <complex>
#include <map>
#include "chessboard.h"
typedef std::complex<double> cplx;

#define CAO_CAO_PATH     ":/chess_pieces/Assets/Chesspieces_img/CAO_CAO.PNG"
#define GUAN_YU_PATH     ":/chess_pieces/Assets/Chesspieces_img/GUAN_YU.PNG"
#define HUANG_ZHONG_PATH ":/chess_pieces/Assets/Chesspieces_img/HUANG_ZHONG.PNG"
#define MA_CHAO_PATH     ":/chess_pieces/Assets/Chesspieces_img/MA_CHAO.PNG"
#define ZHANG_FEI_PATH   ":/chess_pieces/Assets/Chesspieces_img/ZHANG_FEI.PNG"
#define ZHAO_YUN_PATH    ":/chess_pieces/Assets/Chesspieces_img/ZHAO_YUN.PNG"
#define SOLDIER_PATH     ":/chess_pieces/Assets/Chesspieces_img/SOLDIER.PNG"

enum class ChessType;

class Chesspiece : public QWidget
{
    Q_OBJECT
public:
    explicit Chesspiece(QWidget *parent = nullptr);
    void setPosition(cplx pos);
    void setCoordinate(int x, int y);
    void setType(ChessType type);
    int abstract_x = -1;
    int abstract_y = -1;
    ChessType type;
protected:
    void paintEvent(QPaintEvent *);
signals:
private:
    cplx position;
};
enum class ChessType {
    INVALID,
    CAO_CAO,
    GUAN_YU,
    HUANG_ZHONG,
    MA_CHAO,
    ZHANG_FEI,
    ZHAO_YUN,
    SOLDIER
};
const std::map<QString, ChessType> strtotype = {
    {"CAO_CAO", ChessType::CAO_CAO},
    {"GUAN_YU", ChessType::GUAN_YU},
    {"HUANG_ZHONG", ChessType::HUANG_ZHONG},
    {"MA_CHAO", ChessType::MA_CHAO},
    {"ZHANG_FEI", ChessType::ZHANG_FEI},
    {"ZHAO_YUN", ChessType::ZHAO_YUN},
    {"SOLDIER", ChessType::SOLDIER},
};
#endif // CHESSPIECE_H
