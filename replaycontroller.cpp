#include "replaycontroller.h"

ReplayController::ReplayController(QObject *parent)
    : QObject{parent}
{

}

void ReplayController::Load() {
    DrawChessboard();
    ConstructUI();
    QString filename = QFileDialog::getOpenFileName(dynamic_cast<QWidget *>(parent()),
                                                    tr("打开游戏记录"),"","JSON files(*.json)");
    if(filename.isEmpty()) {
        Leave();
        return;
    }
    QFile * file = new QFile(filename, parent());
    file->open(QFile::ReadOnly);
    QJsonDocument record_doc
            = QJsonDocument::fromJson(file->readAll());
    records = record_doc.array();
    LoadFrameAt(0);
}

void ReplayController::DrawChessboard() {
    chessBoard = new ChessBoard(dynamic_cast<QWidget*>(parent()));
    chessBoard->setGeometry(10, 10, 1000, 1000);
    chessBoard->show();
}

void ReplayController::Leave() {
    Unload();
    emit BackToHome();
}

void ReplayController::Unload() {
    QObject::disconnect(prev, SIGNAL(clicked(bool)), this, SLOT(PrevFrame()));
    QObject::disconnect(next, SIGNAL(clicked(bool)), this, SLOT(NextFrame()));
    QObject::disconnect(quit, SIGNAL(clicked(bool)), this, SLOT(Leave()));
    chessBoard->deleteLater();
    chessBoard = nullptr;

    time_elapsed_hint->deleteLater();
    time_elapsed_hint = nullptr;

    steps_hint->deleteLater();
    steps_hint = nullptr;

    time_elapsed->deleteLater();
    time_elapsed = nullptr;

    steps->deleteLater();
    steps = nullptr;

    prev->deleteLater();
    prev = nullptr;

    next->deleteLater();
    next = nullptr;

    quit->deleteLater();
    quit = nullptr;

    currFrame = 0;

    while(!records.empty()) {
        records.pop_back();
    }
    curr_pieces_count = 0;
}

void ReplayController::ConstructUI() {
    time_elapsed_hint = new QLabel(dynamic_cast<QWidget*>(parent()));
    time_elapsed_hint->setGeometry(550, 60, 120, 40);
    time_elapsed_hint->setText("用时");
    time_elapsed_hint->show();

    steps_hint = new QLabel(dynamic_cast<QWidget*>(parent()));
    steps_hint->setGeometry(550, 220, 120, 40);
    steps_hint->setText("走子次数");
    steps_hint->show();

    time_elapsed = new QLCDNumber(dynamic_cast<QWidget*>(parent()));
    time_elapsed->setGeometry(550, 90, 120, 90);
    time_elapsed->setFont(QFont("MingLiU", 40, -1, false));
    time_elapsed->setDigitCount(4);
    time_elapsed->display(0);
    time_elapsed->setSegmentStyle(QLCDNumber::Flat);
    time_elapsed->setFrameShape(QFrame::NoFrame);
    time_elapsed->show();

    steps = new QLCDNumber(dynamic_cast<QWidget*>(parent()));
    steps->setGeometry(550, 280, 120, 90);
    steps->setFont(QFont("MingLiU", 40, -1, false));
    steps->setDigitCount(4);
    steps->display(0);
    steps->setSegmentStyle(QLCDNumber::Flat);
    steps->setFrameShape(QFrame::NoFrame);
    steps->show();

    prev = new QPushButton(dynamic_cast<QWidget*>(parent()));
    QObject::connect(prev, SIGNAL(clicked(bool)), this, SLOT(PrevFrame()));
    prev->setGeometry(550, 400, 100, 60);
    prev->setText("上一步");
    prev->show();

    next = new QPushButton(dynamic_cast<QWidget*>(parent()));
    QObject::connect(next, SIGNAL(clicked(bool)), this, SLOT(NextFrame()));
    next->setGeometry(550, 480, 100, 60);
    next->setText("下一步");
    next->show();

    quit = new QPushButton(dynamic_cast<QWidget*>(parent()));
    QObject::connect(quit, SIGNAL(clicked(bool)), this, SLOT(Leave()));
    quit->setGeometry(550, 600, 100, 60);
    quit->setText("退出");
    quit->show();
    dynamic_cast<QMainWindow *>(parent())->resize(800, 720);
}

void ReplayController::NextFrame() {
    if(currFrame + 1 < records.size()) {
        QMediaPlayer *player = new QMediaPlayer;
        QAudioOutput *aoutput = new QAudioOutput;
        player->setSource(QUrl::fromLocalFile(CHESS_MOVE_SOUND_FILE));
        player->setAudioOutput(aoutput);
        aoutput->setVolume(0.9f);
        player->play();
        LoadFrameAt(currFrame + 1);
        currFrame++;
    }
}

void ReplayController::PrevFrame() {
    if(currFrame - 1 >= 0 ) {
        QMediaPlayer *player = new QMediaPlayer;
        QAudioOutput *aoutput = new QAudioOutput;
        player->setSource(QUrl::fromLocalFile(CHESS_MOVE_SOUND_FILE));
        player->setAudioOutput(aoutput);
        aoutput->setVolume(0.9f);
        player->play();
        LoadFrameAt(currFrame - 1);
        currFrame--;
    }
}

void ReplayController::LoadFrameAt(int i) {
    QJsonObject jsframe = records[i].toObject();
    QJsonArray local_chesspieces = jsframe["chesspieces"].toArray();
    int time_passed = jsframe["time"].toInt();
    int curr_steps = jsframe["count"].toInt();
    time_elapsed->display(time_passed);
    steps->display(curr_steps);
    for(int k = 0; k < curr_pieces_count; ++k) {
        chesspieces[k]->deleteLater();
        chesspieces[k] = nullptr;
    }

    if(curr_pieces_count > 0) {
        delete [] chesspieces;
    }
    chesspieces = nullptr;
    curr_pieces_count = local_chesspieces.size();
    chesspieces = new Chesspiece*[curr_pieces_count];
    for(int i = 0; i < curr_pieces_count; ++i) {
        QJsonObject piece = local_chesspieces[i].toObject();
        ChessType type = (ChessType) piece["type"].toInt();
        int x = piece["x"].toInt();
        int y = piece["y"].toInt();
        chesspieces[i] = new Chesspiece(chessBoard);
        chesspieces[i]->setType(type);
        chesspieces[i]->setCoordinate(x, y);
        chesspieces[i]->show();
    }
}
