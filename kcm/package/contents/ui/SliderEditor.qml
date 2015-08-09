import QtQuick 2.4
import QtQuick.Controls 1.3

Slider {
    id: root
    property var property

    visible: property.available
    enabled: property.writable
    Binding on value {
        when: root.property.available
        value: root.property.value
    }
    onValueChanged: property.setValue(value)
}
