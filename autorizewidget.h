#ifndef AUTORIZEWIDGET_H
#define AUTORIZEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


class AutorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AutorizeWidget(QWidget *parent = nullptr);
    void setEnableLineEdit(bool value);
    QString getLogin();
    QString getPassword();

private:
    QWidget *widget;
    QVBoxLayout *lay_v_elem, // layout авторизации
                *le_lay, // layout для полей ввода логина и пароля
                *label_lay; // layout для текста перед логином и паролем
    QHBoxLayout *enter_lay, // layout для входа
                *lay; // layout виджета авторизации
    QLabel *login_lab, // текст перед логином
           *password_lab; // текст перед паролем
    QLineEdit *login_le, // строка для ввода логина
              *password_le; // строка для ввода пароля
    QPushButton *enter_button,
                *registration_button;

signals:
    void enter_button_clicked();
    void registration_button_clicked();
};

#endif // AUTORIZEWIDGET_H
