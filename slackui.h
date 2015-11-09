#ifndef SLACKUI_H
#define SLACKUI_H

#include <QMainWindow>
#include "roster.h"
#include "input.h"
#include "chat.h"
#include <memory>
#include "slackclient.h"

namespace Ui {
class SlackUI;
}

class SlackUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SlackUI(QWidget *parent = 0);
    ~SlackUI();
    void set_client(SlackClient *cl);
    void add_message(QString message);

private:
    Ui::SlackUI *ui;
    SlackClient *cl;

    std::unique_ptr<Roster> roster;
    std::unique_ptr<Chat>   chat;
    std::unique_ptr<Input>  input;
};

#endif // SLACKUI_H
