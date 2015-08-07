import QtQuick 2.4
import QtQuick.Controls 1.3

Slider {
    id: root
    property var property

    visible: property && property.available
    enabled: property && property.writable

    Binding on value {
        when: root.property && root.property.available
        value: root.property ? root.property.value : 0
    }

    onValueChanged: if (property && property.writable) property.setValue(value)
}
