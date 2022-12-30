#ifndef INODETABLE_H
#define INODETABLE_H

#include <QDialog>

namespace Ui {
class InodeTable;
}

class InodeTable : public QDialog
{
    Q_OBJECT

public:
    explicit InodeTable(QWidget *parent = nullptr);
    ~InodeTable();

private:
    Ui::InodeTable *ui;
};

#endif // INODETABLE_H
