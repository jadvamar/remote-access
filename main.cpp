#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mqttservice.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qmlRegisterType<MqttService>("MqttService", 1, 0, "MqttService");
    engine.loadFromModule("VehicleAccess", "Main");

    return app.exec();
}
