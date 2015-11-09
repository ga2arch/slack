#ifndef CHAT_H
#define CHAT_H

#include <QTextEdit>

class Chat
{
public:
    Chat(QWidget *parent);
    ~Chat();
    void add_message(QString message);

private:
    QTextEdit *chat;
};

#endif // CHAT_H
