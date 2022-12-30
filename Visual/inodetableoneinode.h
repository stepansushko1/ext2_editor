#ifndef INODETABLEONEINODE_H
#define INODETABLEONEINODE_H

#include <QDialog>

namespace Ui {
class InodeTableOneInode;
}

class InodeTableOneInode : public QDialog
{
    Q_OBJECT

public:
    explicit InodeTableOneInode(QWidget *parent = nullptr, int num_of_inode = 0);
    ~InodeTableOneInode();


private slots:
    void on_SubmitChange_clicked();

private:
    Ui::InodeTableOneInode *ui;
};

#endif // INODETABLEONEINODE_H
