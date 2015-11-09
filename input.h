#ifndef INPUT_H
#define INPUT_H

#include <QTextEdit>

class Input
{
public:
    Input(QWidget *parent);
    ~Input();

private:
    QTextEdit *input;

};

#endif // INPUT_H
