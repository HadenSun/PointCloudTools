#pragma once

#include <string>
#include <opencv2\opencv.hpp>

using namespace std;

class MyPicture 
{


public:
	MyPicture();
	~MyPicture();

	cv::Mat depthMat;
	cv::Mat colorMat;
	string dirname;			//�ļ�Ŀ¼
	string fullname;		//����·��
	string filename;		//�ļ�����������׺��
	string filetype;		//�ļ�����
};
