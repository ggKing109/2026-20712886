#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QStatusBar>
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include "optiondialog.h"
#include <QDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeView->addAction(ui->actionItem_Options);

    this->partList = new ModelPartList("PartsList");

    ui->treeView->setModel(this->partList);

    ModelPart *rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart *childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }
    }

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton1);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    emit statusUpdateMessage("Button 1 was clicked", 0);
}

void MainWindow::handleButton2()
{
    OptionDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted) {
        QString msg = QString("Dialog accepted: Name=%1, RGB(%2,%3,%4), Visible=%5")
        .arg(dlg.getName())
            .arg(dlg.getR()).arg(dlg.getG()).arg(dlg.getB())
            .arg(dlg.getVisible() ? "true" : "false");

        emit statusUpdateMessage(msg, 0);
    } else {
        emit statusUpdateMessage("Dialog cancelled", 2000);
    }
}
void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage("Open File action triggered", 2000);

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("All Files (*);;STL Files (*.stl);;Text Files (*.txt)")
        );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("No file selected", 2000);
        return;
    }

    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Selected: " + fileName + " (no tree item selected)", 2000);
        return;
    }

    QString baseName = QFileInfo(fileName).fileName();
    partList->setData(index.siblingAtColumn(0), baseName, Qt::EditRole);

    emit statusUpdateMessage("Renamed selected item to: " + baseName, 2000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 2000);
        return;
    }

    QString curName = selectedPart->data(0).toString();
    bool curVisible = (selectedPart->data(1).toString().toLower() == "true");

    int curR = static_cast<int>(selectedPart->getColourR());
    int curG = static_cast<int>(selectedPart->getColourG());
    int curB = static_cast<int>(selectedPart->getColourB());

    OptionDialog dlg(this);
    dlg.setValues(curName, curR, curG, curB, curVisible);

    if (dlg.exec() != QDialog::Accepted) {
        emit statusUpdateMessage("Dialog cancelled", 2000);
        return;
    }

    partList->setData(index.siblingAtColumn(0), dlg.getName(), Qt::EditRole);
    partList->setData(index.siblingAtColumn(1),
                      dlg.getVisible() ? "true" : "false",
                      Qt::EditRole);

    selectedPart->setColour(
        static_cast<unsigned char>(dlg.getR()),
        static_cast<unsigned char>(dlg.getG()),
        static_cast<unsigned char>(dlg.getB())
        );

    emit statusUpdateMessage("Item updated", 2000);
}

