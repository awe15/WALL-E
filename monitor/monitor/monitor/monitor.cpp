#include "monitor.h"
#include "QDebug"
#include "mon_connector.h"

QMutex mutex;

char buff[50000];
char buff2[50000];
QImage img1(640, 480, QImage::Format_RGB32);
QImage img2(640, 480, QImage::Format_RGB32);
socket_info comm_link = {"192.168.55.1", 1234};
monConnector comm_conct(&comm_link);

monitor::monitor(QWidget *parent)
	: QMainWindow(parent)
{
	speed = 0;
	QPixmap pixmap("Resources/timg.jpg");
	ui.setupUi(this);
	ui.picture->setPixmap(pixmap);
	ui.picture->setFixedSize(640, 480);
	ui.picture->setScaledContents(true);
	ui.picture2->setPixmap(pixmap);
	ui.picture2->setFixedSize(640, 480);
	ui.picture2->setScaledContents(true);

	comm_conct.mon_connect();

	timer = new QTimer;

	connect(timer, SIGNAL(timeout()),this, SLOT(timerDone()) );
	 // 

	connect(ui.open_btn, SIGNAL(clicked()), this, SLOT(on_openBtn()));
	connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(on_closeBtn()));
	connect(ui.link_btn, SIGNAL(clicked()), this, SLOT(on_linkBtn()));
	connect(ui.speed0_btn, SIGNAL(clicked()), this, SLOT(on_speed0Btn()));
	connect(ui.speed1_btn, SIGNAL(clicked()), this, SLOT(on_speed1Btn()));
	connect(ui.speed2_btn, SIGNAL(clicked()), this, SLOT(on_speed2Btn()));
	connect(ui.speed3_btn, SIGNAL(clicked()), this, SLOT(on_speed3Btn()));
	connect(ui.speed4_btn, SIGNAL(clicked()), this, SLOT(on_speed4Btn()));
	connect(ui.speed5_btn, SIGNAL(clicked()), this, SLOT(on_speed5Btn()));
	connect(ui.horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(moveAngleChange()));
	connect(ui.moveDir_vSlider, SIGNAL(sliderReleased()), this, SLOT(moveDirChange()));
	connect(ui.setPWM_btn, SIGNAL(clicked()), this, SLOT(on_setPWMBtn()));
	connect(ui.check_btn, SIGNAL(clicked()), this, SLOT(on_getPWMBtn()));
	connect(ui.camera1Open_btn, SIGNAL(clicked()), this, SLOT(on_camera1OpenBtn()));
	connect(ui.camera2Open_btn, SIGNAL(clicked()), this, SLOT(on_camera2OpenBtn()));
	connect(ui.cameraOpen_btn, SIGNAL(clicked()), this, SLOT(on_cameraOpenBtn()));
	connect(ui.cameraClose_btn, SIGNAL(clicked()), this, SLOT(on_cameraCloseBtn()));
	connect(ui.port2_btn, SIGNAL(clicked()), this, SLOT(on_port2Btn()));
	connect(ui.port1_btn, SIGNAL(clicked()), this, SLOT(on_port1Btn()));
}

monitor::~monitor()
{

}

/***************电机*****************/
void monitor::on_speed0Btn()
{
	speed = 0;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x00, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}
void monitor::on_speed1Btn()
{
	speed = 1;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x01, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}
void monitor::on_speed2Btn()
{
	speed = 2;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x02, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}
void monitor::on_speed3Btn()
{
	speed = 3;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x03, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}
void monitor::on_speed4Btn()
{
	speed = 4;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x04, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}
void monitor::on_speed5Btn()
{
	speed = 5;
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x05, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

//方向
void monitor::moveAngleChange()
{
	char sendbuff1[] = {0x7f, 0x00, 0x06, 0x10, 0x11, 0x00, 0x05, 0x65};
	sendbuff1[5] = ui.horizontalSlider->value();
	sendbuff1[6] = speed;
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

void monitor::moveDirChange()
{
	char sendbuff1[] = {0x7f, 0x00, 0x05, 0x10, 0x10, 0x02,  0x65};
	switch (ui.moveDir_vSlider->value())
	{
	case 0:
			sendbuff1[5] = 2;
			break;
	case 1:
			sendbuff1[5] = 0;
			break;
	case 2:
		sendbuff1[5] = 1;
			break;
	}
	
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

//PWM设置
void monitor::on_setPWMBtn()
{
	
}



//PWM查询
void monitor::on_getPWMBtn()
{

}


/***************整体控制*****************/
void monitor::on_closeBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x03, 0x02, 0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}


void monitor::on_openBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x03, 0x01, 0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

void monitor::on_linkBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x03, 0x03, 0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

/***************摄像头控制*****************/
//摄像头开
void monitor::on_camera1OpenBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x05, 0x20, 0x01, 0x01,  0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

void monitor::on_camera2OpenBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x05, 0x20, 0x01, 0x02,  0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

void monitor::on_cameraOpenBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x05, 0x20, 0x01, 0x0f,  0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}

//摄像头关
void monitor::on_cameraCloseBtn()
{
	char sendbuff1[] = {0x7f, 0x00, 0x05, 0x20, 0x01, 0x00,  0x65};
	comm_conct.mon_send(sendbuff1,sizeof(sendbuff1));
}


void monitor::on_port2Btn()
{
	timer->start(40);
}

void monitor::on_port1Btn()
{

	timer->stop();

}

void monitor::timerDone()
{

	int len = 0;
	
	socket_info tcp_link = {"192.168.55.1", 8090};
	monConnector connector(&tcp_link);
	v1 = new video(&connector);
	socket_info tcp_link2 = {"192.168.55.1", 8070};
	monConnector connector2(&tcp_link2);
	v2 = new video(&connector2);
	

	v1->init();
	len = v1->get_videoStream(buff);       
	if (len > 2000)
	{
		img1.loadFromData((unsigned char*)buff, len, "JPG");          
		if (!img1.isNull())
			ui.picture->setPixmap(QPixmap::fromImage(img1));
	}
	
	v2->init();
	len = v2->get_videoStream(buff);       
	if (len > 2000)
	{
		img2.loadFromData((unsigned char*)buff, len, "JPG");          
		if (!img2.isNull())
			ui.picture2->setPixmap(QPixmap::fromImage(img2));
	}
	delete v1;
	delete v2;
}