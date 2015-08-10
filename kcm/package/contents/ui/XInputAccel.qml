import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

GridLayout {
    columns: 2

    AttachedLabel {
        text: i18n("Acceleration profile:")
        target: accelProfileCombo
    }

    ComboBox {
        Layout.fillWidth: true
        id: accelProfileCombo
        enabled: accelProfile.writable
        visible: accelProfile.available
        model: [i18n("None"),
                i18n("Classic"),
                i18n("Device-dependent"),
                i18n("Polynomial"),
                i18n("Smooth linear"),
                i18n("Simple"),
                i18n("Power"),
                i18n("Linear"),
                i18n("Limited")]

        Binding on currentIndex {
            when: accelProfile.available
            value: accelProfile.value + 1
        }

        onCurrentIndexChanged: {
            accelProfile.setValue(currentIndex - 1)
        }

        SimpleProperty {
            id: accelProfile
            name: "Device Accel Profile"
        }
    }

    AttachedLabel {
        text: i18n("Constant deceleration:")
        target: constantDecelerationSlider
    }

    SliderEditor {
        Layout.fillWidth: true
        id: constantDecelerationSlider
        property: SimpleProperty {
            name: "Device Accel Constant Deceleration"
        }
        minimumValue: 1.0
        maximumValue: 10.0
    }

    AttachedLabel {
        text: i18n("Adaptive deceleration:")
        target: adaptiveDecelerationSlider
    }

    SliderEditor {
        Layout.fillWidth: true
        id: adaptiveDecelerationSlider
        property: SimpleProperty {
            name: "Device Accel Adaptive Deceleration"
        }
        minimumValue: 1.0
        maximumValue: 1000.0
    }

    AttachedLabel {
        text: i18n("Velocity scaling:")
        target: velocityScalingSlider
    }

    SliderEditor {
        Layout.fillWidth: true
        id: velocityScalingSlider
        property: SimpleProperty {
            name: "Device Accel Velocity Scaling"
        }
        minimumValue: 1.0
        maximumValue: 100.0
    }
}
