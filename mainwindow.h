#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply = nullptr;


private slots:
    void on_pushButton_clicked();
    void replyFinished(QNetworkReply *reply);
    void on_lineEdit_returnPressed();
    void on_pushButton_Comdine_clicked();
    void parseJson(QByteArray baJson);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
