#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), label("Hello")
{
    ui->setupUi(this); 
//    this->setCentralWidget(&label);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(grab()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save()
{
    ui->textEdit->append(QString("Saved"));
}

void MainWindow::grab()
{
    std::cout << ui->textEdit->toPlainText().toStdString() << std::endl;
}
