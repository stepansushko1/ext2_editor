#include "groupdescriptors.h"
#include "ui_groupdescriptors.h"

GroupDescriptors::GroupDescriptors(QWidget *parent) : QDialog(parent), ui(new Ui::GroupDescriptors) {
    ui->setupUi(this);

    ui->bg_block_bitmap->setText("123994");
    ui->bg_inode_bitmap->setText("234");
    ui->bg_inode_table->setText("1455");
    ui->bg_free_blocks_count->setText("100");
    ui->bg_free_inodes_count->setText("404");
    ui->bg_used_dirs_count->setText("345");
    ui->bg_pad->setText("55");
    ui->bg_reserved->setText("11");
}

GroupDescriptors::~GroupDescriptors()
{
    delete ui;
}
