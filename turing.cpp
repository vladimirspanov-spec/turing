#include "turing.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <map>
#include <sstream>
#include "mainwindow.h"

QVBoxLayout *table_layout = nullptr;
std::map<int, char> symbols;
std::map<char, int> rsymbols;

turing::turing(std::string alphabet, std::string extrasymbols) {
    if (table_layout) {
        delete table_layout;
        table_layout = nullptr;
    }
    symbols.clear();
    rsymbols.clear();

    table_layout = new QVBoxLayout;
    this->setFixedSize(700, 545);
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
    set_str = new QPushButton("Задать строку");
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

    QHBoxLayout* arrow_layout = new QHBoxLayout;
    arrow_layout->setSpacing(0);
    arrow_layout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 17; ++i) {
        arrowCells[i] = new QLabel;
        arrowCells[i]->setFixedHeight(18);
        arrow_layout->addWidget(arrowCells[i]);
    }
    main_layout->addLayout(arrow_layout);

    arrowFloat = new QLabel("↑", this);
    arrowFloat->setAlignment(Qt::AlignCenter);
    arrowFloat->setStyleSheet("color: #00EEAA; font-size: 14px; font-weight: bold;");
    arrowFloat->setAttribute(Qt::WA_TransparentForMouseEvents);
    arrowFloat->hide();

    arrowAnim = new QPropertyAnimation(arrowFloat, "geometry", this);
    arrowAnim->setDuration(100);
    arrowAnim->setEasingCurve(QEasingCurve::OutCubic);

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


    //QPushButton *plusq, *minusq;
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
    removeSpeed = new QPushButton("Понизить скорость");
    addSpeed = new QPushButton("Повысить скорость");
    QHBoxLayout *speed = new QHBoxLayout;
    speed->addWidget(removeSpeed); speed->addWidget(addSpeed);
    connect(removeSpeed, &QPushButton::clicked, this, &turing::decreaseSpeed);
    connect(addSpeed, &QPushButton::clicked, this, &turing::increaseSpeed);
    main_layout->addLayout(speed);
    changeAlphabet = new QPushButton("Изменить алфавит");
    main_layout->addWidget(changeAlphabet);
    connect(changeAlphabet, &QPushButton::clicked, this, &turing::change_alphabet);

    statusLabel = new QLabel;
    statusLabel->setStyleSheet("color: red; font-size: 11px;");
    statusLabel->setWordWrap(true);
    statusLabel->setFixedHeight(20);
    main_layout->addWidget(statusLabel);

    setLayout(main_layout);
    connect(set_str, &QPushButton::clicked, this, &turing::setNewStr);
}

void turing::freezeTable(bool freeze) {
    // Замораживаем/размораживаем кнопки управления таблицей
    plusq->setDisabled(freeze);
    minusq->setDisabled(freeze);
    set_str->setDisabled(freeze);
    changeAlphabet->setDisabled(freeze);

    // Замораживаем/размораживаем все ячейки таблицы (кроме заголовков и состояний)
    for (int i = 0; i < table->size(); ++i) {
        QLineEdit* cell = table->at(i);
        // Проверяем, что это не ячейка с состоянием (они всегда ReadOnly)
        if (!cell->text().startsWith('q')) {
            cell->setReadOnly(freeze);
        }
    }

    // Изменяем стиль при заморозке
    /*if (freeze) {
        for (int i = 0; i < table->size(); ++i) {
            QLineEdit* cell = table->at(i);
            if (!cell->text().startsWith('q')) {
                cell->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");
            }
        }
    } else {
        for (int i = 0; i < table->size(); ++i) {
            QLineEdit* cell = table->at(i);
            if (!cell->text().startsWith('q')) {
                cell->setStyleSheet("");
            }
        }
    }*/
}

void turing::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    if (*header != -1) {
        moveArrowTo(*header, false);
        arrowFloat->show();
    }
}

void turing::moveArrowTo(int cellIndex, bool animate) {
    QPoint cellPos = arrowCells[cellIndex]->mapTo(this, QPoint(0, 0));
    QRect target(cellPos.x(), cellPos.y() - 8, arrowCells[cellIndex]->width(), arrowCells[cellIndex]->height());
    if (animate) {
        arrowAnim->stop();
        arrowAnim->setStartValue(arrowFloat->geometry());
        arrowAnim->setEndValue(target);
        arrowAnim->start();
    } else {
        arrowFloat->setGeometry(target);
    }
    arrowFloat->raise();
}

