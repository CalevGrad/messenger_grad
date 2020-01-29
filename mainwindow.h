#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "newdialogwindow.h"
#include "downloader.h"
#include "dialoglistwidget.h"
#include "messagelistwidget.h"

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
    void longpolling();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void autorize_enter_button_clicked();
    void autorize_registration_button_clicked();
    void send_message_button_clicked();
    void create_dialog_button_clicked();
    void erorRequest(QString url, QString error_str, QNetworkReply::NetworkError error);
    void resultShow(QString url, QByteArray data);
    void dialog_chosen(int ind);

private:
    Ui::MainWindow *ui;
    User client;
    DialogListWidget *dialogs;
    MessageListWidget *messages;
    Downloader *downloader;
    QVBoxLayout *left_lay, // layout сообщений
                *right_lay, // layout диалогов
                *autorize_lay, // layout авторизации
                *autorize_le_lay, // layout для полей ввода логина и пароля
                *autorize_label_lay; // layout для текста перед логином и паролем
    QHBoxLayout *autorize_enter_lay, // layout для входа
                *send_message_lay;
    QLabel *stat, // описание пользователя после входа
           *autorize_login_lab, // текст перед логином
           *autorize_password_lab, // текст перед паролем
           *status_bar_text; //текст в статус баре
    QLineEdit *autorize_login_le, // строка для ввода логина
              *autorize_password_le; // строка для ввода пароля
    QTextEdit *send_message_te; // строка для ввода сообщения
    QPushButton *autorize_enter_button,
                *autorize_registration_button,
                *send_message_button,
                *create_dialog_button;
    NewDialogWindow *new_dialog_window;
    char status; // 1 - авторизация, 2 - вход выполнен, 3 - создание диалога

    void clean_layout(QLayout *lay);
    void show_widgets(QLayout *lay);
    void setCSS();
};

#endif // MAINWINDOW_H
