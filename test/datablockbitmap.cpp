#include "datablockbitmap.h"
#include "ui_datablockbitmap.h"
#include "funtional_part.h"

#include "iostream"

DataBlockBitmap::DataBlockBitmap(QWidget *parent) :QDialog(parent), ui(new Ui::DataBlockBitmap) {
    ui->setupUi(this);

    int fd = get_fd();
    std::string bitmap = block_bitmap(fd);

    std::string out_bitmap = transform(bitmap);

    ui->DataBlockPrint->setText(QString::fromStdString(out_bitmap));
}

DataBlockBitmap::~DataBlockBitmap()
{
    delete ui;
}


//void DataBlockBitmap::on_SendChangesDataBlock_clicked()
//{
//    std::string temp = ui->InputChangeDataBlock->text().toStdString();
//    std::string a, b;
//    std::string word;

//    for (const auto &x: temp){
//        if (x == ','){

//            a = word;
//            word = "";
//        }
//        else {
//            word = word + x;
//        }

//    }
//    b = word;



//    std::cout <<  a << b << std::endl;
//}

