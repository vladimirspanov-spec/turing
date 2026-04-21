#include "turing.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>

turing::turing(std::string alphabet, std::string extrasymbols) {
    this->setFixedSize(700, 500);
    header = new int;
    *header = -1;
    /*this->Alphabet = new QLineEdit(alphabet.c_str());
    Alphabet->setReadOnly(true);
    this->ExtraSymbols = new QLineEdit(extrasymbols.c_str());
    ExtraSymbols->setReadOnly(true);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(Alphabet); layout->addWidget(ExtraSymbols);*/
    //setLayout(layout);
    QPushButton* set_str = new QPushButton("Задать строку");
    set_str->setFixedSize(250, 50);
    str = new QLineEdit;
    str->setFixedSize(250, 50); str->setPlaceholderText("Введите строку");
    QHBoxLayout* str_layout = new QHBoxLayout;
    str_layout->addWidget(str); str_layout->addWidget(set_str);
    str_layout->setSpacing(20);
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(str_layout);
    ribbon = new QLineEdit[17];
    QHBoxLayout *ribbon_layout = new QHBoxLayout;
    for (int i = 0; i < 17; ++i) {
        ribbon[i].setReadOnly(true);
        ribbon[i].setText("^");
        //ribbon[i].setFixedSize(25, 25);
        ribbon_layout->addWidget(&ribbon[i]);
    }
    ribbon_layout->setSpacing(0);
    ribbon_layout->setContentsMargins(0, 0, 0, 0);
    //ribbon[3].setText("a");
    //ribbon[3].setStyleSheet("QLineEdit { border: 2px solid #00EEAA }");
    main_layout->addLayout(ribbon_layout);
    setLayout(main_layout);
    connect(set_str, &QPushButton::clicked, this, &turing::setNewStr);
}

void turing::setNewStr() {
    std::string s = str->text().toStdString();
    int x = (17-s.size())/2;
    int j = 0;
    for (unsigned long i = x; i < x +s.size(); ++i) {
        ribbon[i].setText(QString(str->text().at(j)));
        ++j;
        qDebug() << "lol";
    }
    //qDebug() << s.size();
    *header = (17-s.size())/2;
    ribbon[(17-s.size())/2].setStyleSheet("QLineEdit { border: 2px solid #00EEAA }");
    //qDebug() << (17-s.size())/2;
}
