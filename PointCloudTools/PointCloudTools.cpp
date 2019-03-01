#include "PointCloudTools.h"

PointCloudTools::PointCloudTools(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	/******************* Slots connection of QMenuBar and QToolBar ************************/
	// File (connect)
	QObject::connect(ui.openAction, &QAction::triggered, this, &PointCloudTools::open);
	QObject::connect(ui.clearAction, &QAction::triggered, this, &PointCloudTools::clear);
	QObject::connect(ui.saveAction, &QAction::triggered, this, &PointCloudTools::save);
	QObject::connect(ui.saveBinaryAction, &QAction::triggered, this, &PointCloudTools::saveBinary);
	QObject::connect(ui.exitAction, &QAction::triggered, this, &PointCloudTools::exit);
	//Display (connect)
	QObject::connect(ui.pointcolorAction, &QAction::triggered, this, &PointCloudTools::pointcolorChanged);
	QObject::connect(ui.bgcolorAction, &QAction::triggered, this, &PointCloudTools::bgcolorChanged);
	QObject::connect(ui.mainviewAction, &QAction::triggered, this, &PointCloudTools::mainview);
	QObject::connect(ui.leftviewAction, &QAction::triggered, this, &PointCloudTools::leftview);
	QObject::connect(ui.topviewAction, &QAction::triggered, this, &PointCloudTools::topview);
	// View (connect)
	QObject::connect(ui.dataAction, &QAction::triggered, this, &PointCloudTools::data);
	QObject::connect(ui.propertyAction, &QAction::triggered, this, &PointCloudTools::properties);
	QObject::connect(ui.consoleAction, &QAction::triggered, this, &PointCloudTools::console);
	QObject::connect(ui.RGBAction, &QAction::triggered, this, &PointCloudTools::rgbDock);
	// Generate (connect)
	QObject::connect(ui.cubeAction, &QAction::triggered, this, &PointCloudTools::cube);
	QObject::connect(ui.sphereAction, &QAction::triggered, this, &PointCloudTools::createSphere);
	QObject::connect(ui.cylinderAction, &QAction::triggered, this, &PointCloudTools::createCylinder);
	// Process (connect)
	QObject::connect(ui.meshsurfaceAction, &QAction::triggered, this, &PointCloudTools::convertSurface);
	QObject::connect(ui.wireframeAction, &QAction::triggered, this, &PointCloudTools::convertWireframe);
	// Option (connect)
	QObject::connect(ui.windowsThemeAction, &QAction::triggered, this, &PointCloudTools::windowsTheme);
	QObject::connect(ui.darculaThemeAction, &QAction::triggered, this, &PointCloudTools::darculaTheme);
	QObject::connect(ui.englishAction, &QAction::triggered, this, &PointCloudTools::langEnglish);
	QObject::connect(ui.chineseAction, &QAction::triggered, this, &PointCloudTools::langChinese);
	// About (connect)
	QObject::connect(ui.aboutAction, &QAction::triggered, this, &PointCloudTools::about);
	QObject::connect(ui.helpAction, &QAction::triggered, this, &PointCloudTools::help);

	/***** Slots connection of image widget *****/
	connect(ui.colormapBtn, SIGNAL(clicked()), this, SLOT(colormapBtnPressed()));
	connect(ui.convertBtn, SIGNAL(clicked()), this, SLOT(convertBtnPressed()));

	/***** Slots connection of RGB widget *****/
	// Random color (connect)
	connect(ui.colorBtn, SIGNAL(clicked()), this, SLOT(colorBtnPressed()));
	// Change size of cloud (connect)
	connect(ui.pSlider, SIGNAL(valueChanged(int)), this, SLOT(pSliderChanged(int)));
	connect(ui.pSlider, SIGNAL(sliderReleased()), this, SLOT(psliderReleased()));
	// Change colormap properties (connect)
	connect(ui.colorMapRight, SIGNAL(editingFinished()), this, SLOT(colormap()));
	connect(ui.colorMapLeft, SIGNAL(editingFinished()), this, SLOT(colormap()));
	connect(ui.colormap_x, SIGNAL(clicked()), this, SLOT(colormap()));
	connect(ui.colormap_y, SIGNAL(clicked()), this, SLOT(colormap()));
	connect(ui.colormap_z, SIGNAL(clicked()), this, SLOT(colormap()));

	/***** Slots connection of dataTree(QTreeWidget) widget *****/
	// Item in dataTree is left-clicked (connect)
	connect(ui.dataTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemSelected(QTreeWidgetItem*, int)));
	// Item in dataTree is right-clicked
	connect(ui.dataTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popMenu(const QPoint&)));

	connect(ui.consoleTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popMenuInConsole(const QPoint&)));
	// Initialization
	initial();
}

