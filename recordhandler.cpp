#include "recordhandler.h"
#include <cstdio>
#include <cstring>

RecordHandler::RecordHandler(QObject *parent)
    : QObject{parent}
{
    controller = nullptr;
}

void RecordHandler::Record() {
    size_t n = controller->chess_count;
    QJsonObject cfg;
    QJsonObject status;
    QJsonArray chesspieces;
    for(size_t k = 0; k < n; ++k) {
        Chesspiece *piece = controller->cp[k];
        cfg["x"] = piece->abstract_x;
        cfg["y"] = piece->abstract_y;
        cfg["type"] = (int) piece->type;
        chesspieces.push_back(cfg);
    }
    status["time"] = controller->curr_time_elapsed;
    status["count"] = controller->step_count;
    status["chesspieces"] = chesspieces;
    records.push_back(status);
}

void RecordHandler::Reset() {
    while(!records.empty()) {
        records.pop_back();
    }
}


void RecordHandler::setController(GameController *gc) {
    controller = gc;
}
//void RecordHandler::setRankingFile(QFile *file) {
//    ranking_file = file;
//}

void RecordHandler::SaveGame(QString name){
    QDateTime dt = QDateTime::currentDateTime();
    int yyyy, mm,dd, h, m, s;
    dt.date().getDate(&yyyy, &mm, &dd);
    h = dt.time().hour();
    m = dt.time().minute();
    s = dt.time().second();

    QString ranking_fname = ranking_filename;
    QJsonArray rankings;
    QFile *ranking_file = new QFile(ranking_fname, parent());

    if(ranking_file->exists()) {
        ranking_file->open(QFile::ReadOnly);
        QJsonDocument rankingDoc = QJsonDocument::fromJson(ranking_file->readAll());
        ranking_file->close();
        ranking_file->deleteLater();
        ranking_file = nullptr;
        rankings = rankingDoc.array();
    }
    QJsonObject new_record;
    char date_buffer[32];
    sprintf(date_buffer, "%04d-%02d-%02d %02d:%02d", yyyy, mm, dd, h, m);
    new_record["name"] = name;
    new_record["time"] = controller->curr_time_elapsed;
    new_record["step"] = controller->step_count;
    new_record["date"] = date_buffer;
    rankings.push_back(new_record);
    QJsonDocument output_ranking_doc;
    output_ranking_doc.setArray(rankings);
    ranking_file = new QFile(ranking_fname);
    ranking_file->open(QFile::WriteOnly);
    ranking_file->write(output_ranking_doc.toJson(QJsonDocument::Indented));
    ranking_file->close();
    ranking_file->deleteLater();








    char timemark[32];
    sprintf(timemark, "record_%04d%02d%02d%02d%02d%02d.json", yyyy, mm, dd, h, m, s);
    QFile *cfg_json = new QFile("Config.json", parent());

    cfg_json->open(QFile::ReadOnly);
    QJsonDocument cfgdoc = QJsonDocument::fromJson(cfg_json->readAll());
    cfg_json->close();
    cfg_json->deleteLater();
    cfg_json = nullptr;
    QJsonObject cfg_obj = cfgdoc.object();

    QFile *records_file = new QFile(cfg_obj["record_dir"].toString() + timemark, parent());
    records_file->open(QFile::WriteOnly);
    QJsonDocument record_doc;
    record_doc.setArray(records);
    records_file->write(record_doc.toJson());
    records_file->close();
    records_file->deleteLater();
    records_file = nullptr;
    while(!records.empty()) {
        records.pop_back();
    }
}



void RecordHandler::HandleCapture() {
    QString name = QInputDialog::getText(dynamic_cast<QWidget *>(parent()),
                                         "捕获棋局", "请输入局面名称");
    if(name.isEmpty())
        return;
    QString save_name = QFileDialog::getSaveFileName(
                dynamic_cast<QWidget *>(parent()), "保存捕获棋局", "", "JSON files(*.json)");
    if(save_name.isEmpty())
        return;
    QJsonArray chesspieces = records.last()["chesspieces"].toArray();
    QJsonArray pieces_dispatch;
    QJsonObject dispatch_buffer;
    QJsonArray pos_buffer;
    for(auto && piece : chesspieces) {
        ChessType type = (ChessType) piece.toObject()["type"].toInt();
        int x = piece.toObject()["x"].toInt();
        int y = piece.toObject()["y"].toInt();
        while(!pos_buffer.empty()) pos_buffer.pop_back();
        pos_buffer.push_back(x);
        pos_buffer.push_back(y);
        dispatch_buffer["name"] = typetostr.at(type);
        dispatch_buffer["pos"] = pos_buffer;
        while(!pos_buffer.empty()) pos_buffer.pop_back();
        pieces_dispatch.push_back(dispatch_buffer);
    }
    QJsonObject top_level;
    top_level["name"] = name;
    top_level["chesspieces"] = pieces_dispatch;
    QJsonDocument top_doc;
    top_doc.setObject(top_level);


    QFile *file = new QFile(save_name, parent());
    file->open(QFile::WriteOnly);
    file->write(top_doc.toJson());
    file->close();
    file->deleteLater();
}
