#ifndef SUPERBLOCKWINDOW_H
#define SUPERBLOCKWINDOW_H

#include <QDialog>

namespace Ui {
class SuperBlockWindow;
}

class SuperBlockWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SuperBlockWindow(QWidget *parent = nullptr);
    ~SuperBlockWindow();


private:
    Ui::SuperBlockWindow *ui;
};

#endif // SUPERBLOCKWINDOW_H
