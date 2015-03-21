#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QtSerialPort/QSerialPortInfo>
#include "backglow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update();
    void engage();

private:
    void readSettings();
    void saveSettings();

    Ui::MainWindow *ui;

    QList<QSerialPortInfo> list;
    BackGlow *ambipixel = nullptr;

    bool running = false;
    QTimer *timer;
    QTime   time;
    int     last_time = 0;
};

#endif // MAINWINDOW_H
