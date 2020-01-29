#include "messagelistwidget.h"
#include <QVBoxLayout>
#include <QLabel>

MessageListWidget::MessageListWidget()
{
}

void MessageListWidget::addMessage(QString user_id, Message *message)
{
    int pos = 0;
    QListWidgetItem* item = new QListWidgetItem;
    QWidget *message_widget = getWidget(message, message->owner_id == user_id);

    // нахождение места в списке для сообщения или выход из функции добавления
    // если сообщение уже есть

    for (int i = 0; i < list_messages.count(); i++) {
        if (list_messages.value(i)->id.toInt() > message->id.toInt()) {
            pos = i;
            break;
        }
        if (list_messages.value(i)->id == message->id)
            return;
    }

    this->insertItem(pos, item);
    list_messages.insert(pos, message);
    item->setSizeHint(message_widget->sizeHint());

    this->setItemWidget(item, message_widget);
    this->scrollToBottom();
}

void MessageListWidget::clean()
{
    list_messages.clear();
    this->clear();
}

QWidget* MessageListWidget::getWidget(Message *message, bool foreign)
{
    QWidget *message_widget = new QWidget;
    QLayout *lay = new QVBoxLayout;
    QLabel *text_last_message_label,
            *date_last_message_label;
    QRegExp reg;

    //преобразование даты
    reg.setPattern("(.*)-(.*)-(.*)T(.*:.*):.*");
    reg.indexIn(message->date);
    message->date = reg.cap(3) + '-' + reg.cap(2) + '-' + reg.cap(1) + ' ' + reg.cap(4);
    message->date.replace('T', ' ');

    text_last_message_label = new QLabel(message->text);
    date_last_message_label = new QLabel(message->date);

    //настройка внешного вида QLabel-ов
    text_last_message_label->setAlignment(Qt::AlignLeft);
    date_last_message_label->setAlignment(Qt::AlignRight);
    text_last_message_label->setWordWrap(true);
    date_last_message_label->setWordWrap(true);

    text_last_message_label->setTextInteractionFlags( text_last_message_label->textInteractionFlags() | Qt::TextSelectableByMouse ); //разрешение на выделение текста QLabel

    date_last_message_label->setStyleSheet("color: grey;");

    lay->addWidget(text_last_message_label);
    lay->addWidget(date_last_message_label);

    message_widget->setObjectName("message_widget");

    if (!foreign)
    {
        message_widget->setStyleSheet("QWidget#message_widget { border: 1px solid black; margin: 1px; border-radius: 2px; }");
    }
    else {
        message_widget->setStyleSheet("QWidget#message_widget { border: 1px solid black; margin: 1px; border-radius: 2px; background-color: #e0ecff; }");
    }

    message_widget->setLayout(lay);

    return message_widget;
}
