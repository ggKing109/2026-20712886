#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}
QString OptionDialog::getName() const { return ui->nameLineEdit->text(); }
int OptionDialog::getR() const { return ui->rspinBox->value(); }
int OptionDialog::getG() const { return ui->gspinBox->value(); }
int OptionDialog::getB() const { return ui->bspinBox->value(); }
bool OptionDialog::getVisible() const { return ui->visibleCheckBox->isChecked(); }

void OptionDialog::setValues(const QString &name, int r, int g, int b, bool visible)
{
    ui->nameLineEdit->setText(name);
    ui->rspinBox->setValue(r);
    ui->gspinBox->setValue(g);
    ui->bspinBox->setValue(b);
    ui->visibleCheckBox->setChecked(visible);
}
