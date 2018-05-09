#include "customdialog.h"
#include "ui_customdialog.h"
#include <QDebug>

customDialog::customDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Custom");
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok_accepted()));
    QWidget::setTabOrder(ui->lineEdit_1, ui->lineEdit_2);
    QWidget::setTabOrder(ui->lineEdit_2, ui->lineEdit_3);
}

customDialog::~customDialog()
{
    delete ui;
}

void customDialog::show()
{
//   ui->lineEdit_1->setText(QString::number(row, 10));
//   ui->lineEdit_2->setText(QString::number(col, 10));
//   ui->lineEdit_3->setText(QString::number(mines, 10));
   ui->lineEdit_1->setText(QString("%1").arg(row));
   ui->lineEdit_2->setText(QString("%1").arg(col));
   ui->lineEdit_3->setText(QString("%1").arg(mines));
}

void customDialog::ok_accepted()
{
   row = ui->lineEdit_1->text().toInt();
   col = ui->lineEdit_2->text().toInt();
   mines = ui->lineEdit_3->text().toInt();
}
