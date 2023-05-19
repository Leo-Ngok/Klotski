#include "gamecontroller.h"
#include <iostream>
GameController::GameController(QObject *parent)
    : QObject{parent}
{
    this->chessBoard = nullptr;
    this->cp = nullptr;
    sel_chess = nullptr;
    counter_disp = nullptr;
}


void GameController::ConstructChessboard() {
    chessBoard = new ChessBoard(dynamic_cast<QWidget*>(parent()));
    chessBoard->setGeometry(10, 10, 1000, 1000);
    chessBoard->show();
}

void GameController::LoadInitFile(const char* filename) {
    chessboardinitconfig = new QFile(filename);
    if(!chessboardinitconfig->open(QFile::ReadOnly)) {
        qDebug() << QDir::currentPath();
        std::cerr << chessboardinitconfig->errorString().toStdString() << std::endl;
    }

}
void GameController::ConstructChesspieces() {
    sel_chess = nullptr;
    QJsonDocument jsdoc = QJsonDocument::fromJson(chessboardinitconfig->readAll());
    chessboardinitconfig->close();
    chessboardinitconfig->deleteLater();
    chessboardinitconfig = nullptr;
    QJsonObject json = jsdoc.object();
    if(json.contains("chesspieces") && json["chesspieces"].isArray()) {
        QJsonArray chess_list = json["chesspieces"].toArray();
        size_t n_chesspiece = chess_list.count();
        cp = new Chesspiece*[n_chesspiece];
        chess_count = n_chesspiece;
        for(size_t k = 0; k < n_chesspiece; ++k) {
            auto chess_cfg = chess_list[k].toObject();
            int x = -1;
            int y = -1;
            ChessType type = ChessType::INVALID;
            if(chess_cfg.contains("pos") && chess_cfg["pos"].isArray()) {
                QJsonArray pos_list = chess_cfg["pos"].toArray();
                x = pos_list[0].toInt();
                y = pos_list[1].toInt();
            } else {
                qDebug() << "File Error: must contain coordinates for chess initializatons\n";
            }
            if(chess_cfg.contains("name") && chess_cfg["name"].isString()) {
                type = strtotype.at(chess_cfg["name"].toString());
            }
            cp[k] = new Chesspiece(chessBoard);
            cp[k]->setType(type);
            AdjustChessList(cp[k], x, y);
            cp[k]->setCoordinate(x, y);
            cp[k]->show();
        }
    }
}
void GameController::ConstructUI() {

    flash_timer = new QTimer(this);
    flash_timer->setInterval(500);
    QObject::connect(flash_timer, SIGNAL(timeout()), this, SLOT(OnFlashElapsed()));

    stopwatch = new QTimer(this);
    stopwatch->setInterval(1000);
    QObject::connect(stopwatch, SIGNAL(timeout()), this, SLOT(OnStopwatchElapsed()));

    time_caption = new QLabel(dynamic_cast<QWidget*>(parent()));
    time_caption->setGeometry(550, 60, 120, 40);
    time_caption->setText("用时");
    time_caption->show();


    counter_disp = new QLCDNumber(dynamic_cast<QWidget*>(parent()));
    counter_disp->setGeometry(550, 90, 120, 90);
    counter_disp->setFont(QFont("MingLiU", 40, -1, false));
    counter_disp->setDigitCount(4);
    counter_disp->display(0);
    counter_disp->setSegmentStyle(QLCDNumber::Flat);
    counter_disp->setFrameShape(QFrame::NoFrame);
    counter_disp->show();




    step_caption = new QLabel(dynamic_cast<QWidget*>(parent()));
    step_caption->setGeometry(550, 220, 120, 40);
    step_caption->setText("走子次数");
    step_caption->show();


    step_disp = new QLCDNumber(dynamic_cast<QWidget*>(parent()));
    step_disp->setGeometry(550, 280, 120, 90);
    step_disp->setFont(QFont("MingLiU", 40, -1, false));
    step_disp->setDigitCount(4);
    step_disp->display(0);
    step_disp->setSegmentStyle(QLCDNumber::Flat);
    step_disp->setFrameShape(QFrame::NoFrame);
    step_disp->show();

    quitButton = new QPushButton(dynamic_cast<QWidget*>(parent()));
    quitButton->setGeometry(550, 600, 100, 60);
    quitButton->setText("退出");
    quitButton->show();

    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(Reset()));

    dynamic_cast<QWidget*>(parent())->setGeometry(100,100, 1024, 768);
}
void GameController::LoadGame() {

    ConstructChessboard();
    LoadInitFile("D:/github/THU_PASS/year2/temp/yoyo/Klotski/Klotski/Assets/Initial_configurations/default.json");
    ConstructChesspieces();
    ConstructUI();
    stopwatch->start();
}

