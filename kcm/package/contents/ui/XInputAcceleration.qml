import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.PointingDevicesKCM 1.0

GridLayout {
    id: root
    property var device: null
    columns: 2
    Label {
        text: i18n("Adaptive Deceleration:")
    }
    Slider {
        Layout.fillWidth: true
        minimumValue: 1
        maximumValue: 10
        DevicePropertyBinding on value {
            device: root.device
            deviceProperty: "accelAdaptiveDeceleration"
        }
    }
    Label {
        text: i18n("Constant Deceleration:")
    }
    Slider {
        Layout.fillWidth: true
        minimumValue: 1
        maximumValue: 10
        DevicePropertyBinding on value {
            device: root.device
            deviceProperty: "accelConstantDeceleration"
        }
    }
    Label {
        text: i18n("Velocity Scaling:")
    }
    Slider {
        Layout.fillWidth: true
        minimumValue: 0
        maximumValue: 100
        DevicePropertyBinding on value {
            device: root.device
            deviceProperty: "accelVelocityScaling"
        }
    }
}
