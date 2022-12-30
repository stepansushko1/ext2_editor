#ifndef GROUPBLOCK_H
#define GROUPBLOCK_H

#include <QDialog>

namespace Ui {
class GroupBlock;
}

class GroupBlock : public QDialog
{
    Q_OBJECT

public:
    explicit GroupBlock(QWidget *parent = nullptr);
    ~GroupBlock();

private slots:
    void on_SuperBlockButton_clicked();

    void on_GroupDescriptorsButton_clicked();

    void on_BlockBitmapButton_clicked();

    void on_InodeBitmap_clicked();

    void on_InodeTableButton_clicked();

    void on_DataBlocksButton_clicked();

private:
    Ui::GroupBlock *ui;
};

#endif // GROUPBLOCK_H
