#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QtGlobal>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    list = QSerialPortInfo::availablePorts();

    for(QSerialPortInfo& port : list) {
        MainWindow::ui->portsComboBox->addItem(
                    "("+ port.portName() + ") " + port.description()
                    );
    }

    readSettings();

    timer = new QTimer(this);
    timer->setInterval(33);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(MainWindow::ui->runPushButton, SIGNAL(clicked()), this, SLOT(engage()));
    timer->start();
    time.start();
}

MainWindow::~MainWindow()
{
    delete timer;
    if(ambipixel) delete ambipixel;
    saveSettings();
    delete ui;
}

void MainWindow::readSettings() {
    QSettings settings("WiktorChomik", "BackGlow");
    MainWindow::ui->brightnessSlider->setValue(settings.value("brightnes",1000).toInt());
    MainWindow::ui->redSlider->setValue(settings.value("redIntensity",1000).toInt());
    MainWindow::ui->greenSlider->setValue(settings.value("greenIntensity",1000).toInt());
    MainWindow::ui->blueSlider->setValue(settings.value("blueIntensity",1000).toInt());
    MainWindow::ui->depthSlider->setValue(settings.value("depth",200).toInt());
}

void MainWindow::saveSettings() {
    QSettings settings("WiktorChomik", "BackGlow");
    settings.setValue("brightnes",      MainWindow::ui->brightnessSlider->value());
    settings.setValue("redIntensity",   MainWindow::ui->redSlider->value());
    settings.setValue("greenIntensity", MainWindow::ui->greenSlider->value());
    settings.setValue("blueIntensity",  MainWindow::ui->blueSlider->value());
    settings.setValue("depth",          MainWindow::ui->depthSlider->value());
}

void MainWindow::engage() {
    if(running) {
        if(ambipixel) {
            delete ambipixel;
            ambipixel = nullptr;
        }
    } else {
        int index = MainWindow::ui->portsComboBox->currentIndex();
        if(index < 0) {
            qDebug() << "Unknown serial port!";
            return;
        }
        ambipixel = new BackGlow((const char *)(list[index].portName().utf16()));
    }
    running = !running;
}

void MainWindow::update()
{
    if(!running)
        return;

    int this_time = time.elapsed();
    if(this_time != last_time) {
        MainWindow::ui->counterLcdNumber->display(1000/(this_time - last_time));
    } else {
        MainWindow::ui->counterLcdNumber->display(-1);
    }
    last_time = this_time;

    if(ambipixel) {
        ambipixel->brightnes      = MainWindow::ui->brightnessSlider->value() / 1000.0f;
        ambipixel->redIntensity   = MainWindow::ui->redSlider->value() / 1000.0f;
        ambipixel->greenIntensity = MainWindow::ui->greenSlider->value() / 1000.0f;
        ambipixel->blueIntensity  = MainWindow::ui->blueSlider->value() / 1000.0f;
        ambipixel->depth          = MainWindow::ui->depthSlider->value();
        ambipixel->process();
    }
    //MainForm::ui->text1->setText(...)
}
