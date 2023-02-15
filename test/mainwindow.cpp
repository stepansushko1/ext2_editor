#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QVBoxLayout"
#include "QPushButton"
#include "vector"
#include <QScrollBar>
#include "groupblock.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "funtional_part.h"

const int num_of_blocks = 1;
std::vector<QPushButton *> buttons_vector;

//GroupBlocks
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
//    std::cout << "THIS IS FD = " << get_fd() << std::endl;
    int fd = get_fd();
    test(fd);
    auto * layout = new QVBoxLayout(this);
    auto * scrollArea = ui->GroupBlocks;
    scrollArea->setWidgetResizable( true );
    layout->addWidget(scrollArea);
    auto * container = new QWidget();
    scrollArea->setWidget( container );
    layout = new QVBoxLayout(container);
    scrollArea->verticalScrollBar()->setEnabled(true);
    scrollArea->horizontalScrollBar()->setEnabled(false);

//    std::vector<QPushButton *> buttons_vector;
    print_inode_content(fd,3);

    for (int i = 0; i < num_of_blocks; ++i){
        GroupBlock *one_group_block = new GroupBlock(this);


        auto * temp_button = new QPushButton( "Group Block " + QString::number(i), container);
        temp_button->resize(100,100);
        temp_button->setObjectName("GroupBlockNum" + QString::number(i));

        buttons_vector.push_back(temp_button);
        layout->addWidget( temp_button );
        connect(temp_button, SIGNAL(clicked()), one_group_block, SLOT(show()));
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}



//void MainWindow::on_pushButton_clicked()
//{

//    GroupBlock group_block;
//    group_block.setModal(true);
//    group_block.exec();
//}

