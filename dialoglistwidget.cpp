#include "dialoglistwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

DialogListWidget::DialogListWidget()
{
}

void DialogListWidget::updateDialog(int pos, Dialog *dialog)
{
    // Эта функция обновляет данные диалога и перерисовывает его
    bool flag = false;
    int ind = this->currentRow();

    if (ind == pos) {
        flag = true;
        emit dialogUpdate(true);
    }

    QWidget *dialog_widget = getWidget(dialog);
    QListWidgetItem *item = this->takeItem(pos);

    list_dialogs.replace(pos, dialog);
    list_dialogs.move(pos, 0);
    this->insertItem(0, item);
    this->setItemWidget(item, dialog_widget);

    if (flag) {
        this->setCurrentRow(0);
        emit dialogUpdate(false);
    }
    else this->setCurrentRow(ind + 1);

}

void DialogListWidget::createDialog(Dialog *dialog, int pos)
{
    // Эта функция создаёт новый диалог

    QListWidgetItem* item = new QListWidgetItem;
    QWidget *dialog_widget = getWidget(dialog);

    list_dialogs.insert(pos, dialog);
    item->setSizeHint(dialog_widget->sizeHint());

    this->insertItem(pos, item);
    this->setItemWidget( item, dialog_widget );
}

void DialogListWidget::addOrUpdateDialog(Dialog* dialog)
{
    int pos = -1;

    // нахождение места в списке для диалога или обновление диалога,
    // если он уже есть

    for (int i = list_dialogs.count() - 1; i >= 0; i--) {
        if (list_dialogs.value(i)->id_last_message.toInt() > dialog->id_last_message.toInt()) {
            pos = i;
            break;
        }
        if (list_dialogs.value(i)->id == dialog->id)
        {
            updateDialog(i, dialog);
            return;
        }
    }
    pos++;

    createDialog(dialog, pos);
}

int DialogListWidget::size()
{
    return list_dialogs.size();
}

void DialogListWidget::clean()
{
    list_dialogs.clear();
    this->clear();
}

QString DialogListWidget::id_selected_dialog()
{
    int ind = this->currentRow();

    if (ind >= 0)
        return list_dialogs[ind]->id;

    return "-1";
}

QString DialogListWidget::id_last_message(int ind)
{
    return list_dialogs[ind]->id_last_message;
}

Dialog &DialogListWidget::operator[](const int index)
{
    return *list_dialogs[index];
}

QWidget* DialogListWidget::getWidget(Dialog *dialog)
{
    // Эта функция возвращает стилизованый виджет диалога

    QWidget *dialog_widget = new QWidget;
    QLayout *lay = new QVBoxLayout;
    QLabel *username_label,
           *text_last_message_label,
           *date_last_message_label;
    QRegExp reg;

    // замена символов переноса строки на пробел
    dialog->username.replace('\n', ' ');
    dialog->text_last_message.replace('\n', ' ');
    dialog->text_last_message.replace('\n', ' ');

    // преобразование даты
    reg.setPattern("(.*)-(.*)-(.*)T(.*:.*):.*");
    reg.indexIn(dialog->date_last_message);
    dialog->date_last_message = reg.cap(3) + '-' + reg.cap(2) + '-' + reg.cap(1) + ' ' + reg.cap(4);
    dialog->date_last_message.replace('T', ' ');

    // обрезка текста при необходимости
    if (dialog->username.length() <= 53)
        username_label =  new QLabel(dialog->username);
    else username_label = new QLabel(dialog->username.left(50) + "...");

    if (dialog->text_last_message.length() <= 53)
        text_last_message_label = new QLabel(dialog->text_last_message);
    else text_last_message_label = new QLabel(dialog->text_last_message.left(50) + "...");

    if (dialog->date_last_message.length() <= 53)
        date_last_message_label = new QLabel(dialog->date_last_message);
    else date_last_message_label = new QLabel(dialog->date_last_message.left(50) + "...");

    // настройка внешного вида QLabel-ов
    username_label->setAlignment(Qt::AlignCenter);
    text_last_message_label->setAlignment(Qt::AlignLeft);
    date_last_message_label->setAlignment(Qt::AlignRight);

    username_label->setStyleSheet("font-weight: bold;");
    text_last_message_label->setStyleSheet("color: #434343;");
    date_last_message_label->setStyleSheet("color: grey;");

    // добавление QLabel-ов
    lay->addWidget(username_label);
    lay->addWidget(text_last_message_label);
    lay->addWidget(date_last_message_label);

    dialog_widget->setObjectName("dialog_widget");
    dialog_widget->setStyleSheet("QWidget#dialog_widget { border: 1px solid black; margin: 1px; border-radius: 2px; }");
    dialog_widget->setLayout(lay);

    return dialog_widget;
}
