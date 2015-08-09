import QtQuick 2.4
import QtQuick.Controls 1.3

CheckBox {
    id: root
    property var property

    visible: property.available
    enabled: property.writable
    Binding on checked {
        when: root.property.available
        value: root.property.value
    }
    onCheckedChanged: property.setValue(checked)
}
