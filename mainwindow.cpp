#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(replyFinished(QNetworkReply*)));

    //ui->label_Icon->setText("<img src=\"http://openweathermap.org/img/w/01d.png\">");
    ui->label_Icon->setText("<b>Hello</b> <i>Qt!</i> <img src='http://openweathermap.org/img/w/01d.png' width=50 height=50 >");
    // http://openweathermap.org/img/w/02d.png
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString qsURL = ui->lineEdit->text();

    QUrl myNewURL(qsURL);
    if (myNewURL.isValid()){
        qDebug() << "Send get to " << qsURL;
        request.setUrl(myNewURL);
        manager->get(request);
        ui->textBrowser->append("URL: "+qsURL);
    }
}

void MainWindow::replyFinished(QNetworkReply *reply1)
{
    qDebug()<< "Got responce";
    if ( reply1->error() != QNetworkReply::NoError ) {
        qWarning() <<"ErrorNo: "<< reply1->error() << "for url: " << reply1->url().toString();
        qDebug() << "Request failed, " << reply1->errorString();
        qDebug() << "Headers:"<<  reply1->rawHeaderList()<< "content:" << reply1->readAll();
        return;
    }
    qDebug() << reply1->url();
    QList<QByteArray> replyHeaders = reply1->rawHeaderList();
    QString qsHeadersList;
    QByteArray baContent = reply1->readAll();
    QString qsContent = baContent;
    QPair<QByteArray, QByteArray> HeaderPair;
    qDebug() << "Headers list:"<<  reply1->rawHeaderList(); //<< "content:";// << reply1->readAll();
    qDebug() << "Replay Headers list: count " << replyHeaders.size();
    for (int i = 0; i < replyHeaders.size(); ++i) {
        HeaderPair = reply1->rawHeaderPairs().at(i);
        qsHeadersList.append(HeaderPair.first);
        qsHeadersList.append(": ");
        qsHeadersList.append(HeaderPair.second);
        qsHeadersList.append("\n");
        qDebug() << "i is: "<< i;
    }
    qDebug() << qsHeadersList;
    ui->textBrowser->append("Headers: " + qsHeadersList);
    ui->textBrowser->append("Content: " + qsContent);

    if (qsHeadersList.contains("Content-Type: application/json")){
        parseJson(baContent);
    }

}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_pushButton_Comdine_clicked()
{
    QString qsURL;
    qsURL.append(ui->lineEdit_Addres->text());
    qsURL.append("?q=");
    qsURL.append(ui->lineEdit_Sity->text());
    qsURL.append("&appid=");
    qsURL.append(ui->lineEdit_AppId->text());

    QUrl myNewURL(qsURL);
    if (myNewURL.isValid()){
        qDebug() << "Send get to " << qsURL;
        request.setUrl(myNewURL);
        manager->get(request);
        ui->textBrowser->append("URL: "+qsURL);
    }
}

void MainWindow::parseJson(QByteArray baJson)
{
    qDebug() << "parseJson";
    qDebug() << "baJson is " << baJson;

    QJsonParseError myParserError;
    QJsonDocument myJson = QJsonDocument::fromJson(baJson,&myParserError);

    if (myParserError.error != QJsonParseError::NoError ){
        qDebug() << "Error is " << myParserError.errorString();
        return;
    }
    qDebug() << "no error in parseJson";

    if (myJson.isArray())  { qDebug() << "This is array!" ;}
    if (myJson.isObject()) { qDebug() << "This is object!" ;}
    if (myJson.isNull() ) { qDebug() << "This is null!" ;}
    if (myJson.isEmpty()) { qDebug() << "This is empty!" ;}
    QJsonObject myObj = myJson.object();

    qDebug() << "All kays list:";
    qDebug() << myObj.keys();

    QJsonValue value = myObj.value(QString("main"));
    qDebug() << value;

    QJsonObject item = value.toObject();
    qDebug() << "QJsonObject of description: " << item;
    QJsonValue tempKelv = item.value(QString("temp"));
    qDebug() << "tempKelv" << tempKelv;
    double d_tempKelv = tempKelv.toDouble(-100);
    double d_tempCelc = d_tempKelv-273.15;

    //value = myObj.value(QString("sys"));
    //qDebug() << value;
    QJsonValue sityName = myObj.value(QString("name"));
    qDebug() << "sityName" << sityName;
    QString qsName = sityName.toString("Default Sity");

    qDebug() << "temp in " << qsName << d_tempKelv << " *K ";
    qDebug() << "temp in " << qsName << d_tempCelc << " *C ";

    ui->label_Name->setText(qsName);
    ui->label_Temp->setNum(d_tempCelc);


    value = myObj.value(QString("weather"));
    qDebug() << value;

    QJsonArray arr = value.toArray();
    qDebug() << "QJsonArray of description: " << arr;
    QJsonValue iconId = arr[0].toObject().value(QString("icon"));
    qDebug() << "iconId" << iconId;
    QString qsIconId = iconId.toString("Def icon");
    ui->label_Icon->setText(qsIconId);
    // http://openweathermap.org/img/w/04d.png
    QString qsIconUrl = "http://openweathermap.org/img/w/%1.png";
    QUrl iconUrl;
    iconUrl.setUrl(qsIconUrl.arg(qsIconId));
    qDebug() << "iconUrl " << iconUrl.toString();
    ui->label_Icon->setText("<html><img src=\"http://openweathermap.org/img/w/01d.png\" alt="" /> </html>");






}
