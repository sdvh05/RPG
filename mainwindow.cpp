#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameWindow.h"
#include "QLabel"
#include "QProgressBar"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_B_SalirJuego_clicked()
{
    close();
}


void MainWindow::on_B_Jugar_clicked()
{

    ui->B_Jugar->setEnabled(false);

    QLabel *labelCarga = new QLabel("Cargando partida...", this);
    labelCarga->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font: bold 16pt 'Arial';"
        "    background-color: rgba(0, 0, 0, 150);"
        "}"
        );
    labelCarga->setAlignment(Qt::AlignCenter);
    labelCarga->setGeometry(
        (this->width() - 400) / 2,
        (this->height() - 100) / 2 + 220,
        400, 40
        );
    labelCarga->show();


    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setGeometry(
        (this->width() - 400) / 2,
        (this->height() - 100) / 2 + 270,
        400, 30
        );
    progressBar->setRange(0, 100);
    progressBar->setValue(0);


    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #555;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    background-color: rgba(0, 0, 0, 180);"
        "    color: white;"
        "    font: bold 12pt 'Arial';"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #00FF00;"
        "    width: 20px;"
        "}"
        );
    progressBar->show();


    QTimer *timer = new QTimer(this);
    int *progressValue = new int(0);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        *progressValue += 2;
        progressBar->setValue(*progressValue);

        if (*progressValue >= 100)
        {
            timer->stop();

            progressBar->hide();
            delete progressBar;
            labelCarga->hide();
            delete labelCarga;
            delete progressValue;

            this->hide();

            GameWindow *gameWindow = new GameWindow(this);
            gameWindow->show();

            this->show();
            ui->B_Jugar->setEnabled(true);
        }
    });

    timer->start(100);
}
