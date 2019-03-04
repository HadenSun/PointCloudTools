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
	string fullname;		//����·��
	string filename;		//�ļ���
	string dirname;			//�ļ���·��
	string filetype;		//�ļ�����
	bool isConvert = false;	//�Ƿ�ת����
};
