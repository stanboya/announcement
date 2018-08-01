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
    add_entry_dialog d;
    d.exec();
}

void MainWindow::on_remove_entry_clicked() {
    std::cout << "\n";
}

void MainWindow::on_calculate_annoouncement_clicked() {
    std::cout << "\n";
}
