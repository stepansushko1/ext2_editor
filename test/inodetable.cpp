#include "inodetable.h"
#include "ui_inodetable.h"
#include "inodetableoneinode.h"
#include "QBoxLayout"
#include "QScrollBar"
#include "QPushButton"

#include "funtional_part.h"


InodeTable::InodeTable(QWidget *parent) : QDialog(parent), ui(new Ui::InodeTable){
    ui->setupUi(this);

    auto * layout = new QVBoxLayout(this);
    auto * scrollArea = ui->scrollAreaInodeTable;
    scrollArea->setWidgetResizable( true );
    layout->addWidget( scrollArea);
    auto * container = new QWidget();
    scrollArea->setWidget( container );
    layout = new QVBoxLayout(container);
    scrollArea->verticalScrollBar()->setEnabled(true);
    scrollArea->horizontalScrollBar()->setEnabled(false);

//    std::vector<QPushButton *> buttons_vector;

//    int fd = get_fd();

//    std::string counter = print_super(fd)[0];

    int inode_num = 100;
    for (int i = 0; i < inode_num; ++i){
        InodeTableOneInode *inode_table = new InodeTableOneInode(this, i);


        auto * temp_button = new QPushButton( "Inode " + QString::number(i), container);
        temp_button->resize(100,100);
        temp_button->setObjectName("InodeBlockNum" + QString::number(i));

//        buttons_vector.push_back(temp_button);
        layout->addWidget( temp_button );
        connect(temp_button, SIGNAL(clicked()), inode_table, SLOT(show()));
    }

}

InodeTable::~InodeTable()
{
    delete ui;
}

void InodeTable::on_pushButton_clicked()
{
    // function call here for pushing info somewhere else
}

