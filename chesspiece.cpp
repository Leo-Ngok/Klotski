#include "chesspiece.h"
#include <QPainter>
#include <cassert>
Chesspiece::Chesspiece(QWidget *parent)
    : QWidget{parent}, type{ChessType::INVALID}, position{cplx(-1,-1)}
{}

void Chesspiece::setPosition(cplx pos) {
    this->position = pos;
    this->move(pos.real(), pos.imag());
}


void Chesspiece::setCoordinate(int x, int y) {
    int global_x = TOP_LEFT_X + x * EDGE_LEN;
    int global_y = TOP_LEFT_Y + y * EDGE_LEN;
    cplx global_pos(global_x, global_y);
    setPosition(global_pos);

    abstract_x = x;
    abstract_y = y;
}


void Chesspiece::setType(ChessType type) {
    this->type = type;
}

void Chesspiece::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int width_unit = 0;
    int height_unit = 0;
    const char *path;
    switch(this->type) {
    case ChessType::CAO_CAO: {
        path = CAO_CAO_PATH;
        width_unit = 2;
        height_unit = 2;
        break;
    }
    case ChessType::GUAN_YU:{
        path = GUAN_YU_PATH;
        width_unit = 2;
        height_unit = 1;
        break;
    }
    case ChessType::HUANG_ZHONG:{
        path = HUANG_ZHONG_PATH;
        width_unit = 1;
        height_unit = 2;
        break;
    }
    case ChessType::MA_CHAO:{
        path = MA_CHAO_PATH;
        width_unit = 1;
        height_unit = 2;
        break;
    }
    case ChessType::ZHANG_FEI:{
        path = ZHANG_FEI_PATH;
        width_unit = 1;
        height_unit = 2;
        break;
    }
    case ChessType::ZHAO_YUN:{
        path = ZHAO_YUN_PATH;
        width_unit = 1;
        height_unit = 2;
        break;
    }
    case ChessType::SOLDIER:{
        path = SOLDIER_PATH;
        width_unit = 1;
        height_unit = 1;
        break;
    }
    case ChessType::INVALID:
    default:
    {
        assert(false);
        path = "";
        break;
    }
    }
    resize(width_unit * EDGE_LEN, height_unit * EDGE_LEN);
    QPixmap pixmap(path);
    painter.drawPixmap(0, 0,width_unit * EDGE_LEN, height_unit * EDGE_LEN ,pixmap);
}

