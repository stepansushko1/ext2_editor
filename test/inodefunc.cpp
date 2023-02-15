#include "inodefunc.h"
#include "ui_inodefunc.h"
#include <string.h>
#include "funtional_part.h"

InodeFunc::InodeFunc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InodeFunc)
{
    ui->setupUi(this);
}

InodeFunc::~InodeFunc()
{
    delete ui;
}

void InodeFunc::on_CopyButton_clicked()
{
    std::string copy_files = ui->copy_info->text().toStdString();

    std::string copy_from;
    std::string destination;
    int checker = 0;

    for (const auto & ch : copy_files){

        if (ch == ','){
            checker++;
            continue;
        }

        if (checker == 0){
            copy_from += ch;
        }
        else
            destination += ch;

    }

    int fd = get_fd();

    copy_inode_content_to_another_inode(fd, copy_from, destination);


}

