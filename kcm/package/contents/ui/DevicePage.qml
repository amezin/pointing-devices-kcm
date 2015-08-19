import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.plasma.core 2.0 as PlasmaCore

ColumnLayout {
    id: root
    spacing: units.largeSpacing

    readonly property var pageSettings: kcm.settingsFor(device, root)

    property Component enableDisableSection
    property Component settingsSection

    property bool testing: false

    Connections {
        target: kcm
        onLoadRequested: {
            pageSettings.loadSaved()
            pageSettings.apply()
        }
        onSaveRequested: {
            pageSettings.save()
            pageSettings.apply()
        }
        onDefaultsRequested: {
            pageSettings.loadDefaults()
        }
    }

    Binding {
        target: kcm
        property: "needsSave"
        value: pageSettings.needsSave
    }

    Connections {
        target: pageSettings
        onSavedDiffersFromActiveChanged: {
            if (!pageSettings.savedDiffersFromActive) {
                testing = false
            }
        }
    }

    Connections {
        target: pageLoader
        onCurrentItemChanged: {
            if (testing) {
                testing = false
                pageSettings.applySaved()
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        visible: pageSettings.savedDiffersFromActive && pageSettings.differsFromActive && !testing

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: i18n("Active device configuration does not match saved settings.") +
                      (!pageSettings.needsSave ? i18n("\nYou currently see saved settings.") : "")
                Layout.fillWidth: true
            }

            Button {
                text: i18n("View active settings")
                onClicked: pageSettings.loadActive()
            }

            Button {
                text: i18n("Apply saved settings")
                onClicked: pageSettings.applySaved()
            }
        }
    }

    GridLayout {
        rowSpacing: units.largeSpacing

        Loader {
            Layout.fillWidth: true
            sourceComponent: enableDisableSection
        }

        Label {
            Action {
                id: stopTestAction
                shortcut: "Esc"
                onTriggered: pageSettings.applySaved()
                enabled: testing
            }

            visible: stopTestAction.enabled
            text: i18n("Press '%1' or click 'Reset' to stop testing", stopTestAction.shortcut)
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
        }

        Button {
            text: i18n("Test changes")
            enabled: pageSettings.differsFromActive
            onClicked: {
                testing = pageSettings.needsSave
                pageSettings.apply()
            }
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
        }

        Label {
            Layout.column: 0
            Layout.row: 1
            text: i18n("Settings")
            font.bold: true
        }
    }

    ScrollView {
        id: scrollArea
        Layout.fillWidth: true
        Layout.fillHeight: true
        highlightOnFocus: true

        Loader {
            x: Math.max(0, (scrollArea.viewport.width - scrollArea.flickableItem.contentWidth) / 2)
            sourceComponent: settingsSection
        }
    }
}
