#include "turing.h"
#include <QHBoxLayout>

turing::turing(std::string alphabet, std::string extrasymbols) {
    this->Alphabet = new QLineEdit(alphabet.c_str());
    Alphabet->setReadOnly(true);
    this->ExtraSymbols = new QLineEdit(extrasymbols.c_str());
    ExtraSymbols->setReadOnly(true);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(Alphabet); layout->addWidget(ExtraSymbols);
    setLayout(layout);

}
