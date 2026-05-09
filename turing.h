#ifndef TURING_H
#define TURING_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPropertyAnimation>
#include <string>
#include <QVBoxLayout>
#include <QVector>
#include <QTimer>
#include <QPushButton>

class turing : public QWidget
{
    Q_OBJECT
public:
    turing(std::string alphabet, std::string extrasymbols);
    ~turing() {
        delete header;
        delete vector_header;
        delete q_header;
        delete al_size;
        delete stopped;
        delete time;
        delete[] ribbon;
        delete ribbon_values;
        delete table;
        delete lines;
        delete[] table_title;
    }
private slots:
    void setNewStr();
    void add_q();
    void delete_q();
    void step();
    void play();
    void stop();
    void increaseSpeed();
    void decreaseSpeed();
    void change_alphabet();
private:
    void freezeTable(bool freeze);
    void moveArrowTo(int cellIndex, bool animate = true);
    void showEvent(QShowEvent* event) override;
    bool validateCommand(const QString& cmd);
    bool validateAll();
    QLineEdit *Alphabet;
    QLineEdit *ExtraSymbols;
    QLineEdit *ribbon;
    QLineEdit *str;
    int *header;
    int *vector_header;
    int *q_header;
    int *al_size;
    int *stopped;
    int *time;
    QVector<QLineEdit*> *table;
    QVector<QHBoxLayout*> *lines;
    QLineEdit *table_title;
    QTimer *timer;
    QVector<QString*> *ribbon_values;
    QPushButton *plusq;
    QPushButton *minusq;
    QPushButton *set_str;
    QPushButton *changeAlphabet;
    QPushButton *removeSpeed;
    QPushButton *addSpeed;
    QLabel* arrowCells[17];
    QLabel* arrowFloat;
    QPropertyAnimation* arrowAnim;
    QLabel* statusLabel;

};

#endif // TURING_H
