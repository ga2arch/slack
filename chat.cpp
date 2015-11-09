#include "chat.h"

Chat::Chat(QWidget *parent)
{
    chat = new QTextEdit(parent);
    chat->setGeometry(80, 0, parent->width() - 80, parent->height() - 100);
    chat->setReadOnly(true);
    chat->show();
}

Chat::~Chat() {
    delete(chat);
}

void Chat::add_message(QString message) {
    chat->append(message);
}
