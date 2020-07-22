#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <iostream>
#include <sstream>
#include <vector>

#include "announce.h"
#include "interactive.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_add_entry_clicked() {
    bool ok;
    QString beliefs = QInputDialog::getText(
            this, tr("Initial Agent Beliefs"), tr("Initial Beliefs:"), QLineEdit::Normal, "", &ok);

    if (!ok || beliefs.isEmpty()) {
        QMessageBox::information(this, tr("Announcement Solver"),
                tr("Initial agent beliefs contained invalid token"));
        return;
    }

    QString goals = QInputDialog::getText(
            this, tr("Desired Agent Goal"), tr("Desired Goal:"), QLineEdit::Normal, "", &ok);

    if (!ok || goals.isEmpty()) {
        QMessageBox::information(
                this, tr("Announcement Solver"), tr("Agent goal contained invalid token"));
        return;
    }

    std::stringstream ss{shunting_yard(beliefs.toStdString())};
    std::vector<std::string> belief_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goals.toStdString())};
    std::vector<std::string> goal_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    if (belief_tokens.empty() || goal_tokens.empty()) {
        QMessageBox::information(
                this, tr("Announcement Solver"), tr("Agent beliefs and/or goal was malformed"));
        return;
    }

    agent_list.emplace_back(create_agent(belief_tokens, goal_tokens));

    ui->data_table->insertRow(ui->data_table->rowCount());
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setCheckState(Qt::Unchecked);
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 0, item);
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 1,
            new QTableWidgetItem(QString::number(ui->data_table->rowCount())));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 2, new QTableWidgetItem(beliefs));
    ui->data_table->setItem(ui->data_table->rowCount() - 1, 3, new QTableWidgetItem(goals));
}

void MainWindow::on_remove_entry_clicked() {
    for (int i = 0; i < ui->data_table->rowCount(); ++i) {
        QTableWidgetItem* item = ui->data_table->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            ui->data_table->removeRow(i);
            agent_list.erase(agent_list.begin() + i);
            //Fix weird issue with column staying put
            --i;
        }
    }
    for (int i = 0; i < ui->data_table->rowCount(); ++i) {
        ui->data_table->item(i, 1)->setText(QString::number(i + 1));
    }
}

void MainWindow::on_calculate_announcement_clicked() {
    if (!agent_list.empty()) {
        //find_announcement(agent_list);
        QMessageBox::information(
                this, tr("Announcement Solver"), tr(find_announcement_KB(agent_list).c_str()));
    } else {
        QMessageBox::information(this, tr("Announcement Solver"),
                tr("You have to enter an entry before you can find an announcement"));
    }
}

