#ifndef CORRESPONDENCEWIDGET_H
#define CORRESPONDENCEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

#include "dialoglistwidget.h"
#include "messagelistwidget.h"

class CorrespondenceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CorrespondenceWidget(QWidget *parent = nullptr);
    void messagesClean();
    void dialogsClean();
    void setStat(QString id, QString login);
    void sendTEClear();
    void addMessage(QString id, Message *message);
    void addOrUpdateDialog(Dialog *dialog);
    bool isDialogUpdate();
    QString getSendMessageTE();
    QString getData();
    QString idLastMessage(int ind);
    QString idSelectedDialog();
    int dialogsCurrentRow();


private:
    DialogListWidget *dialogs;
    MessageListWidget *messages;

    bool dialog_update;

    QVBoxLayout *left_lay, // layout сообщений
                *right_lay; // layout диалогов
    QHBoxLayout *send_message_lay,
                *main_lay;
    QLabel *stat, // описание пользователя после входа
           *status_bar_text; //текст в статус баре
    QTextEdit *send_message_te; // строка для ввода сообщения
    QPushButton *send_message_button,
                *create_dialog_button;

private slots:
    void dialogUpdate(bool a);

signals:
    void create_dialog_button_clicked();
    void send_message_button_clicked();
    void dialog_clicked(int);
};

#endif // CORRESPONDENCEWIDGET_H
