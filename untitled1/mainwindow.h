
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static Ui::MainWindow *UI;
private slots:
    void on_calculateBtn_clicked();

    void on_startSetBtn_clicked();

    void on_endSetBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