void GameController::OnMouseClick(QMouseEvent *e) {
    if(e->button() != Qt::LeftButton)
        return;
    auto pos = e->pos();
    auto coord_x = (pos.x() - TOP_LEFT_X) / EDGE_LEN;
    auto coord_y = (pos.y() - TOP_LEFT_Y) / EDGE_LEN;
    if(TOP_LEFT_X <= pos.x() && coord_x < DIM_X &&
      TOP_LEFT_Y <= pos.y() && coord_y < DIM_Y) {
        qDebug() << coord_x << ", " << coord_y;
        if(curr_piece[coord_x][coord_y] != nullptr) {
            if(sel_chess == curr_piece[coord_x][coord_y]) {
                flash_timer->stop();
                sel_chess->setVisible(true);
                sel_chess = nullptr;
            } else {
                if(sel_chess != nullptr) {
                    sel_chess->setVisible(true);
                }
                sel_chess = curr_piece[coord_x][coord_y];
                flash_timer->start();
            }
        } else {
            if(sel_chess != nullptr) {
                MoveType nbr = getMoveType(coord_x, coord_y);
                if(nbr != MoveType::INVALID) {
                    qDebug() << "Adjacent!";
                    if(AdjustByType(sel_chess, nbr)) {
                        qDebug() << "Chess moved";
                        flash_timer->stop();
                        sel_chess->setVisible(true);
                        if(sel_chess->type == ChessType::CAO_CAO &&
                           sel_chess->abstract_x == 1 &&
                           sel_chess->abstract_y == 3) {
                            GameOver();
                        }
                        sel_chess = nullptr;
                    } else {
                        QMessageBox msgBox;
                        msgBox.setText("走子无效: 需要移除障碍物");
                        msgBox.exec();

                        //qDebug() << "Invalid move: blocked by other chesspieces";
                    }
                } else {
                    QMessageBox msgBox;
                    msgBox.setText("走子无效: 目标位置不相邻");
                    msgBox.exec();
                }
            }
        }
    } else {
        if(sel_chess != nullptr) {
            flash_timer->stop();
            sel_chess->setVisible(true);
            sel_chess = nullptr;
        }
    }

}

void GameController::OnFlashElapsed() {
    if(sel_chess != nullptr) {
        bool visible = sel_chess->isVisible();
        sel_chess->setVisible(!visible);
    }
}

bool GameController::AdjustChessList(Chesspiece *piece, int x, int y) {
    int abstract_x = piece->abstract_x;
    int abstract_y = piece->abstract_y;
    switch(piece->type) {
    case ChessType::CAO_CAO:
        if(abstract_x != -1) {
            curr_piece[abstract_x][abstract_y] = nullptr;
            curr_piece[abstract_x + 1][abstract_y + 1] = nullptr;
            curr_piece[abstract_x + 0][abstract_y + 1] = nullptr;
            curr_piece[abstract_x + 1][abstract_y + 0] = nullptr;
        }
        if(curr_piece[x][y]!= nullptr ||
           curr_piece[x + 1][y + 1] != nullptr ||
           curr_piece[x + 0][y + 1] != nullptr ||
           curr_piece[x + 1][y + 0] != nullptr ) return false;

        curr_piece[x][y] = piece;
        curr_piece[x + 1][y + 1] = piece;
        curr_piece[x + 0][y + 1] = piece;
        curr_piece[x + 1][y + 0] = piece;
        break;
    case ChessType::GUAN_YU:

        if(abstract_x != -1){
            curr_piece[abstract_x][abstract_y] = nullptr;
            curr_piece[abstract_x + 1][abstract_y + 0] = nullptr;
        }
        if(curr_piece[x][y]!= nullptr||curr_piece[x + 1][y + 0] != nullptr ) return false;

        curr_piece[x][y] = piece;
        curr_piece[x + 1][y] = piece; break;
    case ChessType::HUANG_ZHONG:
    case ChessType::MA_CHAO:
    case ChessType::ZHANG_FEI:
    case ChessType::ZHAO_YUN:
        if(abstract_x != -1){
            curr_piece[abstract_x][abstract_y] = nullptr;
            curr_piece[abstract_x + 0][abstract_y + 1] = nullptr;
        }
        if(curr_piece[x][y]!= nullptr||curr_piece[x][y + 1] != nullptr ) return false;

        curr_piece[x][y] = piece;
        curr_piece[x][y + 1] = piece; break;
    case ChessType::SOLDIER:
        if(abstract_x != -1)
            curr_piece[abstract_x][abstract_y] = nullptr;
        if(curr_piece[x][y]!= nullptr) return false;
        curr_piece[x][y] = piece;
        break;
    default: break;
    }
    return true;
}

