#include "superblockwindow.h"
#include "ui_superblockwindow.h"
#include "QVBoxLayout"
#include "QScrollBar"
#include "QPushButton"
#include "QLineEdit"
#include "QGroupBox"
#include "QGridLayout"
#include "QLabel"
#include "iostream"

#include "string"

#include "funtional_part.h"

std::string t(){
    return "hello";
}

SuperBlockWindow::SuperBlockWindow(QWidget *parent) :QDialog(parent), ui(new Ui::SuperBlockWindow){
    ui->setupUi(this);

    int fd = get_fd();

    std::vector<std::string> superblock_info = print_super(fd);


    ui->s_inodes_count->setText(QString::fromStdString(superblock_info[0]));
    ui->s_blocks_count->setText(QString::fromStdString(superblock_info[1]));
    ui->s_r_blocks_count->setText(QString::fromStdString(superblock_info[2]));
    ui->s_free_blocks_count->setText(QString::fromStdString(superblock_info[3]));
    ui->s_free_inodes_count->setText(QString::fromStdString(superblock_info[4]));
    ui->s_first_data_block->setText(QString::fromStdString(superblock_info[5]));
    ui->s_log_block_size->setText(QString::fromStdString(superblock_info[6]));
    ui->s_blocks_per_group->setText(QString::fromStdString(superblock_info[7]));
    ui->s_mtime->setText(QString::fromStdString(superblock_info[8]));
    ui->s_wtime->setText(QString::fromStdString(superblock_info[9]));
    ui->s_mnt_count->setText(QString::fromStdString(superblock_info[10]));
    ui->s_max_mnt_count->setText(QString::fromStdString(superblock_info[11]));
    ui->s_magic->setText(QString::fromStdString(superblock_info[12]));
    ui->s_state->setText(QString::fromStdString(superblock_info[13]));
    ui->s_errors->setText(QString::fromStdString(superblock_info[14]));
    ui->s_minor_rev_level->setText(QString::fromStdString(superblock_info[15]));
    ui->s_lastcheck->setText(QString::fromStdString(superblock_info[16]));
    ui->s_checkinterval->setText(QString::fromStdString(superblock_info[17]));
    ui->s_creator_os->setText(QString::fromStdString(superblock_info[18]));
    ui->s_rev_level->setText(QString::fromStdString(superblock_info[19]));
    ui->s_def_resuid->setText(QString::fromStdString(superblock_info[20]));
    ui->s_def_resgid->setText(QString::fromStdString(superblock_info[21]));
    ui->s_first_ino->setText(QString::fromStdString(superblock_info[22]));
    ui->s_inode_size->setText(QString::fromStdString(superblock_info[23]));
    ui->s_block_group_nr->setText(QString::fromStdString(superblock_info[24]));
    ui->s_feature_compat->setText(QString::fromStdString(superblock_info[25]));
    ui->s_feature_incompat->setText(QString::fromStdString(superblock_info[26]));
    ui->s_feature_ro_compat->setText(QString::fromStdString(superblock_info[27]));
    ui->s_prealloc_dir_blocks->setText(QString::fromStdString(superblock_info[28]));
    ui->s_journal_inum->setText(QString::fromStdString(superblock_info[29]));
    ui->s_journal_dev->setText(QString::fromStdString(superblock_info[30]));
    ui->s_last_orphan->setText(QString::fromStdString(superblock_info[31]));
    ui->s_def_hash_version->setText(QString::fromStdString(superblock_info[32]));
    ui->s_first_meta_bg->setText(QString::fromStdString(superblock_info[33]));
}



SuperBlockWindow::~SuperBlockWindow()
{
    delete ui;
}
//ScrollParametersSuperBlock

//void SuperBlockWindow::on_pushButton_2_clicked()
//{
//    std::cout << ui->lineEdit_2->text().toDouble() << std::endl;
//    int t = ui->lineEdit_2->text().toDouble() ;
//    ui->lineEdit_2->setText(QString::number(t));
//}

