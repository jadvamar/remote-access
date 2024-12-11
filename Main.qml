import QtQuick
import QtQuick.Controls 2.15
import MqttService 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("MQTT Example")

    MqttService {
        id: mqttHandler
        onConnectionStatusChanged: {
            connectedText.text = status
            connectedText.color = connected ? "green" : "red"
        }
    }

    Rectangle {
        id: mainRectangle
        anchors.fill: parent
        color: "#f0f0f0" // Background color
        Text {
            id: connectedText
            text: "Waiting for MQTT message..."
            color: "black"
            font.pixelSize: 15

        }

        Column {
            anchors.centerIn: parent
            spacing: 20

            // Row 1: Start and Stop Buttons
            Row {
                spacing: 20
                Button {
                    text: "Start"
                    width: 100
                    height: 30

                    onClicked: {
                        mqttHandler.connectToHost(
                            "dfd1466df64c41c98c5cd556ff04bf8b.s1.eu.hivemq.cloud", // Hostname
                             8883,             // Port
                            "Amar1",               // Username (if required)
                            "Amar@123",               // Password (if required)
                            "start"            // Topic to subscribe
                        );
                        connectedText.text = "Connecting to broker...";
                    }
                }

                Button {
                    text: "Stop"
                    width: 100
                    height: 30

                    onClicked: {
                        mqttHandler.disconnectFromHost();
                        connectedText.text = "Disconnected from broker.";
                    }
                }
            }

            // Row 2: Input Field
            Row {
                spacing: 20
                TextField {
                    id: inputField
                    placeholderText: "Enter your message..."
                    width: 300
                    height: 40
                    font.pixelSize: 18
                    color: "black"
                    background: Rectangle {
                        color: "white"
                        radius: 5
                        border.color: "gray"
                    }
                }
            }

            // Row 3: Send Button
            Row {
                spacing: 20
                Button {
                    text: "Send"
                    width: 100
                    height: 30

                    onClicked: {
                        if (inputField.text !== "") {
                            mqttHandler.publishMessage("start", inputField.text);
                            connectedText.text = "Message sent: " + inputField.text;
                            inputField.text = ""; // Clear the input field after sending
                        } else {
                            connectedText.text = "Input field is empty!";
                        }
                    }
                }
            }
        }
    }


}