bool GameController::AdjustByType(Chesspiece *piece, MoveType mvt) {
    int x = piece->abstract_x;
    int y = piece->abstract_y;
    switch(mvt) {
    case MoveType::UP:  y--;break;
    case MoveType::DOWN:y++;break;
    case MoveType::LEFT:x--;break;
    case MoveType::RIGHT:x++;break;
     default: assert(false);break;
    }
    bool res = AdjustChessList(piece, x, y);
    if(res) {
        piece->setCoordinate(x, y);
        step_disp->display(++step_count);
    } else {
        AdjustChessList(piece, piece->abstract_x, piece->abstract_y);
    }
    return res;
}
MoveType GameController::getMoveType(int x, int y) {
    bool valid = false;
    int chess_x = sel_chess->abstract_x;
    int chess_y = sel_chess->abstract_y;
    switch(sel_chess->type) {
    case ChessType::CAO_CAO:
        valid |= (x + 0 == chess_x && y + 1 == chess_y); // top
        valid |= (x - 1 == chess_x && y + 1 == chess_y); // top
        if(valid) {
            return MoveType::UP;
        }
        valid |= (x + 1 == chess_x && y + 0 == chess_y); // left
        valid |= (x + 1 == chess_x && y - 1 == chess_y); // left
        if(valid) {
            return MoveType::LEFT;
        }
        valid |= (x + 0 == chess_x && y - 2 == chess_y); // bottom
        valid |= (x - 1 == chess_x && y - 2 == chess_y); // bottom
        if(valid) {
            return MoveType::DOWN;
        }
        valid |= (x - 2 == chess_x && y + 0 == chess_y); // right
        valid |= (x - 2 == chess_x && y - 1 == chess_y); // right
        if(valid) {
            return MoveType::RIGHT;
        }
        break;
    case ChessType::GUAN_YU:
        valid |= (x + 0 == chess_x && y + 1 == chess_y); // top
        valid |= (x - 1 == chess_x && y + 1 == chess_y); // top
        if(valid) {
            return MoveType::UP;
        }
        valid |= (x + 1 == chess_x && y + 0 == chess_y); // left
        if(valid) {
            return MoveType::LEFT;
        }
        valid |= (x + 0 == chess_x && y - 1 == chess_y); // bottom

        valid |= (x - 1 == chess_x && y - 1 == chess_y); // bottom
        if(valid) {
            return MoveType::DOWN;
        }
        valid |= (x - 2 == chess_x && y + 0 == chess_y); // right
        if(valid) {
            return MoveType::RIGHT;
        }
        break;
    case ChessType::HUANG_ZHONG:
    case ChessType::MA_CHAO:
    case ChessType::ZHANG_FEI:
    case ChessType::ZHAO_YUN:
        valid |= (x + 0 == chess_x && y + 1 == chess_y); // top
        if(valid) {
            return MoveType::UP;
        }
        valid |= (x + 1 == chess_x && y + 0 == chess_y); // left
        valid |= (x + 1 == chess_x && y - 1 == chess_y); // left
        if(valid) {
            return MoveType::LEFT;
        }
        valid |= (x + 0 == chess_x && y - 2 == chess_y); // bottom
        if(valid) {
            return MoveType::DOWN;
        }
        valid |= (x - 1 == chess_x && y + 0 == chess_y); // right
        valid |= (x - 1 == chess_x && y - 1 == chess_y); // right
        if(valid) {
            return MoveType::RIGHT;
        }
        break;
    case ChessType::SOLDIER:
        valid |= (x + 0 == chess_x && y + 1 == chess_y); // top
        if(valid) {
            return MoveType::UP;
        }
        valid |= (x + 1 == chess_x && y + 0 == chess_y); // left
        if(valid) {
            return MoveType::LEFT;
        }
        valid |= (x + 0 == chess_x && y - 1 == chess_y); // bottom
        if(valid) {
            return MoveType::DOWN;
        }
        valid |= (x - 1 == chess_x && y + 0 == chess_y); // right
        if(valid) {
            return MoveType::RIGHT;
        }
        break;
    default: assert(false);
    }

    return MoveType::INVALID;
}


void GameController::Reset() {
    for(size_t k = 0; k < chess_count; ++k) {
        this->cp[k]->deleteLater();
    }
    QObject::disconnect(quitButton, SIGNAL(clicked()), this, SLOT(Reset()));
    QObject::disconnect(stopwatch, SIGNAL(timeout()), this, SLOT(OnStopwatchElapsed()));
    QObject::disconnect(flash_timer, SIGNAL(timeout()), this, SLOT(OnFlashElapsed()));
    chessBoard->deleteLater();
    counter_disp->deleteLater();
    step_disp->deleteLater();
    step_caption->deleteLater();
    time_caption->deleteLater();
    stopwatch->deleteLater();
    flash_timer->deleteLater();
    quitButton->deleteLater();
    sel_chess = nullptr;
    stopwatch = nullptr;
    flash_timer = nullptr;
    chessBoard = nullptr;
    counter_disp = nullptr;
    curr_time_elapsed = 0;
    step_count = 0;
    emit BackToHomeScreen();
}


void GameController::OnStopwatchElapsed() {
    counter_disp->display(++this->curr_time_elapsed);
}

void GameController::GameOver() {
    stopwatch->stop();
    QMessageBox msgBox;
    msgBox.setText("完成！");
    msgBox.exec();
    bool ok =  false;

    QString name;
    while(!ok)
    name = QInputDialog::getText(
                dynamic_cast<QWidget *>(parent()), tr("游戏记录"),
                tr("请输入用户名，以便记录成绩"), QLineEdit::Normal,
                QDir::home().dirName(), &ok);

    emit GameEnded(name);
}
