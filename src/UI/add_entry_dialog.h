#ifndef ADD_ENTRY_DIALOG_H
#define ADD_ENTRY_DIALOG_H

#include <QDialog>

namespace Ui {
    class add_entry_dialog;
}

class add_entry_dialog : public QDialog {
    Q_OBJECT

public:
    explicit add_entry_dialog(QWidget *parent = nullptr);
    ~add_entry_dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::add_entry_dialog *ui;
};

#endif // ADD_ENTRY_DIALOG_H
