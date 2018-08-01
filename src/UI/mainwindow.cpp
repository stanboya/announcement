#include <QDir>
#include <QInputDialog>
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
    QString beliefs = QInputDialog::getText(this, tr("Initial Agent Beliefs"),
        tr("Initial Beliefs:"), QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (!ok || beliefs.isEmpty()) {
        return;
    }

    QString goals = QInputDialog::getText(this, tr("Desired Agent Goal"), tr("Desired Goal:"),
        QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (!ok || goals.isEmpty()) {
        return;
    }
    ui->data_table->insertRow(ui->data_table->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setCheckState(Qt::Unchecked);
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 0, item);
    ui->data_table->setItem(
        ui->data_table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(ui->data_table->rowCount())));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 2, new QTableWidgetItem(beliefs));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 3, new QTableWidgetItem(goals));
}

void MainWindow::on_remove_entry_clicked() {
    for (int i = 0; i < ui->data_table->rowCount(); ++i) {
        for (int j = 0; j < ui->data_table->columnCount(); ++j) {
            QTableWidgetItem *item = ui->data_table->item(i, j);
            if (item && item->checkState() == Qt::Checked) {
                std::cout << "Remove element at " << i << ", " << j << "\n";
                ui->data_table->removeRow(i);
            }
        }
    }
    for (int i = 0; i < ui->data_table->rowCount(); ++i) {
        ui->data_table->item(i, 1)->setText(QString::number(i + 1));
    }
}

void MainWindow::on_calculate_announcement_clicked() {
    std::cout << "\n";
}
