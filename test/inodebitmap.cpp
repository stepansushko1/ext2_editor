#include "inodebitmap.h"
#include "ui_inodebitmap.h"
#include "funtional_part.h"

InodeBitmap::InodeBitmap(QWidget *parent) : QDialog(parent), ui(new Ui::InodeBitmap) {
    ui->setupUi(this);

    int fd = get_fd();

    std::string ib = inode_bitmap(fd);

    std::string out_ib = transform(ib);

    ui->InodeBitmapText->setText(QString::fromStdString(out_ib));
}

InodeBitmap::~InodeBitmap()
{
    delete ui;
}
