#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <string>
#include "turing.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(700, 500);
    button = new QPushButton("kek");
    Alphabet = new QLineEdit;
    Alphabet->setFixedSize(200, 50); Alphabet->setPlaceholderText("Стартовый алфавит");
    ExtraSymbols = new QLineEdit;
    ExtraSymbols->setFixedSize(200, 50); ExtraSymbols->setPlaceholderText("Доп. Символы");
    button->setFixedSize(100, 50);

    QHBoxLayout *line_layout = new QHBoxLayout;
    line_layout->addWidget(Alphabet); line_layout->addWidget(ExtraSymbols);
    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(button);
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(line_layout); main_layout->addLayout(button_layout);
    setLayout(main_layout);
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    //ui->setupUi(this);
}

void MainWindow::onButtonClicked() {
    qDebug() << "kek";
    std::string alphabet = Alphabet->text().toStdString();
    std::string extra_symbol = ExtraSymbols->text().toStdString();
    turing* t = new turing(alphabet, extra_symbol);
    this->hide();
    t->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
