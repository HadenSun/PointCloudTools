#include "Tools.h"

QTime time;

Tools::Tools()
{
}

Tools::~Tools()
{
}

//��ȡȫ·���е��ļ���������׺��
std::string getFileName(std::string file_name)
{
	std::string subname;
	for (auto i = file_name.end() - 1; *i != '/'; i--)
	{
		subname.insert(subname.begin(), *i);
	}
	return subname;
}

//��ȡȫ·���е��ļ���׺��
std::string getFileType(std::string file_name)
{
	std::string subname;
	for (auto i = file_name.end() - 1; *i != '.'; i--)
	{
		subname.insert(subname.begin(), *i);
	}
	
	return subname;
}

//��ȡ·�����ļ���·��
std::string getFileDir(std::string file_name)
{
	std::string subname = getFileName(file_name);
	std::string dirname = file_name.substr(0, file_name.size() - subname.size());

	return dirname;
}

//��ʱ��ʼ
void timeStart()
{
	time.start();
}

//��ʱ����
QString timeOff()
{
	int timediff = time.elapsed();   //���ش��ϴ�start()��restart()��ʼ������ʱ����λms
	float f = timediff / 1000.0;
	QString tr_timediff = QString("%1").arg(f);  //float->QString
	return tr_timediff;
}