bool turing::validateCommand(const QString& cmd) {
    QString trimmed = cmd.trimmed();
    if (trimmed.isEmpty()) return true;

    std::string s = trimmed.toStdString();
    std::stringstream ss(s);
    std::string token;
    if (!(ss >> token)) return true;

    // optional symbol: single char not being < > !
    if (token.size() == 1 && token != "<" && token != ">" && token != "!") {
        if (rsymbols.find(token[0]) == rsymbols.end()) return false;
        if (!(ss >> token)) return true;
    }

    // optional direction
    if (token == "<" || token == ">") {
        if (!(ss >> token)) return true;
    }

    // optional state or stop
    if (token == "!") return true;
    if (token.size() >= 2 && token[0] == 'q') {
        try {
            int q = std::stoi(token.substr(1));
            return q >= 0 && q < lines->size();
        } catch (...) {
            return false;
        }
    }

    return false;
}

bool turing::validateAll() {
    bool ok = true;
    int rowSize = *al_size + 1;
    for (int i = 0; i < table->size(); ++i) {
        if (i % rowSize == 0) continue;
        QLineEdit* cell = table->at(i);
        if (!validateCommand(cell->text())) {
            cell->setStyleSheet("QLineEdit { border: 2px solid red; background: #ffe0e0; }");
            ok = false;
        } else {
            cell->setStyleSheet("");
        }
    }
    statusLabel->setText(ok ? "" : "Ошибка: некорректные команды выделены красным");
    return ok;
}

void turing::setNewStr() {
    QString inputStr = str->text();
    for (QChar c : inputStr) {
        if (rsymbols.find(c.toLatin1()) == rsymbols.end()) {
            str->setStyleSheet("QLineEdit { border: 2px solid red; }");
            statusLabel->setText(QString("Ошибка: символ '") + c + "' не входит в алфавит");
            return;
        }
    }
    str->setStyleSheet("");
    statusLabel->setText("");

    freezeTable(false);
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
    table->at(*q_header)->setStyleSheet("");
    *q_header = 0;
    table->at(0)->setStyleSheet("QLineEdit { border: 2px solid #0000AA }");
    *vector_header = 8 + 128;
    arrowFloat->show();
    moveArrowTo(8, false);
    *stopped = 0;
}

void turing::add_q() {
    std::stringstream ss;
    qDebug() << table->size();
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
}

void turing::step() {
    if (*header == -1) { qDebug() << "no"; return; }
    if (*stopped) { qDebug() << "machine stopped"; return; }
    static bool firstStep = true;
    if (firstStep) {
        if (!validateAll()) return;
        freezeTable(true);
        firstStep = false;
    }
    char sym = *ribbon[*header].text().toLatin1();
    if (rsymbols[sym] == 0) { return; }
    std::string s = table->at(rsymbols[sym] + *q_header)->text().toStdString(), new_sym, dir, new_q;
    std::stringstream ss(s);
    std::string s1;
    ss >> s1;
    if (s1 == "") {
        *stopped = 1;
        timer->stop();
        freezeTable(false);
        qDebug() << "Машина остановлена";
        firstStep = true;
    }
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


    int oldHeader = *header;
    if (dir == ">") {
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
        moveArrowTo(*header, qAbs(*header - oldHeader) <= 1);
    } else if(dir =="<") {
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
        moveArrowTo(*header, qAbs(*header - oldHeader) <= 1);
    }

    if (new_q == "!") {
        *stopped = 1;
        timer->stop();
        freezeTable(false);
        qDebug() << "Машина остановлена";
        firstStep = true;
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
    if (!validateAll()) return;
    if (timer->isActive()) {
        timer->stop();
    }
    freezeTable(true);
    timer->start();
    qDebug() << "Запуск машины";
}

void turing::stop() {
    if (timer->isActive()) {
        timer->stop();
        qDebug() << "Машина остановлена";
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
        qDebug() << "макс";
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

void turing::change_alphabet() {
    // Останавливаем таймер
    if (timer->isActive()) {
        timer->stop();
    }

    // Закрываем текущее окно с удалением
    this->close();

    // Создаем новое главное окно
    MainWindow *m = new MainWindow;
    m->show();
}
