#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Smart Study Time Optimizer");

    app.setStyleSheet(R"(
        QWidget {
            background-color: #0d1728;
            color: #edf3ff;
            font-family: "Segoe UI";
            font-size: 10pt;
        }
        QFrame#sidebar {
            background-color: #111f34;
            border: 1px solid #223955;
            border-radius: 14px;
        }
        QLabel#appName {
            color: #ffffff;
            font-size: 15pt;
            font-weight: 700;
            letter-spacing: 1px;
        }
        QLabel#sidebarNote, QLabel#mutedText {
            color: #91a4bf;
            font-size: 9pt;
        }
        QPushButton#navButton {
            text-align: left;
            color: #c4d2e6;
            background: transparent;
            border: none;
            border-radius: 8px;
            padding: 10px;
            font-size: 10pt;
        }
        QPushButton#navButton:hover {
            background-color: #1c304b;
            color: white;
        }
        QPushButton#navButton:checked {
            background-color: #173d69;
            color: #ffffff;
            border-left: 3px solid #22a7ff;
        }
        QLabel#pageTitle {
            color: #ffffff;
            font-size: 23pt;
            font-weight: 700;
        }
        QLabel#sectionHeading, QLabel#smallHeading {
            color: #8fb4dc;
            font-size: 9pt;
            font-weight: 700;
            letter-spacing: 1px;
        }
        QLabel#recommendationText {
            color: #ffffff;
            font-size: 16pt;
            font-weight: 600;
            padding: 9px 0;
        }
        QLabel#largeNumber {
            color: #ffffff;
            font-size: 24pt;
            font-weight: 700;
        }
        QLabel#timerDisplay {
            color: #4db7ff;
            font-size: 54pt;
            font-weight: 700;
            padding: 20px;
        }
        QFrame#card {
            background-color: #14243a;
            border: 1px solid #294361;
            border-radius: 12px;
        }
        QPushButton {
            background-color: #233853;
            color: #edf3ff;
            border: 1px solid #365477;
            border-radius: 7px;
            padding: 8px 13px;
        }
        QPushButton:hover {
            background-color: #2d4a70;
        }
        QPushButton#primaryButton {
            background-color: #087de0;
            color: white;
            border: 1px solid #168ff2;
            font-weight: 600;
            padding: 9px 17px;
        }
        QPushButton#primaryButton:hover {
            background-color: #1595fa;
        }
        QPushButton#textButton {
            background: transparent;
            border: none;
            color: #5ebaff;
        }
        QLineEdit, QComboBox, QDateTimeEdit, QSpinBox {
            background-color: #0e1b2d;
            color: #edf3ff;
            border: 1px solid #365477;
            border-radius: 6px;
            padding: 7px;
            min-height: 22px;
        }
        QComboBox QAbstractItemView {
            background-color: #14243a;
            color: white;
            selection-background-color: #087de0;
        }
        QTableWidget, QListWidget {
            background-color: #101e31;
            alternate-background-color: #15263d;
            color: #e8f0fc;
            border: 1px solid #294361;
            border-radius: 8px;
            gridline-color: #263c58;
            padding: 5px;
        }
        QHeaderView::section {
            background-color: #1b304a;
            color: #dceaff;
            border: none;
            padding: 9px;
            font-weight: 600;
        }
        QTableWidget::item:selected, QListWidget::item:selected {
            background-color: #155b98;
        }
        QProgressBar {
            background-color: #263a54;
            border: none;
            border-radius: 6px;
        }
        QProgressBar::chunk {
            background-color: #0b9cf0;
            border-radius: 6px;
        }
        QCheckBox {
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
        }
        QScrollBar:vertical {
            background: #101e31;
            width: 10px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #355477;
            min-height: 25px;
            border-radius: 5px;
        }
    )");

    MainWindow window;
    window.show();
    return app.exec();
}
