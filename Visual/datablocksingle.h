#ifndef DATABLOCKSINGLE_H
#define DATABLOCKSINGLE_H

#include <QDialog>

namespace Ui {
class DataBlockSingle;
}

class DataBlockSingle : public QDialog
{
    Q_OBJECT

public:
    explicit DataBlockSingle(QWidget *parent = nullptr, int num_data_block = 0);
    ~DataBlockSingle();

private:
    Ui::DataBlockSingle *ui;
};

#endif // DATABLOCKSINGLE_H
