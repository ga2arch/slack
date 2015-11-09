#include "input.h"

Input::Input(QWidget *parent)
{
    input = new QTextEdit(parent);
    input->setGeometry(80, parent->height() - 100, parent->width() - 80, 100);
    input->append("Hello slack++");
    input->show();
}

Input::~Input() {
    delete input;
}

