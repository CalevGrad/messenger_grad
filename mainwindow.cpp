#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define COUNT_MESSAGES "20"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    downloader = new Downloader;
    new_dialog_window = new NewDialogWindow;
    autorize_widget = new AutorizeWidget;
    correspondence_widget = new CorrespondenceWidget;

    status_bar_text = new QLabel;

    ui->statusBar->addWidget(status_bar_text);

    connect(autorize_widget, &AutorizeWidget::enter_button_clicked, this,
            &MainWindow::autorize_enter_button_clicked);
    connect(autorize_widget, &AutorizeWidget::registration_button_clicked, this,
            &MainWindow::autorize_registration_button_clicked);
    connect(correspondence_widget, &CorrespondenceWidget::send_message_button_clicked, this,
            &MainWindow::send_message_button_clicked);
    connect(correspondence_widget, &CorrespondenceWidget::create_dialog_button_clicked, this,
            &MainWindow::create_dialog_button_clicked);

    connect(correspondence_widget, SIGNAL(dialog_clicked(int)), this, SLOT(dialog_clicked(int)));

    connect(downloader, SIGNAL(onError(QString, QString, QNetworkReply::NetworkError)), this,
            SLOT(erorRequest(QString, QString, QNetworkReply::NetworkError)));
    connect(downloader, SIGNAL(onReady(QString, QByteArray)), this,
            SLOT(resultShow(QString, QByteArray)));

    setCSS();

    ui->verticalLayout->addWidget(autorize_widget);
    ui->verticalLayout->addWidget(correspondence_widget);

    autorize_widget_show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::autorize_widget_show()
{
    status = '1';

    autorize_widget->show();
    correspondence_widget->hide();
    ui->menuBar->hide();

    correspondence_widget->messagesClean();
    correspondence_widget->dialogsClean();
}

void MainWindow::correspondence_widget_show()
{
    status = '2';

    correspondence_widget->setStat(client.id, client.login);

    autorize_widget->hide();
    ui->menuBar->show();
    correspondence_widget->show();
}

void MainWindow::setCSS()
{
    QFile styleF;

    styleF.setFileName("..\\Client_v2\\style.css");
    styleF.open(QFile::ReadOnly);
    QString css_str = styleF.readAll();

    this->setStyleSheet(css_str);
    styleF.close();
}

void MainWindow::longpolling()
{
    // longpolling с сервером

    QString data = correspondence_widget->getData();

    QMap<QString, QString> buf;

    buf.insert("data", data);

    downloader->getData(QString("http://127.0.0.1:8000/users/") + client.id + "/longpolling/",
                        (client.login + ':' + client.password).toLocal8Bit(), buf);
}

void MainWindow::erorRequest(QString url, QString error_str, QNetworkReply::NetworkError error)
{
    // Функция, совершающая действия при ошибках соединения с сервером, отказе доступа и пр.

    qDebug() << "ERROR";
    qDebug() << error_str << endl << error;
    status_bar_text->setText(error_str);

    autorize_widget->setEnableLineEdit(true);
}

