#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "newdialogwindow.h"
#include "downloader.h"

#include "autorizewidget.h"
#include "correspondencewidget.h"

#define address_server "http://127.0.0.1:8000"

typedef struct
{
    QString id,
            login,
            password;
} User;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void autorize_window();
    void autorize_ok_window();

    void autorize_widget_show();
    void correspondence_widget_show();
    void longpolling();

private slots:
    void autorize_enter_button_clicked();
    void autorize_registration_button_clicked();

    void send_message_button_clicked();
    void create_dialog_button_clicked();

    void erorRequest(QString url, QString error_str, QNetworkReply::NetworkError error);
    void resultShow(QString url, QByteArray data);

    void dialog_clicked(int ind);

private:
    Ui::MainWindow *ui;
    User client;
    Downloader *downloader;
    AutorizeWidget *autorize_widget;
    CorrespondenceWidget *correspondence_widget;
    NewDialogWindow *new_dialog_window;
    QLabel *status_bar_text;

    char status; // 1 - авторизация, 2 - вход выполнен, 3 - создание диалога

    void clean_layout(QLayout *lay);
    void show_widgets(QLayout *lay);
    void setCSS();
    void widget_processing();
};

#endif // MAINWINDOW_H
