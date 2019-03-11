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
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>

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
#include "ReconstructionWin.h"
#include "VoxelGridWin.h"
#include "FilterWin.h"
#include "ui_PointCloudTools.h"

class PointCloudTools : public QMainWindow
{
	Q_OBJECT

public:
	PointCloudTools(QWidget *parent = Q_NULLPTR);

private:
	Ui::PointCloudToolsClass ui;

	MyCloud* mycloud;
	MyPicture* mypicture;
	vector<MyCloud*> mycloud_vec;
	vector<MyPicture*> mypicture_vec;

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	PointCloudT::Ptr clicked_points;

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

	// Option menu slots
	void windowsTheme();
	void darculaTheme();
	void langEnglish();
	void langChinese();
	// About menu slots
	void about();
	void help();

	/***** Methods ******/
	void initial();				//��ʼ��
	void setConsoleTable();		//���ÿ���̨����
	void setConvertParameters();	//��config.ini��ȡ�ڲΡ��������
	void consoleLog(QString operation, QString object, QString details, QString note);		//����̨��ʾ����
	void gray2rainbow(float value, int min, int max, uint8_t* r, uint8_t* g, uint8_t* b);	//α��ɫת��
	void pp_callback(const pcl::visualization::PointPickingEvent& event, void *args);		//�����Ӧ�¼�
	void showPointcloudAdd();																//��ӵ��Ƶ�viewer����ʾ
	void setCloudColor(unsigned int r, unsigned int g, unsigned int b);						//�������е�����ɫ
	void setA(unsigned int a);																//�������е�͸����
	int saveFile(bool save_as_binary, QString save_filename);

public slots:
	/***** Slots of image widget *****/
	void colormapBtnPressed();
	void convertBtnPressed();
	void surfaceBtnPressed();
	void filterBtnPressed();
	void voxelBtnPressed();
	void wireframeBtnPressed();
	// Process menu slots 
	void colormap(ColormapClass cc);
	void convert();	
	void convertSurface(ReconstructionClass rc);	//���߹��ơ������ؽ���������Ƭ��ʾ
	void convertWireframe(ReconstructionClass rc); //���߹��ơ������ؽ��������߿���ʾ
	void convertFilter(FilterClass fc);	//����ͳ���˲�
	void convertVoxel(VoxelGridClass vc);		//���ؽ�����

	/***** Slots of RGB widget *****/
	// Change color or size of cloud when slider is released or colorBtn is pressed
	void pSliderChanged(int value);
	void pSliderReleased();
	void colorBtnPressed();
	// colorMap slot
	void pColormap();
	void cooCbxChecked(int value);

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
