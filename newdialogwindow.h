#ifndef NEWDIALOGWINDOW_H
#define NEWDIALOGWINDOW_H

#include <QWidget>
#include "downloader.h"

namespace Ui {
class NewDialogWindow;
}

class NewDialogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewDialogWindow(QWidget *parent = nullptr);
    ~NewDialogWindow();
    void setUserData(QByteArray data);

private slots:
    void on_start_dia_button_clicked();

private:
    Ui::NewDialogWindow *ui;
    Downloader* downloader;
    QByteArray user_data;
};

#endif // NEWDIALOGWINDOW_H
