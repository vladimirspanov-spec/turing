#ifndef TURING_H
#define TURING_H

#include <QWidget>
#include <QLineEdit>
#include <string>
#include <QVBoxLayout>
#include <QVector>
#include <QTimer>

class turing : public QWidget
{
    Q_OBJECT
public:
    turing(std::string alphabet, std::string extrasymbols);
    ~turing() {
        delete header;
        delete al_size;
        delete q_header;
        delete stopped;
    }
private slots:
    void setNewStr();
    void add_q();
    void delete_q();
    void step();
    void play();
    void stop();
private:
    QLineEdit *Alphabet;
    QLineEdit *ExtraSymbols;
    QLineEdit *ribbon;
    QLineEdit *str;
    int *header;
    int *q_header;
    int *al_size;
    int *stopped;
    QVector<QLineEdit*> *table;
    QVector<QHBoxLayout*> *lines;
    QLineEdit *table_title;
    QTimer *timer;

};

#endif // TURING_H
