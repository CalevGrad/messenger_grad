#include "correspondencewidget.h"

CorrespondenceWidget::CorrespondenceWidget(QWidget *parent) : QWidget(parent)
{
    dialogs = new DialogListWidget;
    messages = new MessageListWidget;

    left_lay = new QVBoxLayout;
    right_lay = new QVBoxLayout;
    send_message_lay = new QHBoxLayout;
    main_lay = new QHBoxLayout;
    stat = new QLabel;
    send_message_te = new QTextEdit;
    send_message_button = new QPushButton;
    create_dialog_button = new QPushButton;
    main_lay = new QHBoxLayout;
    left_lay = new QVBoxLayout;
    right_lay = new QVBoxLayout;
    stat = new QLabel;
    send_message_lay = new QHBoxLayout;
    send_message_te = new QTextEdit;

    send_message_lay->addWidget(send_message_te);
    send_message_lay->addWidget(send_message_button);
    left_lay->addWidget(stat);
    left_lay->addWidget(create_dialog_button);
    left_lay->addWidget(dialogs);
    right_lay->addWidget(messages);
    right_lay->addLayout(send_message_lay);
    main_lay->addLayout(left_lay);
    main_lay->addLayout(right_lay);

    this->setLayout(main_lay);

    messages->setSelectionMode( QAbstractItemView::NoSelection );
    messages->setFocusPolicy(Qt::NoFocus);

    send_message_button->setText("Отправить");
    create_dialog_button->setText("Создать диалог");

    connect(send_message_button, &QPushButton::clicked, this,
            &CorrespondenceWidget::send_message_button_clicked);
    connect(create_dialog_button, &QPushButton::clicked, this,
            &CorrespondenceWidget::create_dialog_button_clicked);
    connect(dialogs, SIGNAL(currentRowChanged(int)), this, SIGNAL(dialog_clicked(int)));
}

void CorrespondenceWidget::messagesClean()
{
    messages->clean();
}

void CorrespondenceWidget::dialogsClean()
{
    dialogs->clean();
}

void CorrespondenceWidget::setStat(QString id, QString login){
    stat->setText(QString("id: ") + id + "\n" + "username: " + login);
}

void CorrespondenceWidget::sendTEClear()
{
    send_message_te->clear();
}

QString CorrespondenceWidget::getSendMessageTE()
{
    return send_message_te->toPlainText();
}

QString CorrespondenceWidget::getData()
{
    QString data;

    //запись id диалогов и id их последних сообщений в QString для отправки
    for (int i = 0; i < dialogs->size(); i++)
    {
        data += i != 0 ? " " : "";
        data += (*dialogs)[i].id + " " + (*dialogs)[i].id_last_message;
    }

    return data;
}

QString CorrespondenceWidget::getIdSelectedDialog()
{
    return dialogs->id_selected_dialog();
}

QString CorrespondenceWidget::idLastMessage(int ind)
{
    return dialogs->id_last_message(ind);
}

QString CorrespondenceWidget::idSelectedDialog()
{
    return dialogs->id_selected_dialog();
}

void CorrespondenceWidget::addMessage(QString id, Message *message)
{
    messages->addMessage(id, message);
}

void CorrespondenceWidget::addOrUpdateDialog(Dialog *dialog)
{
    dialogs->addOrUpdateDialog(dialog);
}
