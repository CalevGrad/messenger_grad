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
    dialogs = new DialogListWidget;
    messages = new MessageListWidget;
    new_dialog_window = new NewDialogWindow;

    left_lay = new QVBoxLayout;
    right_lay = new QVBoxLayout;
    stat = new QLabel;
    status_bar_text = new QLabel;

    send_message_button = new QPushButton;
    create_dialog_button = new QPushButton;
    send_message_lay = new QHBoxLayout;
    send_message_te = new QTextEdit;

    autorize_lay = new QVBoxLayout;
    autorize_le_lay = new QVBoxLayout;
    autorize_label_lay = new QVBoxLayout;
    autorize_enter_lay = new QHBoxLayout;
    autorize_login_le = new QLineEdit;
    autorize_password_le = new QLineEdit;
    autorize_login_lab = new QLabel;
    autorize_password_lab = new QLabel;
    autorize_enter_button = new QPushButton;
    autorize_registration_button = new QPushButton;

    send_message_button->setText("Отправить");
    create_dialog_button->setText("Создать диалог");
    autorize_enter_button->setText("Вход");
    autorize_registration_button->setText("Регистрация");
    autorize_login_lab->setText("Логин: ");
    autorize_password_lab->setText("Пароль: ");

    ui->statusBar->addWidget(status_bar_text);

    messages->setSelectionMode( QAbstractItemView::NoSelection );
    messages->setFocusPolicy(Qt::NoFocus);



    connect(autorize_enter_button, &QPushButton::clicked, this,
            &MainWindow::autorize_enter_button_clicked);
    connect(autorize_registration_button, &QPushButton::clicked, this,
            &MainWindow::autorize_registration_button_clicked);
    connect(send_message_button, &QPushButton::clicked, this,
            &MainWindow::send_message_button_clicked);
    connect(create_dialog_button, &QPushButton::clicked, this,
            &MainWindow::create_dialog_button_clicked);

    connect(dialogs, SIGNAL(currentRowChanged(int)), this, SLOT(dialog_chosen(int)));

    connect(downloader, SIGNAL(onError(QString, QString, QNetworkReply::NetworkError)), this,
            SLOT(erorRequest(QString, QString, QNetworkReply::NetworkError)));
    connect(downloader, SIGNAL(onReady(QString, QByteArray)), this,
            SLOT(resultShow(QString, QByteArray)));

    autorize_login_le->setText("user1");
    autorize_password_le->setText("0000");

    setCSS();

    autorize_ok_window();
    autorize_window();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::autorize_window()
{
    // Развёртывание окна авторизации

    auto *lay = ui->horizontalLayout;

    status = '1';

    clean_layout(lay);
    dialogs->clean();
    messages->clean();

    autorize_label_lay->addWidget(autorize_login_lab);
    autorize_label_lay->addWidget(autorize_password_lab);
    autorize_le_lay->addWidget(autorize_login_le);
    autorize_le_lay->addWidget(autorize_password_le);
    autorize_enter_lay->addLayout(autorize_label_lay);
    autorize_enter_lay->addLayout(autorize_le_lay);
    autorize_lay->addLayout(autorize_enter_lay);
    autorize_lay->addWidget(autorize_enter_button);
    autorize_lay->addWidget(autorize_registration_button);

    lay->addLayout(autorize_lay);
    show_widgets(lay);
    ui->menuBar->hide();
}

void MainWindow::autorize_ok_window()
{
    // Развёртывание основного окна

    auto *lay = ui->horizontalLayout;

    status = '2';

    clean_layout(lay);

    send_message_lay->addWidget(send_message_te);
    send_message_lay->addWidget(send_message_button);

    stat->setText(QString("id: ") + client.id + "\n" + "username: " + client.login);
    left_lay->addWidget(stat);
    left_lay->addWidget(create_dialog_button);
    left_lay->addWidget(dialogs);
    right_lay->addWidget(messages);
    right_lay->addLayout(send_message_lay);

    lay->addLayout(left_lay);
    lay->addLayout(right_lay);
    show_widgets(lay);
    ui->menuBar->show();
}

void MainWindow::clean_layout(QLayout *lay)
{
    // Скрытие виджетов в layout

    QLayoutItem *child;

    while ((child = lay->takeAt(0)) != 0)
        if (child->layout() != nullptr) {
                clean_layout(child->layout());
                //delete child->layout();
        }
        else child->widget()->hide();
}

void MainWindow::show_widgets(QLayout *lay)
{
    // Проявление виджетов в layout

    QLayoutItem *child;

    for (int i = 0; i < lay->count(); i++) {
        child = lay->itemAt(i);
        if (child->layout() != nullptr)
                show_widgets(child->layout());
        else child->widget()->show();
    }
}

void MainWindow::setCSS()
{
    QFile styleF;

    styleF.setFileName("style.css");
    styleF.open(QFile::ReadOnly);
    QString css_str = styleF.readAll();

    this->setStyleSheet(css_str);
}

