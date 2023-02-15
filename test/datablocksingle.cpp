#include "datablocksingle.h"
#include "ui_datablocksingle.h"

DataBlockSingle::DataBlockSingle(QWidget *parent, int num_data_block) :
    QDialog(parent),
    ui(new Ui::DataBlockSingle)
{
    ui->setupUi(this);
}

DataBlockSingle::~DataBlockSingle()
{
    delete ui;
}
