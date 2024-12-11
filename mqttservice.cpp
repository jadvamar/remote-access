#include "mqttservice.h"
#include <QSslConfiguration>
#include <QDebug>

MqttService::MqttService(QObject *parent)
    : QObject(parent),
    m_client(new QMqttClient(this)) {
    // Connect signals from the QMqttClient to slots in this class
    connect(m_client, &QMqttClient::connected, this, &MqttService::onConnected);
    connect(m_client, &QMqttClient::disconnected, this, &MqttService::onDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this, &MqttService::onMessageReceived);
    connect(m_client, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error) {
        qWarning() << "MQTT Error:" << error;
    });
}

MqttService::~MqttService() {
    delete m_client;
}

void MqttService::connectToHost(
    const QString &hostname,
    int port,
    const QString &username,
    const QString &password,
    const QString &topic) {

    qDebug() << "Attempting to connect to host:" << hostname << "on port:" << port;

    m_topic = topic;

    // Configure SSL/TLS
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater); // Ensure modern protocol is used

    // Set MQTT client parameters
    m_client->setHostname(hostname);
    m_client->setPort(port);
    m_client->setUsername(username);
    m_client->setPassword(password);
    m_client->setProtocolVersion(QMqttClient::MQTT_3_1_1);

    qDebug() << "SSL Configuration and Client parameters set. Connecting...";

    // Pass the hostname and port explicitly to connectToHostEncrypted
    m_client->connectToHostEncrypted(sslConfig);
}

void MqttService::disconnectFromHost() {
    if (isConnected()) {
        qDebug() << "Disconnecting from broker...";
        m_client->disconnectFromHost();
    } else {
        qWarning() << "Cannot disconnect: Not connected to any broker.";
    }
}

bool MqttService::isConnected() const {
    return m_client->state() == QMqttClient::Connected;
}

void MqttService::publishMessage(const QString &topic, const QString &message) {
    qDebug()<<"publishing";
    if (isConnected()) {
        auto result = m_client->publish(topic, message.toUtf8());
        if (result == -1) {
            qWarning() << "Failed to publish message to topic:" << topic;
        } else {
            qDebug() << "Message published to topic:" << topic << "Message:" << message;
        }
    } else {
        qWarning() << "Cannot publish: Not connected to broker.";
    }
}

void MqttService::onConnected() {
    qDebug() << "Successfully connected to broker. Subscribing to topic:" << m_topic;
    auto subscription = m_client->subscribe(m_topic, 0);
    if (subscription) {
        emit connectionStatusChanged("Connected to " + m_client->hostname(), true);
        qDebug() << "Subscribed to topic:" << m_topic;
    } else {
        emit connectionStatusChanged("Connected, but subscription failed", false);
        qWarning() << "Subscription failed for topic:" << m_topic;
    }
}

void MqttService::onDisconnected() {
    emit connectionStatusChanged("Disconnected", false);
    qDebug() << "Disconnected from broker.";
}

void MqttService::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic) {
    emit messageReceived(QString::fromUtf8(message), topic.name());
    qDebug() << "Message received. Topic:" << topic.name() << "Message:" << message;
}
