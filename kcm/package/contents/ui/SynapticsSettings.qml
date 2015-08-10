import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    CheckboxEditor {
        text: i18n("Enable device")
        property: SimpleProperty {
            name: "Device Enabled"
        }
    }

    TabView {
        Layout.fillWidth: true
        Layout.margins: 5
        tabsVisible: true
        frameVisible: true

        Tab {
            title: i18n("Basic")
            SynapticsBasicTab { }
        }
    }
}
