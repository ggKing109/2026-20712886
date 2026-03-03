#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    QString getName() const;
    int getR() const;
    int getG() const;
    int getB() const;
    bool getVisible() const;
    void setValues(const QString &name, int r, int g, int b, bool visible);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
