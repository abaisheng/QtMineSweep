#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>

namespace Ui {
class customDialog;
}

class customDialog : public QDialog
{
    Q_OBJECT

public:
    explicit customDialog(QWidget *parent = 0);
    ~customDialog();
    int row;
    int col;
    int mines;
    void show();

private slots:
    void ok_accepted();

private:
    Ui::customDialog *ui;

};

#endif // CUSTOMDIALOG_H