void PointCloudTools::open()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Open image file"), QString::fromUtf8(mypicture.fullname.c_str()), tr("Depth Image(*.png);;PointCloud File(*.pcd *.ply);;All file(*.*)"));
	if (filenames.isEmpty())
		return;

	for (int i = 0; i != filenames.size(); i++)
	{
		timeStart();
		QString filename = filenames[i];
		std::string file_name = string(filename.toLocal8Bit());
		std::string subname = getFileName(file_name);
		std::string filetype = getFileType(file_name);

		//����״̬��
		ui.statusBar->showMessage(QString::fromUtf8(subname.c_str()) + " : " + QString::number(i) + "/" + QString::number(filenames.size()) + " image open");

		int status = -1;
		if (filename.endsWith(".png", Qt::CaseInsensitive) || filename.endsWith(".bmp",Qt::CaseInsensitive))
		{
			// ͼƬ��
			cv::Mat img = cv::imread(file_name, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
			mypicture.fullname = file_name;
			mypicture.filename = subname;
			mypicture.filetype = filetype;
			mypicture.depthMat = img.clone();

			//���ͼ����ȡ�ͨ��
			if (img.type() != CV_16U)
			{
				int type = img.type();
				QString type_str = QString::number(type / 8 + 1) + "C";
				switch (type % 8)
				{
				case 0: type_str += "8U"; break;
				case 1: type_str += "8S"; break;
				case 2: type_str += "16U"; break;
				case 3: type_str += "16S"; break;
				case 4: type_str += "32S"; break;
				case 5: type_str += "32F"; break;
				case 6: type_str += "64F"; break;
				default: type_str += "Unknow";
					break;
				}
				consoleLog("Open", QString::fromLocal8Bit(subname.c_str()), "Image type:" + type_str, "Should open 16bits depth image.");
			}

			cv::Mat zip;
			img.convertTo(zip, CV_8U, 1.0 / 256, 0);
			cv::resize(img, img, cv::Size(320, 240));
			QImage qimg = QImage((const unsigned char*)(zip.data), zip.cols, zip.rows, QImage::Format_Indexed8);
			ui.imageDepth->setPixmap(QPixmap::fromImage(qimg));
			status = 0;

			//������Դ������
			QTreeWidgetItem *picName = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit(subname.c_str()));
			picName->setIcon(0, QIcon(":/Resources/images/pic.png"));
			ui.dataTree->addTopLevelItem(picName);
		}
		else
		{
			//���ƴ�
			if (filename.endsWith(".pcd", Qt::CaseInsensitive))
			{

			}
			else if (filename.endsWith(".ply", Qt::CaseInsensitive))
			{

			}
			else
			{

			}
		}

		QString time_cost = timeOff();
		consoleLog("Open", QString::fromLocal8Bit(subname.c_str()), QString::fromLocal8Bit(file_name.c_str()), "Time cost: " + time_cost + " s");

	}

	ui.statusBar->showMessage("");
	
}

void PointCloudTools::clear()
{

}

void PointCloudTools::save()
{

}

void PointCloudTools::saveBinary()
{

}

void PointCloudTools::exit()
{
	this->close();
}

void PointCloudTools::pointcolorChanged()
{

}

void PointCloudTools::bgcolorChanged()
{

}

void PointCloudTools::mainview()
{

}

void PointCloudTools::leftview()
{

}

void PointCloudTools::topview()
{

}

void PointCloudTools::data()
{

}

void PointCloudTools::properties()
{

}

void PointCloudTools::console()
{

}

void PointCloudTools::rgbDock()
{

}

void PointCloudTools::cube()
{

}

void PointCloudTools::createSphere()
{

}

void PointCloudTools::createCylinder()
{

}

int PointCloudTools::convertSurface()
{
	return 0;
}

int PointCloudTools::convertWireframe()
{
	return 0;
}

void PointCloudTools::windowsTheme()
{

}

void PointCloudTools::darculaTheme()
{

}

void PointCloudTools::langEnglish()
{

}

void PointCloudTools::langChinese()
{

}

void PointCloudTools::about()
{
	AboutWin *aboutwin = new AboutWin(this);
	aboutwin->setModal(true);
	aboutwin->show();

	consoleLog("About", "", "", "");
}

void PointCloudTools::help()
{

}

void PointCloudTools::colormapBtnPressed()
{
	ColormapWin *win = new ColormapWin();
	connect(win, SIGNAL(infoSend(ColormapClass)), this, SLOT(colormap(ColormapClass)));
	win->setModal(true);
	win->show();
}

