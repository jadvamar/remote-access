#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <QObject>
#include <QMqttClient>

class MqttService : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectionStatusChanged)

public:
    explicit MqttService(QObject *parent = nullptr);
    ~MqttService();

    Q_INVOKABLE void connectToHost(
        const QString &hostname,
        int port,
        const QString &username,
        const QString &password,
        const QString &topic);

    Q_INVOKABLE void disconnectFromHost();
    Q_INVOKABLE void publishMessage(const QString &topic, const QString &message);

    bool isConnected() const;

signals:
    void connectionStatusChanged(const QString &status, bool connected);
    void messageReceived(const QString &message, const QString &topic);

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

private:
    QMqttClient *m_client;
    QString m_topic;
};

#endif // MQTTSERVICE_H
