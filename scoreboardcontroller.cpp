#include "scoreboardcontroller.h"

ScoreboardController::ScoreboardController(QObject *parent, RecordHandler *rHandler)
    : QObject{parent}, scoreBoard{nullptr},
      trigger_sortByStep{nullptr}, trigger_sortByTime{nullptr}, scoreBoardreturnToHome{nullptr},
      rHandler{rHandler}
{}

void ScoreboardController::Load() {
    /* ------------------ Step 1: Setup record table --------- */
    QFile *rankings = new QFile(rHandler->ranking_filename);
    QJsonArray ranking_list;
    while(!ranking_list.empty()) {
        ranking_list.pop_back();
    }
    if(rankings->exists()) {
        rankings->open(QFile::ReadOnly);
        QJsonDocument rankingDoc = QJsonDocument
                ::fromJson(rankings->readAll());
        rankings->close();
        rankings->deleteLater();
        rankings = nullptr;
        ranking_list = rankingDoc.array();
    }
    GameplayRecord record;
    for(auto && ranking_item : ranking_list) {
        auto item = ranking_item.toObject();
        if(item.contains("name") && item["name"].isString()) {
            record.name = item["name"].toString();
        } else {
            record.name = tr("unnamed");
        }
        if(item.contains("step") && item["step"].isDouble()) {
            record.step = item["step"].toInt();
        } else {
            record.step = 10000;
        }
        if(item.contains("time") && item["time"].isDouble()) {
            record.time_elapsed = item["time"].toInt();
        } else {
            record.time_elapsed = 10000;
        }
        if(item.contains("date") && item["date"].isString()) {
            record.date = item["date"].toString();
        } else {
            record.date = tr("1970-01-01");
        }
        records.push_back(record);
    }

    /* ================== Step 2: Setup UI Buttons =================== */
    dynamic_cast<QWidget *>(parent())->resize(640, 240);
    dynamic_cast<QMainWindow *>(parent())->setWindowTitle("排行榜");
    trigger_sortByStep = new QPushButton(dynamic_cast<QWidget *>(parent()));
    trigger_sortByTime = new QPushButton(dynamic_cast<QWidget *>(parent()));
    scoreBoardreturnToHome = new QPushButton(dynamic_cast<QWidget *>(parent()));

    trigger_sortByStep->setGeometry(440, 10, 180, 40);
    trigger_sortByStep->setText("按步长排序");
    trigger_sortByStep->show();
    QObject::connect(trigger_sortByStep, SIGNAL(clicked(bool)), this, SLOT(ShowLeastSteps()));


    trigger_sortByTime->setGeometry(440, 70, 180, 40);
    trigger_sortByTime->setText("按用时排序");
    trigger_sortByTime->show();
    QObject::connect(trigger_sortByTime, SIGNAL(clicked(bool)), this, SLOT(ShowLeastTimeElapsed()));

    scoreBoardreturnToHome->setGeometry(440, 130, 180, 40);
    scoreBoardreturnToHome->setText("退出");
    scoreBoardreturnToHome->show();
    QObject::connect(scoreBoardreturnToHome, SIGNAL(clicked(bool)), this, SLOT(ScoreboardToHome()));


    /* =================== Step 3: Setup scoreboard ==================== */
    scoreBoard = new QTableWidget(dynamic_cast<QWidget *>(parent()));

    // Initialize table headers and configurations
    scoreBoard->setColumnCount(4);
    scoreBoard->setSelectionMode(QAbstractItemView::SingleSelection);
    scoreBoard->setSelectionBehavior(QAbstractItemView::SelectRows);
    scoreBoard->setHorizontalHeaderLabels(QStringList() <<
                                          "用户名" << "日期" << "用时" << "步数");

    ShowLeastSteps();
}

void ScoreboardController::Unload() {

    scoreBoard->deleteLater();
    scoreBoard = nullptr;
    records.clear();

    trigger_sortByStep->deleteLater();
    QObject::disconnect(trigger_sortByStep, SIGNAL(clicked(bool)), this, SLOT(ShowLeastSteps()));
    trigger_sortByStep = nullptr;

    trigger_sortByTime->deleteLater();
    QObject::disconnect(trigger_sortByTime, SIGNAL(clicked(bool)), this, SLOT(ShowLeastTimeElapsed()));
    trigger_sortByTime = nullptr;

    scoreBoardreturnToHome->deleteLater();
    QObject::disconnect(scoreBoardreturnToHome, SIGNAL(clicked(bool)), this, SLOT(ScoreboardToHome()));
    scoreBoardreturnToHome = nullptr;
}


void ScoreboardController::ShowLeastSteps(){
    std::sort(records.begin(), records.end(), [](const GameplayRecord &a, const GameplayRecord &b) {
        return a.step < b.step;
    });
    size_t n = records.size();
    if(n > 5) n = 5;
    for(size_t k = 0; k < n; ++k) {
        scoreBoard->removeRow(k);
        scoreBoard->insertRow(k);
        scoreBoard->setItem(k, 0, new QTableWidgetItem(records[k].name));
        scoreBoard->setItem(k, 1, new QTableWidgetItem(records[k].date));
        scoreBoard->setItem(k, 2, new QTableWidgetItem(QString(std::to_string(records[k].time_elapsed).c_str())));
        scoreBoard->setItem(k, 3, new QTableWidgetItem(QString(std::to_string(records[k].step).c_str())));
    }
    scoreBoard->setGeometry(10, 10, 420, 190);
    scoreBoard->show();

}
void ScoreboardController::ShowLeastTimeElapsed(){
    std::sort(records.begin(), records.end(), [](const GameplayRecord &a, const GameplayRecord &b) {
        return a.time_elapsed < b.time_elapsed;
    });
    size_t n = records.size();
    if(n > 5) n = 5;

    for(size_t k = 0; k < n; ++k) {
        scoreBoard->removeRow(k);
        scoreBoard->insertRow(k);
        scoreBoard->setItem(k, 0, new QTableWidgetItem(records[k].name));
        scoreBoard->setItem(k, 1, new QTableWidgetItem(records[k].date));
        scoreBoard->setItem(k, 2, new QTableWidgetItem(QString(std::to_string(records[k].time_elapsed).c_str())));
        scoreBoard->setItem(k, 3, new QTableWidgetItem(QString(std::to_string(records[k].step).c_str())));
    }
    scoreBoard->setGeometry(10, 10, 420, 190);
    scoreBoard->show();

}
void ScoreboardController::ScoreboardToHome(){
    Unload();
    emit BackToHomeScreen();
}
