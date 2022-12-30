#include "inodebitmap.h"
#include "ui_inodebitmap.h"

InodeBitmap::InodeBitmap(QWidget *parent) : QDialog(parent), ui(new Ui::InodeBitmap) {
    ui->setupUi(this);
    ui->InodeBitmapText->setText("00001 00001 00001 00001 00001 00001");
    ui->InputBitmapChange->setText("num,num | Input first byte, then bit to change");
}

InodeBitmap::~InodeBitmap()
{
    delete ui;
}
