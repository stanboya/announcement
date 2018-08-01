#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "announce.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_entry_clicked();

    void on_remove_entry_clicked();

    void on_calculate_announcement_clicked();

private:
    Ui::MainWindow *ui;

    std::vector<agent> agent_list;
};

#endif // MAINWINDOW_H
