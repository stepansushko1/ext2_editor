#include "inodetableoneinode.h"
#include "ui_inodetableoneinode.h"
#include "iostream"

InodeTableOneInode::InodeTableOneInode(QWidget *parent, int num_of_inode) : QDialog(parent), ui(new Ui::InodeTableOneInode) {
//    std::cout << m_path.toStdString() << std::endl;
    ui->setupUi(this);
    std::cout << num_of_inode << std::endl;
    // here function to receive parameters
    ui->i_mode->setText("1");
    ui->i_uid->setText("554535");
    ui->i_size->setText("15");
    ui->i_atime->setText("53");
    ui->i_ctime->setText("35");
    ui->i_mtime->setText("35");
    ui->i_dtime->setText("345");
    ui->i_gid->setText("1345");
    ui->i_links_count->setText("1345");
    ui->i_blocks->setText("1345");
    ui->i_flags->setText("1345");
    ui->i_osd1->setText("1345");
    ui->i_block->setText("545");
    ui->i_generation->setText("345");
    ui->i_file_acl->setText("3452");
    ui->i_dir_acl->setText("96");
    ui->i_faddr->setText("001013");

}

InodeTableOneInode::~InodeTableOneInode()
{
    delete ui;
}




void InodeTableOneInode::on_SubmitChange_clicked()
{
    std::string i_mode = ui->i_mode->text().toStdString();
    std::string i_uid = ui->i_uid->text().toStdString();
    std::string i_size = ui->i_size->text().toStdString();
    std::string i_atime = ui->i_atime->text().toStdString();
    std::string i_ctime = ui->i_ctime->text().toStdString();
    std::string i_mtime = ui->i_mtime->text().toStdString();
    std::string i_dtime = ui->i_dtime->text().toStdString();
    std::string i_gid = ui->i_gid->text().toStdString();
    std::string i_links_count = ui->i_links_count->text().toStdString();
    std::string i_blocks = ui->i_blocks->text().toStdString();
    std::string i_flags = ui->i_flags->text().toStdString();
    std::string i_osd1 = ui->i_osd1->text().toStdString();
    std::string i_block = ui->i_block->text().toStdString();
    std::string i_generation = ui->i_generation->text().toStdString();
    std::string i_file_acl = ui->i_file_acl->text().toStdString();
    std::string i_dir_acl = ui->i_dir_acl->text().toStdString();
    std::string i_faddr = ui->i_faddr->text().toStdString();
    // function send
    std::cout << ui->i_mode->text().toStdString() << std::endl;
}

