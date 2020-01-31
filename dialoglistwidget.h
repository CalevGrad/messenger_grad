#ifndef DIALOGLISTWIDGET_H
#define DIALOGLISTWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QList>

typedef struct
{
    QString id, // id диалога
            text_last_message, // текст последнего сообщения
            date_last_message, // дата последнего сообщения
            id_last_message, // id последнего сообщения
            username; // имя собеседника
    QList<QString*> owners; // владельцы диалога [0] - имя, [1] - id
} Dialog;

class DialogListWidget : public QListWidget
{
    Q_OBJECT

public:
    DialogListWidget();
    void addOrUpdateDialog(Dialog *dialog);
    int size();
    void clean();
    QString id_selected_dialog();
    QString id_last_message(int ind);
    Dialog& operator[] (const int index);

private:
    QList<Dialog*> list_dialogs;
    void updateDialog(int pos, Dialog *dialog);
    void createDialog(Dialog *dialog, int pos);
    QWidget* getWidget(Dialog *dialog);

signals:
    void dialogUpdate(bool a);
};

#endif // DIALOGLISTWIDGET_H
