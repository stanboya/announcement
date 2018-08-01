#include <QMessageBox>

#include "add_entry_dialog.h"
#include "ui_add_entry_dialog.h"

add_entry_dialog::add_entry_dialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::add_entry_dialog) {
    ui->setupUi(this);
}

add_entry_dialog::~add_entry_dialog() {
    delete ui;
}

void add_entry_dialog::on_pushButton_clicked() {
    QMessageBox::information(this, tr("Belief Announcement Solver"), tr("Example help message"));
}
