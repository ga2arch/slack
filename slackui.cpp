#include "slackui.h"
#include "ui_slackui.h"

SlackUI::SlackUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SlackUI)
{
    ui->setupUi(this);
    roster = std::make_unique<Roster>(this);
    input = std::make_unique<Input>(this);
    chat = std::make_unique<Chat>(this);
}

SlackUI::~SlackUI()
{
    delete ui;
}

void SlackUI::set_client(SlackClient *cl) {
    this->cl = cl;
}

void SlackUI::add_message(QString message) {
    chat->add_message(message);
}
