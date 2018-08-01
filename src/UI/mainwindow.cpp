#include <QInputDialog>
#include <QDir>
#include <QTableWidgetItem>
#include <iostream>

#include "add_entry_dialog.h"
#include "mainwindow.h"
#include "ui_add_entry_dialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_add_entry_clicked() {
    bool ok;
    QString beliefs = QInputDialog::getText(this, tr("Initial Agent Beliefs"), tr("Initial Beliefs:"),
        QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok && !beliefs.isEmpty()) {
        std::cout << beliefs.toStdString() << "\n";
    }
    QString goals = QInputDialog::getText(this, tr("Desired Agent Goal"), tr("Desired Goal:"),
        QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok && !goals.isEmpty()) {
        std::cout << goals.toStdString() << "\n";
    }

    std::cout << "Done info gathering\n";

    ui->data_table->insertRow(ui->data_table->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setCheckState(Qt::Unchecked);
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 0, item);
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(7)));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 2, new QTableWidgetItem(beliefs));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 3, new QTableWidgetItem(goals));
}

void MainWindow::on_remove_entry_clicked() {
    QItemSelectionModel *selectionModel = ui->data_table->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.size() > 0) {
        // There is at lease one selected row.
        std::cout << "Have something to remove\n";
    }
}

void MainWindow::on_calculate_annoouncement_clicked() {
    std::cout << "\n";
}
