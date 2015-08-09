import QtQuick 2.4

QtObject {
    id: root

    property var property
    property var availableFlag

    readonly property bool available: property.available &&
                                      availableFlag.available &&
                                      availableFlag.value
    readonly property bool writable: available && property.writable

    readonly property var value: available ? property.value : undefined
    function setValue(v) {
        if (available && writable) {
            property.setValue(v)
        }
    }
}
