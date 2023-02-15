#ifndef INODEFUNCTIONS_H
#define INODEFUNCTIONS_H

#include <QWidget>

namespace Ui {
class InodeFunctions;
}

class InodeFunctions : public QWidget
{
    Q_OBJECT

public:
    explicit InodeFunctions(QWidget *parent = nullptr);
    ~InodeFunctions();

private:
    Ui::InodeFunctions *ui;
};

#endif // INODEFUNCTIONS_H
