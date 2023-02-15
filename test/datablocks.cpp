#include "datablocks.h"
#include "ui_datablocks.h"
#include "QVBoxLayout"
#include "QScrollBar"
#include "datablocksingle.h"
#include "QPushButton"

DataBlocks::DataBlocks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataBlocks)
{
    ui->setupUi(this);

    auto * layout = new QVBoxLayout(this);
    auto * scrollArea = ui->scrollAreaDataBlocks;
    scrollArea->setWidgetResizable( true );
    layout->addWidget( scrollArea);
    auto * container = new QWidget();
    scrollArea->setWidget( container );
    layout = new QVBoxLayout(container);
    scrollArea->verticalScrollBar()->setEnabled(true);
    scrollArea->horizontalScrollBar()->setEnabled(false);

//    std::vector<QPushButton *> buttons_vector;

    int num_of_datablocks = 10;
    for (int i = 0; i < num_of_datablocks; ++i){
        DataBlockSingle *data_block_single = new DataBlockSingle(this, i);


        auto * temp_button = new QPushButton( "Data Block " + QString::number(i), container);
        temp_button->resize(100,100);
        temp_button->setObjectName("DataBlockNum" + QString::number(i));

//        buttons_vector.push_back(temp_button);
        layout->addWidget( temp_button );
        connect(temp_button, SIGNAL(clicked()), data_block_single, SLOT(show()));
    }

}

DataBlocks::~DataBlocks()
{
    delete ui;
}
