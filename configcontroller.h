#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCheckBox>

class ConfigController : public QObject
{
    Q_OBJECT
public:
    explicit ConfigController(QObject *parent = nullptr);
    void Load();
signals:
    void ReturnHome();
private slots:
    void Leave();
    void Save();
    void OnRndInitToggled();
private:
    void Unload();

    QPushButton *saveButton     = nullptr;
    QPushButton *quitButton     = nullptr;

    QLabel    *ranking_label    = nullptr;
    QLineEdit *ranking_file     = nullptr;

    QLabel    *record_label     = nullptr;
    QLineEdit *record_path      = nullptr;

    QLabel    *init_doc_label   = nullptr;
    QLineEdit *init_file        = nullptr;

    QLabel    *init_folder_label= nullptr;
    QLineEdit *init_path        = nullptr;

    QCheckBox *randomInit       = nullptr;
    QCheckBox *play_bgm         = nullptr;
    QCheckBox *sound_effects    = nullptr;
};

#endif // CONFIGCONTROLLER_H
