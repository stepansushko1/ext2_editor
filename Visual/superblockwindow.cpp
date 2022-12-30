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

std::string t(){
    return "hello";
}

SuperBlockWindow::SuperBlockWindow(QWidget *parent) :QDialog(parent), ui(new Ui::SuperBlockWindow){
    ui->setupUi(this);

    ui->s_inodes_count->setText(QString::fromStdString("1235"));
    ui->s_blocks_count->setText(QString::fromStdString("589"));
    ui->s_r_blocks_count->setText(QString::fromStdString("345"));
    ui->s_free_blocks_count->setText(QString::fromStdString("1003"));
    ui->s_free_inodes_count->setText(QString::fromStdString("200"));
    ui->s_first_data_block->setText(QString::fromStdString("4"));
    ui->s_log_block_size->setText(QString::fromStdString("20"));
    ui->s_log_frag_size->setText(QString::fromStdString("1234"));
    ui->s_blocks_per_group->setText(QString::fromStdString("2134"));
    ui->s_frags_per_group->setText(QString::fromStdString("545"));
//    ui->s_inodes_per_group->setText(QString::fromStdString("ttt"));
    ui->s_mtime->setText(QString::fromStdString("12"));
    ui->s_wtime->setText(QString::fromStdString("24"));
    ui->s_mnt_count->setText(QString::fromStdString("38"));
    ui->s_max_mnt_count->setText(QString::fromStdString("123"));
    ui->s_magic->setText(QString::fromStdString("953"));
    ui->s_state->setText(QString::fromStdString("4545"));
    ui->s_errors->setText(QString::fromStdString("1"));
    ui->s_minor_rev_level->setText(QString::fromStdString("234"));
    ui->s_lastcheck->setText(QString::fromStdString("234"));
    ui->s_checkinterval->setText(QString::fromStdString("234"));
    ui->s_creator_os->setText(QString::fromStdString("54"));
    ui->s_rev_level->setText(QString::fromStdString("34"));
    ui->s_def_resuid->setText(QString::fromStdString("64"));
    ui->s_def_resgid->setText(QString::fromStdString("88"));
    ui->s_first_ino->setText(QString::fromStdString("656"));
    ui->s_inode_size->setText(QString::fromStdString("ttt"));
    ui->s_block_group_nr->setText(QString::fromStdString("ttt"));
    ui->s_feature_compat->setText(QString::fromStdString("ttt"));
    ui->s_feature_incompat->setText(QString::fromStdString("ttt"));
    ui->s_feature_ro_compat->setText(QString::fromStdString("ttt"));
    ui->s_uuid->setText(QString::fromStdString("ttt"));
    ui->s_volume_name->setText(QString::fromStdString("ttt"));
    ui->s_last_mounted->setText(QString::fromStdString("ttt"));
    ui->s_algo_bitmap->setText(QString::fromStdString("ttt"));
    ui->s_prealloc_dir_blocks->setText(QString::fromStdString("ttt"));
    ui->s_journal_uuid->setText(QString::fromStdString("ttt"));
    ui->s_journal_inum->setText(QString::fromStdString("ttt"));
    ui->s_journal_dev->setText(QString::fromStdString("ttt"));
    ui->s_last_orphan->setText(QString::fromStdString("ttt"));
    ui->s_hash_seed->setText(QString::fromStdString("ttt"));
    ui->s_def_hash_version->setText(QString::fromStdString("ttt"));
    ui->s_default_mount_options->setText(QString::fromStdString("ttt"));
    ui->s_first_meta_bg->setText(QString::fromStdString("ttt"));
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

