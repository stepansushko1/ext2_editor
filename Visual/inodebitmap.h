#ifndef INODEBITMAP_H
#define INODEBITMAP_H

#include <QDialog>

namespace Ui {
class InodeBitmap;
}

class InodeBitmap : public QDialog
{
    Q_OBJECT

public:
    explicit InodeBitmap(QWidget *parent = nullptr);
    ~InodeBitmap();

private:
    Ui::InodeBitmap *ui;
};

#endif // INODEBITMAP_H
