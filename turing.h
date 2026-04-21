#ifndef TURING_H
#define TURING_H

#include <QWidget>
#include <QLineEdit>
#include <string>

class turing : public QWidget
{
    Q_OBJECT
public:
    turing(std::string alphabet, std::string extrasymbols);
    ~turing() {
        delete header;
    }
private slots:
    void setNewStr();
private:
    QLineEdit *Alphabet;
    QLineEdit *ExtraSymbols;
    QLineEdit *ribbon;
    QLineEdit *str;
    int *header;
};

#endif // TURING_H
