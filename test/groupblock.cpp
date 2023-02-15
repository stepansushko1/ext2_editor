#include "groupblock.h"
#include "ui_groupblock.h"
#include "superblockwindow.h"
#include "groupdescriptors.h"
#include "datablockbitmap.h"
#include "datablocks.h"
#include "inodetable.h"
#include "inodebitmap.h"
#include "inodefunc.h"

GroupBlock::GroupBlock(QWidget *parent) : QDialog(parent), ui(new Ui::GroupBlock) {
    ui->setupUi(this);
}

GroupBlock::~GroupBlock()
{
    delete ui;
}





void GroupBlock::on_SuperBlockButton_clicked()
{
    SuperBlockWindow sbw;
    sbw.setModal(true);
    sbw.exec();
}


void GroupBlock::on_GroupDescriptorsButton_clicked()
{
        GroupDescriptors gd;
        gd.setModal(true);
        gd.exec();
}


void GroupBlock::on_BlockBitmapButton_clicked()
{
    DataBlockBitmap dbb;
    dbb.setModal(true);
    dbb.exec();
}


void GroupBlock::on_InodeBitmap_clicked()
{
    InodeBitmap ib;
    ib.setModal(true);
    ib.exec();
}


void GroupBlock::on_InodeTableButton_clicked()
{
    InodeTable it;
    it.setModal(true);
    it.exec();
}


void GroupBlock::on_DataBlocksButton_clicked()
{

}


void GroupBlock::on_pushButton_clicked()
{
    InodeFunc ifun;
    ifun.setModal(true);
    ifun.exec();
}

