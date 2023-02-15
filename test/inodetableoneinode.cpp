#include "inodetableoneinode.h"
#include "ui_inodetableoneinode.h"
#include "iostream"
#include "funtional_part.h"

InodeTableOneInode::InodeTableOneInode(QWidget *parent, int num_of_inode) : QDialog(parent), ui(new Ui::InodeTableOneInode) {
//    std::cout << m_path.toStdString() << std::endl;
    ui->setupUi(this);
    std::cout << "This is number of current inode = " << num_of_inode << std::endl;

    // functio to get info
    int fd = get_fd();
    std::vector<std::string> inode_information = print_inode(fd, num_of_inode);

    // here function to receive parameters
    ui->i_mode->setText(QString::fromStdString(inode_information[0]));
    ui->i_uid->setText(QString::fromStdString(inode_information[1]));
    ui->i_size->setText(QString::fromStdString(inode_information[2]));
    ui->i_atime->setText(QString::fromStdString(inode_information[3]));
    ui->i_ctime->setText(QString::fromStdString(inode_information[4]));
    ui->i_mtime->setText(QString::fromStdString(inode_information[5]));
    ui->i_dtime->setText(QString::fromStdString(inode_information[6]));
    ui->i_gid->setText(QString::fromStdString(inode_information[7]));
    ui->i_links_count->setText(QString::fromStdString(inode_information[8]));
    ui->i_blocks->setText(QString::fromStdString(inode_information[9]));
    ui->i_flags->setText(QString::fromStdString(inode_information[10]));
    ui->i_generation->setText(QString::fromStdString(inode_information[11]));
    ui->i_file_acl->setText(QString::fromStdString(inode_information[12]));
    ui->i_faddr->setText(QString::fromStdString(inode_information[13]));
    ui->Inode_Num->setText(QString::fromStdString(std::to_string(num_of_inode)));

    std::string content = print_inode_content(fd, num_of_inode);

    ui->inode_content->setText(QString::fromStdString(content));

}

InodeTableOneInode::~InodeTableOneInode()
{
    delete ui;
}




void InodeTableOneInode::on_SubmitChange_clicked()
{

    int fd = get_fd();

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
    std::string i_generation = ui->i_generation->text().toStdString();
    std::string i_file_acl = ui->i_file_acl->text().toStdString();
    std::string i_faddr = ui->i_faddr->text().toStdString();

    int inode_number = stoi(ui->Inode_Num->text().toStdString());
    // function send

    change_mode(fd,inode_number, stoi(i_mode));
    change_uid(fd,inode_number, stoi(i_uid));
    change_size(fd,inode_number, stoi(i_size));
    change_atime(fd,inode_number, stoi(i_atime));
    change_ctime(fd,inode_number, stoi(i_ctime));
    change_mtime(fd,inode_number, stoi(i_mtime));
    change_dtime(fd,inode_number, stoi(i_dtime));
    change_gid(fd,inode_number, stoi(i_gid));
    change_flags(fd, inode_number, stoi(i_flags));

}


void InodeTableOneInode::on_SubmitDataBlock_clicked()
{
    std::string data_block_num = ui->data_block_num->text().toStdString();
    // parse value to function
    std::string inode_num = ui->Inode_Num->text().toStdString();
    int fd = get_fd();
    append_block(fd, stoi(inode_num), stoi(data_block_num));
}


void InodeTableOneInode::on_SubmitDataDelete_clicked()
{
    int fd = get_fd();
    std::string inode_num = ui->Inode_Num->text().toStdString();
    delete_block(fd, stoi(inode_num));

}


void InodeTableOneInode::on_CopyInode_clicked()
{
    // copy inode function
}


void InodeTableOneInode::on_MoveInode_clicked()
{
    // move inode
}

