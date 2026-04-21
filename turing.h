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
private:
    QLineEdit *Alphabet;
    QLineEdit *ExtraSymbols;
};

#endif // TURING_H
