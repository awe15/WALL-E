#ifndef MONITOR_H
#define MONITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_monitor.h"
#include "qtimer.h"
#include "stream_unpacket.h"
class monitor : public QMainWindow
{
	Q_OBJECT

public:
	monitor(QWidget *parent = 0);
	~monitor();
	
private:
	Ui::monitorClass ui;
	char speed;
	video* v1;
	video* v2;
	QTimer *timer;

public slots:  
	void on_openBtn();
	void on_closeBtn();
	void on_linkBtn();
	void timerDone();
	void on_speed0Btn();
	void on_speed1Btn();
	void on_speed2Btn();
	void on_speed3Btn();
	void on_speed4Btn();
	void on_speed5Btn();
	void moveAngleChange();
	void moveDirChange();
	void on_getPWMBtn();
	void on_setPWMBtn();
	void on_camera1OpenBtn();
	void on_camera2OpenBtn();
	void on_cameraOpenBtn();
	void on_cameraCloseBtn();
	void on_port2Btn();
	void on_port1Btn();
};

#endif // MONITOR_H
