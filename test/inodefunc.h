#ifndef INODEFUNC_H
#define INODEFUNC_H

#include <QDialog>

namespace Ui {
class InodeFunc;
}

class InodeFunc : public QDialog
{
    Q_OBJECT

public:
    explicit InodeFunc(QWidget *parent = nullptr);
    ~InodeFunc();

private slots:
    void on_CopyButton_clicked();

private:
    Ui::InodeFunc *ui;
};

#endif // INODEFUNC_H
