import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Label {
    Layout.alignment: Qt.AlignRight

    property Item target
    enabled: target.enabled
    visible: target.visible
}
