/********************************************************************************
** Form generated from reading UI file 'monitor.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITOR_H
#define UI_MONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_monitorClass
{
public:
    QWidget *centralWidget;
    QLabel *picture;
    QPushButton *open_btn;
    QLabel *picture2;
    QPushButton *close_btn;
    QPushButton *camera2Open_btn;
    QPushButton *cameraOpen_btn;
    QPushButton *cameraClose_btn;
    QPushButton *camera1Open_btn;
    QPushButton *link_btn;
    QSlider *moveDir_vSlider;
    QSlider *horizontalSlider;
    QPushButton *speed0_btn;
    QPushButton *speed1_btn;
    QPushButton *speed2_btn;
    QPushButton *speed3_btn;
    QPushButton *speed4_btn;
    QPushButton *speed5_btn;
    QLineEdit *left_lEdit;
    QLabel *picture2_2;
    QLabel *picture2_3;
    QLineEdit *right_lEdit;
    QLabel *picture2_4;
    QLineEdit *leftDuty_lEdit;
    QLabel *picture2_5;
    QLineEdit *rightDuty_lEdit;
    QLineEdit *rightDuty_lEdit_2;
    QLineEdit *left_lEdit_2;
    QLabel *picture2_6;
    QLabel *picture2_7;
    QLineEdit *right_lEdit_2;
    QLabel *picture2_8;
    QLineEdit *leftDuty_lEdit_2;
    QLabel *picture2_9;
    QPushButton *check_btn;
    QPushButton *setPWM_btn;
    QLineEdit *caremaPort1_lEdit;
    QLabel *picture2_10;
    QLineEdit *left_lEdit_4;
    QLineEdit *caremaPort2_lEdit;
    QLabel *picture2_11;
    QPushButton *port1_btn;
    QPushButton *port2_btn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *monitorClass)
    {
        if (monitorClass->objectName().isEmpty())
            monitorClass->setObjectName(QStringLiteral("monitorClass"));
        monitorClass->resize(1332, 773);
        QIcon icon;
        icon.addFile(QStringLiteral(":/monitor/Resources/timg.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        monitorClass->setWindowIcon(icon);
        centralWidget = new QWidget(monitorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        picture = new QLabel(centralWidget);
        picture->setObjectName(QStringLiteral("picture"));
        picture->setGeometry(QRect(20, 20, 54, 12));
        open_btn = new QPushButton(centralWidget);
        open_btn->setObjectName(QStringLiteral("open_btn"));
        open_btn->setGeometry(QRect(630, 560, 61, 31));
        picture2 = new QLabel(centralWidget);
        picture2->setObjectName(QStringLiteral("picture2"));
        picture2->setGeometry(QRect(680, 20, 54, 12));
        close_btn = new QPushButton(centralWidget);
        close_btn->setObjectName(QStringLiteral("close_btn"));
        close_btn->setGeometry(QRect(630, 640, 61, 31));
        camera2Open_btn = new QPushButton(centralWidget);
        camera2Open_btn->setObjectName(QStringLiteral("camera2Open_btn"));
        camera2Open_btn->setGeometry(QRect(160, 570, 81, 41));
        cameraOpen_btn = new QPushButton(centralWidget);
        cameraOpen_btn->setObjectName(QStringLiteral("cameraOpen_btn"));
        cameraOpen_btn->setGeometry(QRect(280, 570, 91, 41));
        cameraClose_btn = new QPushButton(centralWidget);
        cameraClose_btn->setObjectName(QStringLiteral("cameraClose_btn"));
        cameraClose_btn->setGeometry(QRect(400, 570, 75, 41));
        camera1Open_btn = new QPushButton(centralWidget);
        camera1Open_btn->setObjectName(QStringLiteral("camera1Open_btn"));
        camera1Open_btn->setGeometry(QRect(40, 570, 81, 41));
        link_btn = new QPushButton(centralWidget);
        link_btn->setObjectName(QStringLiteral("link_btn"));
        link_btn->setGeometry(QRect(630, 600, 61, 31));
        moveDir_vSlider = new QSlider(centralWidget);
        moveDir_vSlider->setObjectName(QStringLiteral("moveDir_vSlider"));
        moveDir_vSlider->setGeometry(QRect(750, 530, 31, 81));
        moveDir_vSlider->setMaximum(2);
        moveDir_vSlider->setPageStep(1);
        moveDir_vSlider->setValue(1);
        moveDir_vSlider->setOrientation(Qt::Vertical);
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(820, 560, 121, 22));
        horizontalSlider->setMaximum(180);
        horizontalSlider->setValue(90);
        horizontalSlider->setOrientation(Qt::Horizontal);
        speed0_btn = new QPushButton(centralWidget);
        speed0_btn->setObjectName(QStringLiteral("speed0_btn"));
        speed0_btn->setGeometry(QRect(960, 550, 41, 41));
        speed1_btn = new QPushButton(centralWidget);
        speed1_btn->setObjectName(QStringLiteral("speed1_btn"));
        speed1_btn->setGeometry(QRect(1010, 550, 41, 41));
        speed2_btn = new QPushButton(centralWidget);
        speed2_btn->setObjectName(QStringLiteral("speed2_btn"));
        speed2_btn->setGeometry(QRect(1060, 550, 41, 41));
        speed3_btn = new QPushButton(centralWidget);
        speed3_btn->setObjectName(QStringLiteral("speed3_btn"));
        speed3_btn->setGeometry(QRect(1110, 550, 41, 41));
        speed4_btn = new QPushButton(centralWidget);
        speed4_btn->setObjectName(QStringLiteral("speed4_btn"));
        speed4_btn->setGeometry(QRect(1160, 550, 41, 41));
        speed5_btn = new QPushButton(centralWidget);
        speed5_btn->setObjectName(QStringLiteral("speed5_btn"));
        speed5_btn->setGeometry(QRect(1210, 550, 41, 41));
        left_lEdit = new QLineEdit(centralWidget);
        left_lEdit->setObjectName(QStringLiteral("left_lEdit"));
        left_lEdit->setEnabled(false);
        left_lEdit->setGeometry(QRect(760, 640, 51, 20));
        picture2_2 = new QLabel(centralWidget);
        picture2_2->setObjectName(QStringLiteral("picture2_2"));
        picture2_2->setGeometry(QRect(720, 640, 51, 21));
        picture2_3 = new QLabel(centralWidget);
        picture2_3->setObjectName(QStringLiteral("picture2_3"));
        picture2_3->setGeometry(QRect(920, 640, 51, 21));
        right_lEdit = new QLineEdit(centralWidget);
        right_lEdit->setObjectName(QStringLiteral("right_lEdit"));
        right_lEdit->setEnabled(false);
        right_lEdit->setGeometry(QRect(960, 640, 51, 20));
        picture2_4 = new QLabel(centralWidget);
        picture2_4->setObjectName(QStringLiteral("picture2_4"));
        picture2_4->setGeometry(QRect(820, 640, 51, 21));
        leftDuty_lEdit = new QLineEdit(centralWidget);
        leftDuty_lEdit->setObjectName(QStringLiteral("leftDuty_lEdit"));
        leftDuty_lEdit->setGeometry(QRect(860, 640, 51, 20));
        picture2_5 = new QLabel(centralWidget);
        picture2_5->setObjectName(QStringLiteral("picture2_5"));
        picture2_5->setGeometry(QRect(1020, 640, 51, 21));
        rightDuty_lEdit = new QLineEdit(centralWidget);
        rightDuty_lEdit->setObjectName(QStringLiteral("rightDuty_lEdit"));
        rightDuty_lEdit->setGeometry(QRect(1060, 640, 51, 20));
        rightDuty_lEdit_2 = new QLineEdit(centralWidget);
        rightDuty_lEdit_2->setObjectName(QStringLiteral("rightDuty_lEdit_2"));
        rightDuty_lEdit_2->setEnabled(false);
        rightDuty_lEdit_2->setGeometry(QRect(1060, 670, 51, 20));
        left_lEdit_2 = new QLineEdit(centralWidget);
        left_lEdit_2->setObjectName(QStringLiteral("left_lEdit_2"));
        left_lEdit_2->setEnabled(false);
        left_lEdit_2->setGeometry(QRect(760, 670, 51, 20));
        picture2_6 = new QLabel(centralWidget);
        picture2_6->setObjectName(QStringLiteral("picture2_6"));
        picture2_6->setGeometry(QRect(720, 670, 51, 21));
        picture2_7 = new QLabel(centralWidget);
        picture2_7->setObjectName(QStringLiteral("picture2_7"));
        picture2_7->setGeometry(QRect(920, 670, 51, 21));
        right_lEdit_2 = new QLineEdit(centralWidget);
        right_lEdit_2->setObjectName(QStringLiteral("right_lEdit_2"));
        right_lEdit_2->setEnabled(false);
        right_lEdit_2->setGeometry(QRect(960, 670, 51, 20));
        picture2_8 = new QLabel(centralWidget);
        picture2_8->setObjectName(QStringLiteral("picture2_8"));
        picture2_8->setGeometry(QRect(820, 670, 51, 21));
        leftDuty_lEdit_2 = new QLineEdit(centralWidget);
        leftDuty_lEdit_2->setObjectName(QStringLiteral("leftDuty_lEdit_2"));
        leftDuty_lEdit_2->setEnabled(false);
        leftDuty_lEdit_2->setGeometry(QRect(860, 670, 51, 20));
        picture2_9 = new QLabel(centralWidget);
        picture2_9->setObjectName(QStringLiteral("picture2_9"));
        picture2_9->setGeometry(QRect(1020, 670, 51, 21));
        check_btn = new QPushButton(centralWidget);
        check_btn->setObjectName(QStringLiteral("check_btn"));
        check_btn->setGeometry(QRect(1130, 670, 75, 23));
        setPWM_btn = new QPushButton(centralWidget);
        setPWM_btn->setObjectName(QStringLiteral("setPWM_btn"));
        setPWM_btn->setGeometry(QRect(1130, 640, 75, 23));
        caremaPort1_lEdit = new QLineEdit(centralWidget);
        caremaPort1_lEdit->setObjectName(QStringLiteral("caremaPort1_lEdit"));
        caremaPort1_lEdit->setGeometry(QRect(140, 630, 71, 21));
        picture2_10 = new QLabel(centralWidget);
        picture2_10->setObjectName(QStringLiteral("picture2_10"));
        picture2_10->setGeometry(QRect(60, 630, 91, 21));
        left_lEdit_4 = new QLineEdit(centralWidget);
        left_lEdit_4->setObjectName(QStringLiteral("left_lEdit_4"));
        left_lEdit_4->setGeometry(QRect(-530, 620, 71, 21));
        caremaPort2_lEdit = new QLineEdit(centralWidget);
        caremaPort2_lEdit->setObjectName(QStringLiteral("caremaPort2_lEdit"));
        caremaPort2_lEdit->setGeometry(QRect(350, 630, 71, 21));
        picture2_11 = new QLabel(centralWidget);
        picture2_11->setObjectName(QStringLiteral("picture2_11"));
        picture2_11->setGeometry(QRect(270, 630, 91, 21));
        port1_btn = new QPushButton(centralWidget);
        port1_btn->setObjectName(QStringLiteral("port1_btn"));
        port1_btn->setGeometry(QRect(100, 660, 75, 23));
        port2_btn = new QPushButton(centralWidget);
        port2_btn->setObjectName(QStringLiteral("port2_btn"));
        port2_btn->setGeometry(QRect(320, 660, 75, 23));
        monitorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(monitorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1332, 23));
        monitorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(monitorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        monitorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(monitorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        monitorClass->setStatusBar(statusBar);

        retranslateUi(monitorClass);

        QMetaObject::connectSlotsByName(monitorClass);
    } // setupUi

    void retranslateUi(QMainWindow *monitorClass)
    {
        monitorClass->setWindowTitle(QApplication::translate("monitorClass", "monitor", 0));
        picture->setText(QApplication::translate("monitorClass", "TextLabel", 0));
        open_btn->setText(QApplication::translate("monitorClass", "\346\211\223\345\274\200", 0));
        picture2->setText(QApplication::translate("monitorClass", "TextLabel", 0));
        close_btn->setText(QApplication::translate("monitorClass", "\345\205\263\351\227\255", 0));
        camera2Open_btn->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\2642\346\211\223\345\274\200", 0));
        cameraOpen_btn->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\264\345\205\250\346\211\223\345\274\200", 0));
        cameraClose_btn->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\264\345\205\263\351\227\255", 0));
        camera1Open_btn->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\2641\346\211\223\345\274\200", 0));
        link_btn->setText(QApplication::translate("monitorClass", "\351\207\215\350\277\236", 0));
        speed0_btn->setText(QApplication::translate("monitorClass", "0", 0));
        speed1_btn->setText(QApplication::translate("monitorClass", "1", 0));
        speed2_btn->setText(QApplication::translate("monitorClass", "2", 0));
        speed3_btn->setText(QApplication::translate("monitorClass", "3", 0));
        speed4_btn->setText(QApplication::translate("monitorClass", "4", 0));
        speed5_btn->setText(QApplication::translate("monitorClass", "5", 0));
        picture2_2->setText(QApplication::translate("monitorClass", "\346\226\271\345\220\2211\357\274\232", 0));
        picture2_3->setText(QApplication::translate("monitorClass", "\346\226\271\345\220\2211\357\274\232", 0));
        picture2_4->setText(QApplication::translate("monitorClass", "PWM1\357\274\232", 0));
        picture2_5->setText(QApplication::translate("monitorClass", "PWM2\357\274\232", 0));
        picture2_6->setText(QApplication::translate("monitorClass", "\346\226\271\345\220\2211\357\274\232", 0));
        picture2_7->setText(QApplication::translate("monitorClass", "\346\226\271\345\220\2211\357\274\232", 0));
        picture2_8->setText(QApplication::translate("monitorClass", "PWM1\357\274\232", 0));
        picture2_9->setText(QApplication::translate("monitorClass", "PWM2\357\274\232", 0));
        check_btn->setText(QApplication::translate("monitorClass", "\346\237\245\350\257\242", 0));
        setPWM_btn->setText(QApplication::translate("monitorClass", "\350\256\276\347\275\256", 0));
        picture2_10->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\2641\347\253\257\345\217\243", 0));
        picture2_11->setText(QApplication::translate("monitorClass", "\346\221\204\345\203\217\345\244\2642\347\253\257\345\217\243", 0));
        port1_btn->setText(QApplication::translate("monitorClass", "\350\256\276\347\275\256", 0));
        port2_btn->setText(QApplication::translate("monitorClass", "\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class monitorClass: public Ui_monitorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITOR_H