void PointCloudTools::convertBtnPressed()
{
	if (mypicture.depthMat.empty())
		return;

	timeStart();

	double fx = ui.fxLineedit->text().toDouble();
	double fy = ui.fyLineedit->text().toDouble();
	double cx = ui.cxLineedit->text().toDouble();
	double cy = ui.cyLineedit->text().toDouble();
	double a = ui.aLineedit->text().toDouble();
	double k1 = ui.k1Lineedit->text().toDouble();
	double k2 = ui.k2Lineedit->text().toDouble();
	double p1 = ui.p1Lineedit->text().toDouble();
	double p2 = ui.p2Lineedit->text().toDouble();
	double k3 = ui.k3Lineedit->text().toDouble();

	//�������
	cv::Mat img;

	//�ڲξ���
	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);		//3*3��λ����
	cameraMatrix.at<double>(0, 0) = fx;
	cameraMatrix.at<double>(0, 1) = 0;
	cameraMatrix.at<double>(0, 2) = cx;
	cameraMatrix.at<double>(1, 1) = fy;
	cameraMatrix.at<double>(1, 2) = cy;
	cameraMatrix.at<double>(2, 2) = 1;
	//�������
	cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);		//5*1ȫ0����
	distCoeffs.at<double>(0, 0) = k1;
	distCoeffs.at<double>(1, 0) = k2;
	distCoeffs.at<double>(2, 0) = p1;
	distCoeffs.at<double>(3, 0) = p2;
	distCoeffs.at<double>(4, 0) = k3;

	cv::Size imageSize = mypicture.depthMat.size();
	cv::Mat map1, map2;

	//����1������ڲξ���
	//����2���������
	//����3����ѡ���룬��һ�͵ڶ��������֮�����ת����
	//����4��У�����3X3�������
	//����5����ʧ��ͼ��ߴ�
	//����6��map1�������ͣ�CV_32FC1��CV_16SC2
	//����7��8�����X/Y������ӳ�����
	initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, imageSize, CV_32FC1, map1, map2);	//�������ӳ��
	//����1������ԭʼͼ��
	//����2�����ͼ��
	//����3��4��X\Y������ӳ��
	//����5��ͼ��Ĳ�ֵ��ʽ
	//����6���߽���䷽ʽ
	remap(mypicture.depthMat, img, map1, map2, cv::INTER_LINEAR);																	//�������

	//���Ʊ任
	int imgWidth = img.size().width;
	int imgHeight = img.size().height;
	PointCloudT::Ptr pointcloud(new PointCloudT);

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			float picDist = sqrt((i - imgHeight / 2.0)*(i - imgHeight / 2.0) + (j - imgWidth / 2.0)*(j - imgWidth / 2.0));	//ͼ���ϵ㵽���ĵ����ص����
			float picAngle = atan2(i - imgHeight / 2.0, j - imgWidth / 2.0);												//ͼ����x,y�����ĵ�Ƕȹ�ϵ
			float angle = atan(sqrt((j - imgWidth / 2.0)*(j - imgWidth / 2.0) / fx / fx + (i - imgHeight / 2.0)*(i - imgHeight / 2.0) / fy / fy));
			float dist = img.at<ushort>(i, j) / a;				//ԭʼͼ�����

			pcl::PointXYZRGBA p;
			p.z = dist*cos(angle);									//����任������
			p.x = dist*sin(angle)*cos(picAngle);
			p.y = dist*sin(angle)*sin(picAngle);

			p.r = 250;
			p.g = 250;
			p.b = 250;
			p.a = 255;

			pointcloud->points.push_back(p);
		}
	}

	if (mypicture.isConvert)
	{
		for (auto it = mycloud_vec.begin(); it != mycloud_vec.end(); it++)
		{
			if (it->fullname == mypicture.fullname + ".pcd")
			{
				it->cloud = pointcloud;
				it->visible = true;
				mycloud = *it;
				break;
			}
		}
	}
	else
	{
		mypicture.isConvert = true;
		mycloud.cloud = pointcloud;
		mycloud.filename = mypicture.filename + ".pcd";
		mycloud.fullname = mypicture.fullname + ".pcd";
		mycloud.filetype = "pcd";
		mycloud.visible = true;
		mycloud_vec.push_back(mycloud);

		//������Դ��
		QTreeWidgetItem *cloudName = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit(mycloud.filename.c_str()));
		cloudName->setIcon(0, QIcon(":/Resources/images/point.png"));
		ui.dataTree->addTopLevelItem(cloudName);
	}
	//��ʱ����
	QString time_cost = timeOff();
	consoleLog("Convert", QString::fromLocal8Bit(mypicture.filename.c_str()), QString::fromLocal8Bit(mycloud.filename.c_str()), "Time cost: " + time_cost + " s, Points: " + QString::number(mycloud.cloud->points.size()));
	

	showPointcloudAdd();		//���µ��ƴ���
}

