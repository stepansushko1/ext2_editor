#include "datablockbitmap.h"
#include "ui_datablockbitmap.h"

#include "iostream"

DataBlockBitmap::DataBlockBitmap(QWidget *parent) :QDialog(parent), ui(new Ui::DataBlockBitmap) {
    ui->setupUi(this);
    ui->DataBlockPrint->setText("00001 00001 00001 00001 00001 00001");
    ui->InputChangeDataBlock->setText("num,num | Input first byte, then bit to change");
}

DataBlockBitmap::~DataBlockBitmap()
{
    delete ui;
}


void DataBlockBitmap::on_SendChangesDataBlock_clicked()
{
    std::string temp = ui->InputChangeDataBlock->text().toStdString();
    std::string a, b;
    std::string word;

    for (const auto &x: temp){
        if (x == ','){

            a = word;
            word = "";
        }
        else {
            word = word + x;
        }

    }
    b = word;



    std::cout <<  a << b << std::endl;
}

