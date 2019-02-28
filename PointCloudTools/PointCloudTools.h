#pragma once

#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h>
#include <pcl/io/obj_io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/common/common.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/features/normal_3d.h>
#include <pcl/search/kdtree.h>
#include <pcl/surface/gp3.h>

#include <QtWidgets/QMainWindow>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "QVTKWidget.h"
#include <vtkRenderWindow.h>
#include <QTextEdit>
#include <QTime>
#include <QMouseEvent> 
#include <string>
#include "tools.h"
#include "AboutWin.h"
#include "MyCloud.h"
#include "MyPicture.h"
#include "ColormapWin.h"
#include "ui_PointCloudTools.h"

class PointCloudTools : public QMainWindow
{
	Q_OBJECT

public:
	PointCloudTools(QWidget *parent = Q_NULLPTR);

private:
	Ui::PointCloudToolsClass ui;

	MyCloud mycloud;
	MyPicture mypicture;
	vector<MyCloud> mycloud_vec;
	vector<MyPicture> mypicture_vec;

	bool enable_console = true;		//console����״̬
	bool save_as_binary = false;	//�����Ʊ���


	/***** Slots of QMenuBar and QToolBar *****/
	// File menu slots
	void open();
	void clear();
	void save();
	void saveBinary();
	void exit();
	// Display menu slots
	void pointcolorChanged();
	void bgcolorChanged();
	void mainview();
	void leftview();
	void topview();
	// View menu slots
	void data();
	void properties();
	void console();
	void rgbDock();
	// Generate menu slots
	void cube();
	void createSphere();
	void createCylinder();
	// Process menu slots 
	int convertSurface();  //���߹��ơ������ؽ���������Ƭ��ʾ
	int convertWireframe(); //���߹��ơ������ؽ��������߿���ʾ
	// Option menu slots
	void windowsTheme();
	void darculaTheme();
	void langEnglish();
	void langChinese();
	// About menu slots
	void about();
	void help();

	/***** Methods ******/
	void initial();			//��ʼ��
	void consoleLog(QString operation, QString object, QString details, QString note);		//����̨��ʾ����
	void gray2rainbow(float value, int min, int max, uint8_t* r, uint8_t* g, uint8_t* b);	//α��ɫת��
	

public slots:
	/***** Slots of image widget *****/
	void colormapBtnPressed();
	void convertBtnPressed();
	void colormap(ColormapClass cc);
	void convert();

	/***** Slots of RGB widget *****/
	// Change color or size of cloud when slider is released or colorBtn is pressed
	void pSliderChanged(int value);
	void pSliderReleased();
	void colorBtnPressed();
	// colorMap slot
	void pColormap();

	/***** Slots of dataTree(QTreeWidget) widget *****/
	// Item in dataTree is left-clicked
	void itemSelected(QTreeWidgetItem*, int);
	// Item in dataTree is right-clicked
	void popMenu(const QPoint&);
	void hideItem();
	void showItem();
	void deleteItem();

	/***** Slots of console(QTreeWidget) widget *****/
	void popMenuInConsole(const QPoint&);
	void clearConsole();
	void enableConsole();
	void disableConsole();

	
};