void PointCloudTools::colormap(ColormapClass cc)
{
	if (mypicture.depthMat.empty())
		return;

	int max = cc.max;
	int min = cc.min;
	int type = cc.type;
	if (max < min)
	{
		QMessageBox::warning(this, "Warrning", "The max number is smaller than the min number!", QMessageBox::Yes);
		return;
	}

	timeStart();

	float scal = 255.0 / (max - min);
	cv::Mat colorMat;
	mypicture.depthMat.convertTo(colorMat, CV_8U, scal, -min*scal);		//ת8λ�Ҷ�
	cv::applyColorMap(colorMat, colorMat, type);						//ת��ɫͼ
	mypicture.colorMat = colorMat.clone();

	cv::cvtColor(colorMat, colorMat, CV_BGR2RGB);						//OpencvĬ��BGR�洢��Qt��ҪRGB
	QImage qimg = QImage((const unsigned char*)(colorMat.data), colorMat.cols, colorMat.rows, QImage::Format_RGB888);
	ui.imageColor->setPixmap(QPixmap::fromImage(qimg));

	QString time_cost = timeOff();
	//��Ϣ��ʾ
	QString type_str;
	switch (type)
	{
	case 0: type_str = "AUTUMN"; break;
	case 1: type_str = "BONE"; break;
	case 2: type_str = "JET"; break;
	case 4: type_str = "RAINBOW"; break;
	case 9: type_str = "HSV"; break;
	case 11: type_str = "HOT"; break;
	default:
		type_str = "";
		break;
	}
	consoleLog("Colormap", QString::fromLocal8Bit(mypicture.filename.c_str()), "Max: " + QString::number(max) + " Min: " + QString::number(min) + " Type:" + type_str, "Time cost: " + time_cost + " s");
}

void PointCloudTools::convert()
{

}

void PointCloudTools::pSliderChanged(int value)
{

}

void PointCloudTools::pSliderReleased()
{

}

void PointCloudTools::colorBtnPressed()
{
	
}

void PointCloudTools::pColormap()
{

}

void PointCloudTools::itemSelected(QTreeWidgetItem*, int)
{

}

void PointCloudTools::popMenu(const QPoint&)
{
	QTreeWidgetItem* curItem = ui.dataTree->currentItem(); //��ȡ��ǰ������Ľڵ�
	if (curItem == NULL)return;           //����������Ҽ���λ�ò���treeItem�ķ�Χ�ڣ����ڿհ�λ���һ�
	QString name = curItem->text(0);
	int id = ui.dataTree->indexOfTopLevelItem(curItem);

	QAction hideItemAction("Hide", this);
	QAction showItemAction("Show", this);
	QAction deleteItemAction("Delete", this);
	QAction changeColorAction("Change color", this);

	connect(&hideItemAction, &QAction::triggered, this, &PointCloudTools::hideItem);
	connect(&showItemAction, &QAction::triggered, this, &PointCloudTools::showItem);
	connect(&deleteItemAction, &QAction::triggered, this, &PointCloudTools::deleteItem);
	connect(&changeColorAction, &QAction::triggered, this, &PointCloudTools::pointcolorChanged);

	QPoint pos;
	QMenu menu(ui.dataTree);
	menu.addAction(&hideItemAction);
	menu.addAction(&showItemAction);
	menu.addAction(&deleteItemAction);
	menu.addAction(&changeColorAction);
	
	if (getFileType(name.toStdString()) == "ply" || getFileType(name.toStdString()) == "pcd")
	{
		for (auto it = mycloud_vec.begin(); it != mycloud_vec.end(); it++)
		{
			if (it->filename == name.toStdString())
			{
				if (it->visible == true){
					menu.actions()[1]->setVisible(false);
					menu.actions()[0]->setVisible(true);
				}
				else{
					menu.actions()[1]->setVisible(true);
					menu.actions()[0]->setVisible(false);
				}
				break;
			}
		}
	}
	else
	{
		menu.removeAction(&hideItemAction);
		menu.removeAction(&changeColorAction);
	}
		


	menu.exec(QCursor::pos()); //�ڵ�ǰ���λ����ʾ
}

void PointCloudTools::hideItem()
{

}

void PointCloudTools::showItem()
{

}

