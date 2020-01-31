#include "newdialogwindow.h"
#include "ui_newdialogwindow.h"

NewDialogWindow::NewDialogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewDialogWindow)
{
    ui->setupUi(this);
    downloader = new Downloader;
}

NewDialogWindow::~NewDialogWindow()
{
    delete ui;
}

void NewDialogWindow::setUserData(QByteArray data)
{
    user_data = data;
}
void NewDialogWindow::on_start_dia_button_clicked()
{
    QMap<QString, QString> buf1;

    buf1.insert("recipientID", ui->id_rec_le->text());
    buf1.insert("textFirstMessage", ui->text_fir_mes_te->toPlainText().toLocal8Bit().toBase64());

    ui->id_rec_le->clear();
    ui->text_fir_mes_te->clear();

    downloader->postData("http://127.0.0.1:8000/dialogs/", user_data, QMap<QString, QString>(), buf1);
    this->close();
}
