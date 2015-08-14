import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.plasma.core 2.0 as PlasmaCore

ColumnLayout {
    id: root
    spacing: units.largeSpacing

    property Component enableDisableSection

    Loader {
        sourceComponent: enableDisableSection
    }

    RowLayout {
        Layout.fillWidth: true

        Label {
            Layout.fillWidth: true
            text: i18n("Settings")
            font.bold: true
        }

        Action {
            id: stopTestAction
            shortcut: "Esc"
            onTriggered: kcm.revertTest()
            enabled: kcm.canRevertTest
        }

        Label {
            visible: stopTestAction.enabled
            text: i18n("Press '%1' or click 'Reset' to stop testing", stopTestAction.shortcut)
        }

        Button {
            text: i18n("Test changes")
            enabled: kcm.canTest
            onClicked: kcm.test()
        }
    }

    property Component settingsSection

    ScrollView {
        id: scrollArea
        Layout.fillWidth: true
        Layout.fillHeight: true

        Loader {
            x: Math.max(0, (scrollArea.viewport.width - scrollArea.flickableItem.contentWidth) / 2)
            sourceComponent: settingsSection
        }
    }
}
