#include "mainwindow.h"
#include "ui_mainwindow.h"

bool estadoPrograma = false; // Começa desligado

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setVisible(estadoPrograma);
    ui->label_2->setVisible(estadoPrograma);
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readSerial);

    // Colocar a porta que esta o arduino
    serial.setPortName("COMX");
    serial.setBaudRate(QSerialPort::Baud9600);
    if(!serial.open(QIODevice::ReadWrite)) {
        qDebug() << "Não foi possível abrir a porta serial!";
    }

    ui->spinBox->setRange(15, 30); // Configura o intervalo mínimo e máximo de temperatura
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
}

MainWindow::~MainWindow()
{
    if(serial.isOpen()) {
        serial.close();
    }
    delete ui;
}

void MainWindow::on_radioButton_clicked()
{
    // As opções de temperaturas so aparecerem quando o programa "estiver ligado"
    estadoPrograma = ui->radioButton->isChecked();
    ui->spinBox->setVisible(estadoPrograma);
    ui->label_2->setVisible(estadoPrograma);

    QByteArray command = estadoPrograma ? "L\n" : "D\n";
    if(serial.isOpen()) {
        serial.write(command);
    }
}

void MainWindow::onSpinBoxValueChanged(int value)
{
    QString command = QString::number(value) + "\n";
    if(serial.isOpen()) {
        serial.write(command.toUtf8());
    }
}

void MainWindow::readSerial()
{
    // Colocar a temperatura do sensor LM35 no display de qtlcd
    QByteArray serialData = serial.readAll();
    QString dataStr = QString::fromUtf8(serialData).trimmed();

    if(dataStr.startsWith("T")) {
        bool ok;
        double temperatura = dataStr.mid(1).toDouble(&ok);
        if(ok) {
            ui->lcdNumber->display(temperatura);
        }
    }
}

void MainWindow::on_label_2_linkActivated(const QString &link) {
}

void MainWindow::on_lcdNumber_overflow() {
}