void MainWindow::longpolling()
{
    // longpolling с сервером

    QString data = "";

    //запись id диалогов и id их последних сообщений в QString для отправки
    for (int i = 0; i < dialogs->size(); i++)
    {
        data += i != 0 ? " " : "";
        data += (*dialogs)[i].id + " " + (*dialogs)[i].id_last_message;
    }

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

    autorize_login_le->setEnabled(true);
    autorize_password_le->setEnabled(true);
}

void MainWindow::resultShow(QString url, QByteArray data)
{
    // Функция, обрабатывающая ответ сервера

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject root = document.object();
    QJsonArray val = root.value("results").toArray();
    QRegExp dia(".*/dialogs/\\d?/?$"),
            mes(".*/dialogs/\\d?/?messages/\\??.*"),
            lon(".*/users/\\d/longpolling/\\??.*"),
            che(".*/users/check/\\??.*");

    if (mes.indexIn(url) != -1) {
        Message *message;
        QJsonObject obj;

        for (int i = 0; i < val.size(); i++) {
            message = new Message;
            obj = val.at(i).toObject();

            message->id = QString(QString::number(obj.value("id").toInt()));
            message->dialog_id = QString( QString::number(obj.value("dialogID").toInt()) );
            message->owner_id = QString( QString::number(obj.value("ownerID").toInt()) );
            message->text = QString(  QByteArray::fromBase64( obj.value("text").toString().toLocal8Bit() ) );
            message->date = QString( obj.value("date").toString());

            if (message->dialog_id == dialogs->id_selected_dialog() &&
                status == '2')
                messages->addMessage(client.id, message);
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
                dialogs->addOrUpdateDialog(dialog);
        }
    }
    else if (che.indexIn(url) != -1) {
        QJsonObject obj = val.at(0).toObject();

        client.id = QString::number(obj.value("id").toInt());
        client.login = autorize_login_le->text();
        client.password = autorize_password_le->text();

        autorize_login_le->setEnabled(true);
        autorize_password_le->setEnabled(true);

        autorize_ok_window();
    }

    //если данные разделены на страницы, то получить остальные страницы

    if (root.value("next").isString())
        downloader->getData(root.value("next").toString(),
                            (client.login + ':' + client.password).toLocal8Bit(),
                            QMap<QString, QString>());

    if (lon.indexIn(url) != -1 || che.indexIn(url) != -1)
        longpolling();
}

void MainWindow::dialog_chosen(int ind)
{
    messages->clean();
    QMap<QString, QString> buf;
    buf.insert("messageID", dialogs->id_last_message(ind));
    buf.insert("countMessages", COUNT_MESSAGES);

    downloader->getData("http://127.0.0.1:8000/dialogs/" + dialogs->id_selected_dialog() + "/messages/",
                        (client.login + ':' + client.password).toLocal8Bit(), buf);
}








/*
 * css для элементов считывать из файла
 */

void MainWindow::on_pushButton_clicked()
{
    Dialog *dialog = new Dialog;
    dialog->id = "1";
    dialog->username = "username1";
    dialog->id_last_message = "1";
    dialog->date_last_message = "1";
    dialog->text_last_message = "message1";

    dialogs->addOrUpdateDialog(dialog);
}

void MainWindow::on_pushButton_2_clicked()
{
    Dialog *dialog = new Dialog;
    dialog->id = "2";
    dialog->username = "username2";
    dialog->id_last_message = "2";
    dialog->date_last_message = "2";
    dialog->text_last_message = "message2";

    dialogs->addOrUpdateDialog(dialog);
}

void MainWindow::send_message_button_clicked()
{
    //autorize_window();
    QString id = dialogs->id_selected_dialog();

    if (id.toInt() >= 0) {
        QMap<QString, QString> buf1;

        buf1.insert("dialogID", id);
        buf1.insert("text", send_message_te->toPlainText().toLocal8Bit().toBase64());

        downloader->postData("http://127.0.0.1:8000/messages/",
                             (client.login + ':' + client.password).toLocal8Bit(),
                             QMap<QString, QString>(), buf1);

        send_message_te->clear();
    }
}

void MainWindow::create_dialog_button_clicked()
{
    new_dialog_window->setUserData((client.login + ':' + client.password).toLocal8Bit());
    new_dialog_window->show();
}

void MainWindow::autorize_enter_button_clicked()
{
    autorize_login_le->setEnabled(false);
    autorize_password_le->setEnabled(false);
    downloader->getData(address_server"/users/check/",
                        (autorize_login_le->text() + ':' + autorize_password_le->text()).toLocal8Bit(),
                        QMap<QString, QString>());
    //autorize_ok_window();
}

void MainWindow::autorize_registration_button_clicked()
{
    QMap<QString, QString> buf1;

    buf1.insert("username", autorize_login_le->text());
    buf1.insert("password", autorize_password_le->text());

    downloader->postData(address_server"/users/", "", QMap<QString, QString>(), buf1);
}
