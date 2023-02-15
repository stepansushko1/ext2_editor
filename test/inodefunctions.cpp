#include "inodefunctions.h"
#include "ui_inodefunctions.h"

InodeFunctions::InodeFunctions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InodeFunctions)
{
    ui->setupUi(this);
}

InodeFunctions::~InodeFunctions()
{
    delete ui;
}
