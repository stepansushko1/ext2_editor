#ifndef GROUPDESCRIPTORS_H
#define GROUPDESCRIPTORS_H

#include <QDialog>

namespace Ui {
class GroupDescriptors;
}

class GroupDescriptors : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDescriptors(QWidget *parent = nullptr);
    ~GroupDescriptors();

private:
    Ui::GroupDescriptors *ui;
};

#endif // GROUPDESCRIPTORS_H
