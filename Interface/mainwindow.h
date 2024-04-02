#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSpinBoxValueChanged(int value); // Adicione esta linha

    void on_radioButton_clicked();

    void readSerial();

    void on_label_2_linkActivated(const QString &link);

    void on_lcdNumber_overflow();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
};

#endif // MAINWINDOW_H
