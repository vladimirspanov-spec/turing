#include "turing.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <map>
#include <sstream>

QVBoxLayout *table_layout = new QVBoxLayout;
std::map<int, char> symbols;
std::map<char, int> rsymbols;

turing::turing(std::string alphabet, std::string extrasymbols) {
    this->setFixedSize(700, 500);
    header = new int;
    vector_header = new int;
    *header = -1;
    *vector_header = -1;

    timer = new QTimer(this); //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
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &turing::step);
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
    ribbon_values = new QVector<QString*>;
    for (int i = 0; i < 512; ++i) {
        ribbon_values->push_back(new QString("^"));
    } //128 - 0
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


    int x = 0;
    for (unsigned i = 0; i < alphabet.size(); ++i) {
        symbols[x] = alphabet[i];
        rsymbols[alphabet[i]] = x + 1;
        x++;
    }
    symbols[x] = '^';
    rsymbols['^'] = x+1;
    x++;
    for (unsigned i = 0; i < extrasymbols.size(); ++i) {
        symbols[x] = extrasymbols[i];
        rsymbols[extrasymbols[i]]= x + 1;
        x++;
    }
    al_size = new int;
    *al_size = x;

    table_title = new QLineEdit[x+1];
    QHBoxLayout *table_title_layout = new QHBoxLayout;
    //тут был table_layout
    table_title[0].setText("Состояние");
    table_title[0].setReadOnly(true);
    table_title_layout->addWidget(&table_title[0]);
    for (int i = 1; i <= x; ++i) {
        table_title[i].setText(QString(symbols[i-1]));
        table_title[i].setReadOnly(true);
        table_title_layout->addWidget(&table_title[i]);
    }
    table_layout->setSpacing(0);
    table_layout->setContentsMargins(0, 0, 0, 0);
    table_layout->addLayout(table_title_layout);


    table = new QVector<QLineEdit*>;
    lines = new QVector<QHBoxLayout*>;
    lines->append(new QHBoxLayout);
    q_header = new int;
    *q_header = 0;


    table->append(new QLineEdit("q0"));
    table->at(0)->setReadOnly(true);
    table->at(0)->setStyleSheet("QLineEdit { border: 2px solid #0000AA }");
    lines->at(0)->addWidget(table->at(0));
    for (int i = 1; i <= x; ++i) {
        table->append(new QLineEdit(""));
        lines->at(0)->addWidget(table->at(i));
    }
    table_layout->addLayout(lines->at(0));


    QPushButton *plusq, *minusq;
    plusq = new QPushButton("+");
    minusq = new QPushButton("-");
    connect(plusq, &QPushButton::clicked, this, &turing::add_q);
    connect(minusq, &QPushButton::clicked, this, &turing::delete_q);
    QHBoxLayout *add_q_layout = new QHBoxLayout;
    add_q_layout->addWidget(plusq); add_q_layout->addWidget(minusq);


    main_layout->addLayout(table_layout);
    main_layout->addLayout(add_q_layout);


    QPushButton *play_button = new QPushButton("Запустить");
    QPushButton *step_button = new QPushButton("Шаг");
    QPushButton *stop_button = new QPushButton("Стоп");
    QHBoxLayout *play_layout = new QHBoxLayout;
    play_layout->addWidget(play_button); play_layout->addWidget(step_button); play_layout->addWidget(stop_button);
    main_layout->addLayout(play_layout);
    connect(step_button, &QPushButton::clicked, this, &turing::step);
    connect(play_button, &QPushButton::clicked, this, &turing::play);
    connect(stop_button, &QPushButton::clicked, this, &turing::stop);
    stopped = new int;
    *stopped = 0;

    time = new int;
    *time = 1000;
    QPushButton *removeSpeed = new QPushButton("Понизить скорость");
    QPushButton *addSpeed = new QPushButton("Повысить скорость");
    QHBoxLayout *speed = new QHBoxLayout;
    speed->addWidget(removeSpeed); speed->addWidget(addSpeed);
    connect(removeSpeed, &QPushButton::clicked, this, &turing::decreaseSpeed);
    connect(addSpeed, &QPushButton::clicked, this, &turing::increaseSpeed);
    main_layout->addLayout(speed);

    setLayout(main_layout);
    connect(set_str, &QPushButton::clicked, this, &turing::setNewStr);
}

void turing::setNewStr() {
    for (int i = 0; i < 17; i++) {
        ribbon[i].setText("^");
        ribbon[i].setStyleSheet("");
    }
    for (int i = 0; i < 512; ++i) {
        *(ribbon_values->at(i)) = "^";
    } //128 - 0
    std::string s = str->text().toStdString();
    int x = 8;
    int j = 0;
    for (unsigned long i = x; i < x + s.size() && i < 17; ++i) {
        ribbon[i].setText(QString(str->text().at(j)));
        ++j;
        //qDebug() << "lol";
    }
    j = 0;
    for (unsigned long i = 128 + x; i < 128 + x + s.size(); ++i) {
        *(ribbon_values->at(i)) = QString(str->text().at(j));
        ++j;
        //qDebug() << "lol";
    }
    qDebug() << j;
    *header = 8;
    *vector_header = 8 + 128;
    ribbon[8].setStyleSheet("QLineEdit { border: 2px solid #00EEAA }");
    *stopped = 0;
}

