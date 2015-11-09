#include "slackui.h"
#include "slackclient.h"
#include <QApplication>
#include <thread>
#include <future>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlackUI w;
   // SlackClient cl;

   // w.set_client(&cl);
   // cl.set_ui(&w);

    // std::thread([&]() { cl.start(); }).detach();

    w.setWindowTitle("slack++");
    w.show();

    return a.exec();
}
