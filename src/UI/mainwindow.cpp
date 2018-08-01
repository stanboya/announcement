#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_add_entry_clicked()
{

}

void MainWindow::on_remove_entry_clicked()
{

}

void MainWindow::on_calculate_annoouncement_clicked()
{

}
