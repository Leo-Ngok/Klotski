#include "configcontroller.h"


ConfigController::ConfigController(QObject *parent)
    : QObject{parent}
{

}

void ConfigController::Load() {
    quitButton = new QPushButton(dynamic_cast<QWidget *>(parent()));
    QObject::connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(Leave()));
    quitButton->setText("返回");
    quitButton->setGeometry(310, 400, 90, 40);
    quitButton->show();

    saveButton = new QPushButton(dynamic_cast<QWidget *>(parent()));
    QObject::connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(Save()));
    saveButton->setText("保存");
    saveButton->setGeometry(210, 400, 90, 40);
    saveButton->show();

    ranking_label = new QLabel(dynamic_cast<QWidget *>(parent()));
    ranking_label->setText("排行榜文件位置");
    ranking_label->setGeometry(10, 10, 100, 40);
    ranking_label->show();

    record_label = new QLabel(dynamic_cast<QWidget *>(parent()));
    record_label->setText("回放文件夹位置");
    record_label->setGeometry(10, 60, 100, 40);
    record_label->show();


    randomInit = new QCheckBox(dynamic_cast<QWidget *>(parent()));
    randomInit->setText("随机初始化棋盘");
    randomInit->setGeometry(10, 110, 100, 40);
    randomInit->show();

    init_doc_label = new QLabel(dynamic_cast<QWidget *>(parent()));
    init_doc_label->setText("棋盘初始化文件位置");
    init_doc_label->setGeometry(10, 160, 120, 40);
    init_doc_label->show();

    init_folder_label = new QLabel(dynamic_cast<QWidget *>(parent()));
    init_folder_label->setText("棋盘初始化文件夹位置");
    init_folder_label->setGeometry(10, 210, 120, 40);
    init_folder_label->show();

    ranking_file = new QLineEdit(dynamic_cast<QWidget *>(parent()));
    ranking_file->setGeometry(140, 20, 400, 20);
    ranking_file->show();

    record_path = new QLineEdit(dynamic_cast<QWidget *>(parent()));
    record_path->setGeometry(140, 70, 400, 20);
    record_path->show();

    init_file = new QLineEdit(dynamic_cast<QWidget *>(parent()));
    init_file->setGeometry(140, 170, 400, 20);
    init_file->show();

    init_path = new QLineEdit(dynamic_cast<QWidget *>(parent()));
    init_path->setGeometry(140, 220, 400, 20);
    init_path->show();

    play_bgm = new QCheckBox(dynamic_cast<QWidget *>(parent()));
    play_bgm->setText("播放背景音乐");
    play_bgm->setGeometry(10, 260, 100, 40);
    play_bgm->show();

    sound_effects = new QCheckBox(dynamic_cast<QWidget *>(parent()));
    sound_effects->setText("走子音效");
    sound_effects->setGeometry(10, 310, 100, 40);
    sound_effects->show();

    QFile *cfg_json = new QFile("Config.json", parent());
    if(!cfg_json->exists()){
        QMessageBox msgBox;
        msgBox.setText("错误：配置文件缺失，请联系游戏开发者补发");
        Leave();
        return;
    }
    cfg_json->open(QFile::ReadOnly);
    QJsonDocument cfgdoc = QJsonDocument::fromJson(cfg_json->readAll());
    cfg_json->close();
    cfg_json->deleteLater();
    cfg_json = nullptr;
    QJsonObject cfg_obj = cfgdoc.object();

    ranking_file->setText(cfg_obj["rankings_file"].toString());
    record_path->setText(cfg_obj["record_dir"].toString());
    init_file->setText(cfg_obj["init_chess_file"].toString());
    init_path->setText(cfg_obj["init_chess_dir"].toString());
    randomInit->setChecked(cfg_obj["random_init"].toBool());
    play_bgm->setChecked(cfg_obj["play_bgm"].toBool());
    sound_effects->setChecked(cfg_obj["sound_effect"].toBool());

    OnRndInitToggled();
    QObject::connect(randomInit, SIGNAL(toggled(bool)), this, SLOT(OnRndInitToggled()));
}

void ConfigController::OnRndInitToggled(){
    init_file->setEnabled(!randomInit->isChecked());
    init_path->setEnabled(randomInit->isChecked());
}

void ConfigController::Unload() {
    QObject::disconnect(quitButton, SIGNAL(clicked(bool)), this, SLOT(Leave()));
    QObject::disconnect(saveButton, SIGNAL(clicked(bool)), this, SLOT(Save()));
    QObject::disconnect(randomInit, SIGNAL(toggled(bool)), this, SLOT(OnRndInitToggled()));

    quitButton->deleteLater();
    quitButton = nullptr;

    saveButton->deleteLater();
    saveButton = nullptr;

    ranking_label->deleteLater();
    ranking_label = nullptr;

    record_label->deleteLater();
    record_label = nullptr;

    init_doc_label->deleteLater();
    init_doc_label = nullptr;

    init_folder_label->deleteLater();
    init_folder_label = nullptr;

    ranking_file->deleteLater();
    ranking_file = nullptr;

    record_path->deleteLater();
    record_path = nullptr;

    init_file->deleteLater();
    init_file = nullptr;

    init_path->deleteLater();
    init_path = nullptr;

    randomInit->deleteLater();
    randomInit = nullptr;

    play_bgm->deleteLater();
    play_bgm = nullptr;

    sound_effects->deleteLater();
    sound_effects = nullptr;
}

void ConfigController::Leave() {
    Unload();
    emit ReturnHome();
}

void ConfigController::Save() {
    QJsonObject cfg_write;
    cfg_write["rankings_file"] = ranking_file->text();
    cfg_write["record_dir"] = record_path->text();
    cfg_write["init_chess_file"] = init_file->text();
    cfg_write["init_chess_dir"] = init_path->text();

    cfg_write["random_init"] = randomInit->isChecked();
    cfg_write["play_bgm"] = play_bgm->isChecked();
    cfg_write["sound_effect"] = sound_effects->isChecked();
    QFile *cfg_json = new QFile("Config.json", parent());
    cfg_json->open(QFile::WriteOnly);

    QJsonDocument cfgdoc;
    cfgdoc.setObject(cfg_write);
    cfg_json->write(cfgdoc.toJson());
    cfg_json->close();
    cfg_json->deleteLater();
    cfg_json = nullptr;
    Leave();
}
