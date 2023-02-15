#include "groupdescriptors.h"
#include "ui_groupdescriptors.h"
#include "funtional_part.h"

GroupDescriptors::GroupDescriptors(QWidget *parent) : QDialog(parent), ui(new Ui::GroupDescriptors) {
    ui->setupUi(this);
    int fd = get_fd();
    std::vector<std::string> group_descriptors = print_group(fd);

    ui->bg_block_bitmap->setText(QString::fromStdString(group_descriptors[0]));
    ui->bg_inode_bitmap->setText(QString::fromStdString(group_descriptors[1]));
    ui->bg_inode_table->setText(QString::fromStdString(group_descriptors[2]));
    ui->bg_free_blocks_count->setText(QString::fromStdString(group_descriptors[3]));
    ui->bg_free_inodes_count->setText(QString::fromStdString(group_descriptors[4]));
    ui->bg_used_dirs_count->setText(QString::fromStdString(group_descriptors[5]));
}

GroupDescriptors::~GroupDescriptors()
{
    delete ui;
}
