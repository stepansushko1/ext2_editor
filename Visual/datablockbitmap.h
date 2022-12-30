#ifndef DATABLOCKBITMAP_H
#define DATABLOCKBITMAP_H

#include <QDialog>

namespace Ui {
class DataBlockBitmap;
}

class DataBlockBitmap : public QDialog
{
    Q_OBJECT

public:
    explicit DataBlockBitmap(QWidget *parent = nullptr);
    ~DataBlockBitmap();

private slots:
    void on_SendChangesDataBlock_clicked();

private:
    Ui::DataBlockBitmap *ui;
};

#endif // DATABLOCKBITMAP_H
