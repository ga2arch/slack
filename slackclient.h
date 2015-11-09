#ifndef SLACKCLIENT_H
#define SLACKCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

class SlackUI;

class SlackClient : public QObject
{
public:
    void start();
    void set_ui(SlackUI* ui);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void process_event(const std::string& json);

private:
    const std::string fetch_data();
    void connect(const std::string& uri);
    QJsonDocument call(const std::string& api, const std::string& query);

    QWebSocket m_webSocket;
    SlackUI* ui;
    int sent_id = 0;
};

#include "slackui.h"

#endif // SLACKCLIENT_H
