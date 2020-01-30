#include "autorizewidget.h"

AutorizeWidget::AutorizeWidget(QWidget *parent) : QWidget(parent)
{
    widget = new QWidget;
    lay_v_elem = new QVBoxLayout;
    le_lay = new QVBoxLayout;
    label_lay = new QVBoxLayout;
    enter_lay = new QHBoxLayout;
    lay = new QHBoxLayout;
    login_lab = new QLabel;
    password_lab = new QLabel;
    login_le = new QLineEdit;
    password_le = new QLineEdit;
    enter_button = new QPushButton;
    registration_button = new QPushButton;

    login_le->setText("user1");
    password_le->setText("0000");
    enter_button->setText("Вход");
    registration_button->setText("Регистрация");
    login_lab->setText("Логин: ");
    password_lab->setText("Пароль: ");

    label_lay->addWidget(login_lab);
    label_lay->addWidget(password_lab);
    le_lay->addWidget(login_le);
    le_lay->addWidget(password_le);
    enter_lay->addLayout(label_lay);
    enter_lay->addLayout(le_lay);
    lay_v_elem->addLayout(enter_lay);
    lay_v_elem->addWidget(enter_button);
    lay_v_elem->addWidget(registration_button);

    widget->setLayout(lay_v_elem);
    widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    lay->addWidget(widget);

    this->setLayout(lay);

    connect(enter_button, &QPushButton::clicked, this, &AutorizeWidget::enter_button_clicked);
    connect(registration_button, &QPushButton::clicked, this, &AutorizeWidget::registration_button_clicked);
}

void AutorizeWidget::setEnableLineEdit(bool value)
{
    login_le->setEnabled(value);
    password_le->setEnabled(value);
}

QString AutorizeWidget::getLogin()
{
    return login_le->text();
}

QString AutorizeWidget::getPassword()
{
    return password_le->text();
}

//autorize_login_le->setEnabled(true);
//autorize_password_le->setEnabled(true);

/*
        client.login = autorize_login_le->text();
        client.password = autorize_password_le->text();

        autorize_login_le->setEnabled(true);
        autorize_password_le->setEnabled(true);*/

