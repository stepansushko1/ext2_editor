#include "groupblock.h"
#include "ui_groupblock.h"
#include "superblockwindow.h"
#include "groupdescriptors.h"
#include "datablockbitmap.h"
#include "datablocks.h"
#include "inodetable.h"
#include "inodebitmap.h"

GroupBlock::GroupBlock(QWidget *parent) : QDialog(parent), ui(new Ui::GroupBlock) {
    ui->setupUi(this);
//    ui->InodeBitmap->setText("2");
}

GroupBlock::~GroupBlock()
{
    delete ui;
}


//GroupBlock *one_group_block = new GroupBlock(this);


//auto * temp_button = new QPushButton( "Group Block " + QString::number(i), container);
//temp_button->resize(100,100);
//temp_button->setObjectName("GroupBlockNum" + QString::number(i));

//buttons_vector.push_back(temp_button);
//layout->addWidget( temp_button );
//connect(temp_button, SIGNAL(clicked()), one_group_block, SLOT(show()));
//void GroupBlock::on_SuperBlockButton_clicked()
//{
//    SuperBlockWindow sbw;
//    sbw.setModal(true);
//    sbw.exec();
//}


//void GroupBlock::on_BlockBitmapButton_clicked()
//{

//}


//void GroupBlock::on_GroupDescriptorsButton_clicked()
//{
//    GroupDescriptors gd;
//    gd.setModal(true);
//    gd.exec();
//}


//void GroupBlock::on_InodeBitmap_clicked()
//{

//}


//void GroupBlock::on_InodeTableButton_clicked()
//{

//}


//void GroupBlock::on_DataBlocksButton_clicked()
//{

//}


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
    DataBlocks data_blocks;
    data_blocks.setModal(true);
    data_blocks.exec();
}