void PointCloudTools::deleteItem()
{

}

void PointCloudTools::popMenuInConsole(const QPoint&)
{

}

void PointCloudTools::clearConsole()
{

}

void PointCloudTools::enableConsole()
{

}

void PointCloudTools::disableConsole()
{

}

void PointCloudTools::initial()
{
	//�����ʼ��
	setWindowIcon(QIcon(tr(":Resources/images/logo.png")));
	setWindowTitle(tr("PointCloud Tools"));

	//���Ƴ�ʼ��
	mycloud.cloud.reset(new PointCloudT);
	mycloud.cloud->resize(1);
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));

	//���Ƶ���¼��ص�����
	clicked_points.reset(new PointCloudT);
	viewer->registerPointPickingCallback(&PointCloudTools::pp_callback, *this, NULL);

	//����ui����Ԫ�ذ�
	ui.screen->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui.screen->GetInteractor(), ui.screen->GetRenderWindow());
	ui.screen->update();
	//���ñ�����ɫ
	viewer->setBackgroundColor(30 / 255.0, 30 / 255.0, 30 / 255.0);

	ui.consoleTable->setSelectionMode(QAbstractItemView::NoSelection);		//��ֹ���������ڵ�item
	ui.dataTree->setSelectionMode(QAbstractItemView::ExtendedSelection);	//����dataTree��ѡ

	setConsoleTable();

	//����Ĭ������
	QString qss = windows_qss;
	qApp->setStyleSheet(qss);

	// �������
	consoleLog("Software start", "PointCloud Tools", "Welcome to use PointCloud Tools", "Haden");

}

void PointCloudTools::setConsoleTable(){
	// �����������
	QStringList header2;
	header2 << "Time" << "Operation" << "Operation obeject" << "Details" << "Note";
	ui.consoleTable->setHorizontalHeaderLabels(header2);
	ui.consoleTable->setColumnWidth(0, 150);
	ui.consoleTable->setColumnWidth(1, 200);
	ui.consoleTable->setColumnWidth(2, 200);
	ui.consoleTable->setColumnWidth(3, 300);

	//ui.consoleTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
	ui.consoleTable->verticalHeader()->setDefaultSectionSize(22); //�����о�

	ui.consoleTable->setContextMenuPolicy(Qt::CustomContextMenu);

}

void PointCloudTools::consoleLog(QString operation, QString object, QString details, QString note)
{
	if (enable_console == false)
		return;

	int rows = ui.consoleTable->rowCount();
	int new_rows = rows + 1;
	ui.consoleTable->setRowCount(new_rows);
	QDateTime time = QDateTime::currentDateTime();		//��ȡϵͳʱ��
	QString time_str = time.toString("MM-dd hh:mm:ss");	//��׼����ʾʱ��
	ui.consoleTable->setItem(rows, 0, new QTableWidgetItem(time_str));
	ui.consoleTable->setItem(rows, 1, new QTableWidgetItem(operation));
	ui.consoleTable->setItem(rows, 2, new QTableWidgetItem(object));
	ui.consoleTable->setItem(rows, 3, new QTableWidgetItem(details));
	ui.consoleTable->setItem(rows, 4, new QTableWidgetItem(note));

	ui.consoleTable->scrollToBottom();					//�������ײ�
}

void PointCloudTools::gray2rainbow(float value, int min, int max, uint8_t* r, uint8_t* g, uint8_t* b)
{

}

void PointCloudTools::pp_callback(const pcl::visualization::PointPickingEvent& event, void *args)
{
	if (event.getPointIndex() == -1)
		return;

	pcl::PointXYZRGBA current_point;
	event.getPoint(current_point.x, current_point.y, current_point.z);
	clicked_points->points.clear();
	clicked_points->points.push_back(current_point);

	// ��ɫ����ѡ���
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGBA> red(clicked_points, 255, 0, 0);
	viewer->removePointCloud("clicked_points");
	viewer->addPointCloud(clicked_points, red, "clicked_points");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "clicked_points");

	QString details = QString("X:%1 Y:%2 Z:%3").arg((int)current_point.x, 5, 10, QLatin1Char(' ')).arg((int)current_point.y, 5, 10, QLatin1Char(' ')).arg((int)current_point.z, 5, 10, QLatin1Char(' '));
	consoleLog("Choose point", "", details, "");
}

//��ӵ��Ƶ�viewer,����ʾ����
void PointCloudTools::showPointcloudAdd()
{
	for (int i = 0; i != mycloud_vec.size(); i++)
	{
		viewer->addPointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
		viewer->updatePointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
	}
	viewer->resetCamera();
	ui.screen->update();
}