void turing::add_q() {
    std::stringstream ss;
    qDebug() << *al_size;
    int index = lines->size();
    ss << 'q' << index;
    lines->append(new QHBoxLayout);
    table->append(new QLineEdit(QString::fromStdString(ss.str())));
    table->at(table->size()-1)->setReadOnly(true);
    lines->at(index)->addWidget(table->at(table->size()-1));
    for (int i = 1; i <= *al_size; ++i) {
        table->append(new QLineEdit(""));
        lines->at(index)->addWidget(table->at(table->size()-1));
    }
    table_layout->addLayout(lines->at(index));
}


void turing::delete_q() {
    if (lines->size() <= 1) {
        qDebug() << "Нельзя удалить последнюю строку";
        return;
    }

    // Получаем последний layout
    QHBoxLayout* lastLayout = lines->last();
    if (!lastLayout) return;

    // Удаляем все виджеты из последнего layout
    while (lastLayout->count() > 0) {
        QWidget* widget = lastLayout->itemAt(0)->widget();
        if (widget) {
            lastLayout->removeWidget(widget);
            delete widget;
        }
    }

    // Удаляем последний layout из table_layout
    table_layout->removeItem(lastLayout);

    // Удаляем последний layout из вектора lines
    lines->pop_back();
    delete lastLayout;

    // Удаляем соответствующие QLineEdit из вектора table
    int itemsToRemove = *al_size + 1;
    for (int i = 0; i < itemsToRemove && !table->isEmpty(); ++i) {
        table->pop_back();
    }
    if (*q_header >= table->size()) {
        *q_header = 0;
        table->at(0)->setStyleSheet("QLineEdit { border: 2px solid #0000AA }");
    }
    qDebug() << *q_header << " " << table->size();
    qDebug() << "Осталось строк:" << lines->size();
}

void turing::step() {
    if (*header == -1) { qDebug() << "no"; return; }
    if (*stopped) { qDebug() << "machine stopped"; return; }
    char sym = *ribbon[*header].text().toLatin1();
    if (rsymbols[sym] == 0) { return; }
    std::string s = table->at(rsymbols[sym] + *q_header)->text().toStdString(), new_sym, dir, new_q;
    std::stringstream ss(s);
    std::string s1;
    ss >> s1;
    if (s1.size() == 1 && s1 != "<" && s1 != ">" && s1 != "!") {
        new_sym = s1;
        ss >> s1;
    }
    if (s1 == "<" || s1 == ">") {
        dir = s1;
        ss >> s1;
    }
    if ((s1[0] == 'q' && s1.size() > 1) || s1 == "!") {
        new_q = s1;
    }

    if (new_sym != "") {
        ribbon[*header].setText(QString::fromStdString(new_sym));
        *(ribbon_values->at(*vector_header)) = QString::fromStdString(new_sym);
    }


    if (dir == ">") {
        ribbon[*header].setStyleSheet("");
        *header += 1;
        *vector_header += 1;
        if (*header >= 17) {
            for (int i = 0; i < 13; ++i) {
                ribbon[i].setText(ribbon[i+4].text());
            }
            *header -= 4;
            int j = 0;
            for (int i = 13; i < 17; ++i) {
                ribbon[i].setText(*(ribbon_values->at(*vector_header + j)));
                ++j;
            }
        }
        ribbon[*header].setStyleSheet("QLineEdit { border: 2px solid #00EEAA }");
    } else if(dir =="<") {
        ribbon[*header].setStyleSheet("");
        *header -= 1;
        *vector_header -= 1;
        if (*header < 0) {
            for (int i = 16; i > 3; --i) {
                ribbon[i].setText(ribbon[i-4].text());
            }
            *header += 4;
            int j = 0;
            for (int i = 3; i >= 0; --i) {
                ribbon[i].setText(*(ribbon_values->at(*vector_header - j)));
                ++j;
            }
        }
        ribbon[*header].setStyleSheet("QLineEdit { border: 2px solid #00EEAA }");
    }

    if (new_q == "!") {
        *stopped = 1;
        timer->stop();
    }
    else if (new_q != "") {
        new_q.erase(0, 1);
        int q = std::stoi(new_q);
        table->at(*q_header)->setStyleSheet("");
        *q_header = q * (*al_size + 1);
        table->at(*q_header)->setStyleSheet("QLineEdit { border: 2px solid #0000AA }");
        qDebug() << *q_header;
    }

    //qDebug() << QString::fromStdString(new_q);

    //qDebug() << table->at(rsymbols[sym])->text().toStdString();

}

void turing::play() {
    if (*stopped) {
        qDebug() << "Машина уже остановлена";
        return;
    }
    if (timer->isActive()) {
        timer->stop();
    }
    timer->start(); // Запускаем таймер
    qDebug() << "Запуск машины с интервалом 2 секунды";
}

void turing::stop() {
    if (timer->isActive()) {
        timer->stop();
        qDebug() << "Машина остановлена пользователем";
    }
}

void turing::increaseSpeed() {
    if (*time > 100) {
        *time -= 100;
        timer->setInterval(*time);
        qDebug() << "Скорость увеличена. Интервал:" << *time << "мс";
    } else if (*time > 50) {
        *time = 50;
        timer->setInterval(*time);
        qDebug() << "Максимальная скорость. Интервал:" << *time << "мс";
    } else {
        qDebug() << "Уже максимальная скорость";
    }
}

void turing::decreaseSpeed() {
    if (*time < 1000) {
        *time += 100;
        timer->setInterval(*time);
        qDebug() << "Скорость уменьшена. Интервал:" << *time << "мс";
    } else if (*time < 2000) {
        *time = 2000;
        timer->setInterval(*time);
        qDebug() << "Минимальная скорость. Интервал:" << *time << "мс";
    } else {
        qDebug() << "Уже минимальная скорость";
    }
}
