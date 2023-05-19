#include "recordhandler.h"

RecordHandler::RecordHandler(QObject *parent)
    : QObject{parent}
{
    ranking_file = nullptr;
    controller = nullptr;
}

void RecordHandler::Record() {
    size_t n = controller->chess_count;
    struct ChessConfig cfg;
    struct SceneStatus status;
    for(size_t k = 0; k < n; ++k) {
        Chesspiece *piece = controller->cp[k];
        cfg.x = piece->abstract_x;
        cfg.y = piece->abstract_y;
        cfg.type = piece->type;
        status.chesspieces.push_back(cfg);
    }
    status.time = controller->curr_time_elapsed;
    status.count = controller->step_count;
    records.push_back(status);
}

void RecordHandler::Reset() {
    records.clear();
}


void RecordHandler::setController(GameController *gc) {
    controller = gc;
}
//void RecordHandler::setRankingFile(QFile *file) {
//    ranking_file = file;
//}

void RecordHandler::SaveGame(QString name){
    QString ranking_fname = ranking_filename;
    QJsonArray rankings;
    ranking_file = new QFile(ranking_fname, parent());

    if(ranking_file->exists()) {
        ranking_file->open(QFile::ReadOnly);
        QJsonDocument rankingDoc = QJsonDocument::fromJson(ranking_file->readAll());
        ranking_file->close();
        ranking_file->deleteLater();
        ranking_file = nullptr;
        rankings = rankingDoc.array();
    }
    // ranking_file->fileName();
    QJsonObject new_record;
    new_record["name"] = name;
    new_record["time"] = controller->curr_time_elapsed;
    new_record["step"] = controller->step_count;
    rankings.push_back(new_record);
    QJsonDocument output_ranking_doc;
    output_ranking_doc.setArray(rankings);
    ranking_file = new QFile(ranking_fname);
    ranking_file->open(QFile::WriteOnly);
    ranking_file->write(output_ranking_doc.toJson(QJsonDocument::Indented));
    ranking_file->close();
}