void MainWindow::resultShow(QString url, QByteArray data)
{
    // Функция, обрабатывающая ответ сервера

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject root = document.object();
    QJsonArray val = root.value("results").toArray();
    QRegExp dia(".*/dialogs/\\d?/?$"),
            mes(".*/dialogs/\\d?/messages/\\??.*"),
            lon(".*/users/\\d/longpolling/\\??.*"),
            che(".*/users/check/\\??.*");

    if (mes.indexIn(url) != -1) {
        Message *message;
        QJsonObject obj;

        for (int i = 0; i < val.size(); i++) {
            message = new Message;
            obj = val.at(i).toObject();

            message->id = QString(QString::number(obj.value("id").toInt()));
            message->dialog_id = QString(QString::number(obj.value("dialogID").toInt()));
            message->owner_id = QString(QString::number(obj.value("ownerID").toInt()));
            message->text = QString(QByteArray::fromBase64(obj.value("text").toString().toStdString().c_str()));
            message->date = QString(obj.value("date").toString());

            if (message->dialog_id == correspondence_widget->idSelectedDialog() &&
                status == '2')
                correspondence_widget->addMessage(client.id, message);
        }
    }
    else if (dia.indexIn(url) != -1 || lon.indexIn(url) != -1) {
        Dialog *dialog;
        QJsonObject obj;
        QJsonArray valOwners;
        QJsonObject objOwner;
        QString buf[2];
        bool flag;

        for (int i = 0; i < val.size(); i++)
        {
            dialog = new Dialog;
            obj = val.at(i).toObject();

            //считываем данные диалога

            dialog->id = QString::number(obj.value("id").toInt());
            dialog->text_last_message = QByteArray::fromBase64(obj.value("textLastMessage")
                                                               .toString().toLocal8Bit());
            dialog->date_last_message = obj.value("dateLastMessage").toString();
            dialog->id_last_message = QString::number(obj.value("idLastMessage").toInt());

            valOwners = obj.value("owners").toArray();

            for (int j = 0; j < valOwners.size(); j++)
            {
                objOwner = valOwners.at(j).toObject();
                buf[0] = objOwner.keys().at(0);
                buf[1] = QString::number(objOwner.value(buf[0]).toInt());
                dialog->owners.append(buf);

                if (buf[1] == client.id)
                    flag = true;
                else dialog->username = buf[0];
            }

            if (status == '2' && flag)
            {
                correspondence_widget->addOrUpdateDialog(dialog);
                QString id_sel_dia = correspondence_widget->idSelectedDialog();
                if (dialog->id == id_sel_dia)
                {
                    QMap<QString, QString> buf;
                    buf.insert("messageID", correspondence_widget->
                               idLastMessage(correspondence_widget->dialogsCurrentRow()));
                    buf.insert("countMessages", "1");
                    buf.insert("indent", "0");

                    downloader->getData("http://127.0.0.1:8000/dialogs/" +
                                        id_sel_dia + "/messages/",
                                        (client.login + ':' + client.password).toLocal8Bit(), buf);
                }
            }
        }
    }
    else if (che.indexIn(url) != -1) {
        QJsonObject obj = val.at(0).toObject();

        client.id = QString::number(obj.value("id").toInt());
        client.login = autorize_widget->getLogin();
        client.password = autorize_widget->getPassword();

        autorize_widget->setEnableLineEdit(true);

        correspondence_widget_show();
    }

    //если данные разделены на страницы, то получить остальные страницы

    if (root.value("next").isString())
        downloader->getData(root.value("next").toString(),
                            (client.login + ':' + client.password).toLocal8Bit(),
                            QMap<QString, QString>());

    if (lon.indexIn(url) != -1 || che.indexIn(url) != -1)
        longpolling();
}

void MainWindow::dialog_clicked(int ind)
{
    if (correspondence_widget->isDialogUpdate())
        return;

    qDebug() << correspondence_widget->idSelectedDialog();

    if (id_selected_dialog != correspondence_widget->idSelectedDialog()) {
        correspondence_widget->messagesClean();
        QMap<QString, QString> buf;
        buf.insert("messageID", correspondence_widget->idLastMessage(ind));
        buf.insert("countMessages", COUNT_MESSAGES);

        downloader->getData("http://127.0.0.1:8000/dialogs/" + correspondence_widget->idSelectedDialog() +
                            "/messages/", (client.login + ':' + client.password).toLocal8Bit(), buf);

        id_selected_dialog = correspondence_widget->idSelectedDialog();
    }
}

void MainWindow::send_message_button_clicked()
{
    //autorize_window();
    QString id = correspondence_widget->idSelectedDialog();

    if (id.toInt() >= 0) {
        QMap<QString, QString> buf1;

        buf1.insert("dialogID", id);
//        buf1.insert("text", correspondence_widget->getSendMessageTE().toLocal8Bit().toBase64());
        buf1.insert("text", QByteArray(correspondence_widget->getSendMessageTE().toStdString().c_str()).toBase64());

        downloader->postData("http://127.0.0.1:8000/messages/",
                             (client.login + ':' + client.password).toLocal8Bit(),
                             QMap<QString, QString>(), buf1);

        correspondence_widget->sendTEClear();
    }
}

void MainWindow::create_dialog_button_clicked()
{
    new_dialog_window->setUserData((client.login + ':' + client.password).toLocal8Bit());
    new_dialog_window->show();
}

void MainWindow::autorize_enter_button_clicked()
{
    autorize_widget->setEnableLineEdit(false);
    downloader->getData(address_server"/users/check/",
                        (autorize_widget->getLogin() + ':' + autorize_widget->getPassword()).toLocal8Bit(),
                        QMap<QString, QString>());
}

void MainWindow::autorize_registration_button_clicked()
{
    QMap<QString, QString> buf1;

    buf1.insert("username", autorize_widget->getLogin());
    buf1.insert("password", autorize_widget->getPassword());

    downloader->postData(address_server"/users/", "", QMap<QString, QString>(), buf1);
}

void MainWindow::on_exit_triggered()
{
    autorize_widget_show();
}
