#ifndef MESSAGELISTWIDGET_H
#define MESSAGELISTWIDGET_H

#include <QListWidget>
#include <QList>

typedef struct
{
    QString id,
            dialog_id,
            owner_id,
            text,
            date;
} Message;

class MessageListWidget : public QListWidget
{
    Q_OBJECT

public:
    MessageListWidget();
    void addMessage(QString user_id, Message *message);
    void clean();

private:
    QList<Message*> list_messages;
    QWidget* getWidget(Message *message, bool foreign);
};

#endif // MESSAGELISTWIDGET_H
