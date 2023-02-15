#ifndef DATABLOCKS_H
#define DATABLOCKS_H

#include <QDialog>

namespace Ui {
class DataBlocks;
}

class DataBlocks : public QDialog
{
    Q_OBJECT

public:
    explicit DataBlocks(QWidget *parent = nullptr);
    ~DataBlocks();

private:
    Ui::DataBlocks *ui;
};

#endif // DATABLOCKS_H
