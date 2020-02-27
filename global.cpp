#include "global.h"
#include "util.h"

GlobalParams* GlobalParams::pSingleTon = new GlobalParams();

GlobalParams* GlobalParams::getSingTonInstance()
{
	return pSingleTon;
}

GlobalParams::~GlobalParams()
{
}

int GlobalParams::initParams(string config_file)
{
	//读取配置文件
	string line;
	ifstream file(config_file);
	if (file)
	{
		while (getline(file, line)) // 按行读取信息 
		{
			vector<string> params = CommonUtil::getSingTonInstance()->split(line, "=");
			if (params.size() == 2)
			{
				if (params[0].compare("is_local_copy") == 0)
				{
					setIsLocalCopy(atoi(params[1].c_str()));
				}
				else if (params[0].compare("dest_ip") == 0)
				{
					setDestIp(params[1]);
				}
				else if (params[0].compare("ftp_path") == 0)
				{
					setFtpPath(params[1]);
				}
				else if (params[0].compare("dest_pic_path") == 0)
				{
					setDestPicpath(params[1]);
				}
				else if (params[0].compare("dest_user") == 0)
				{
					setDestUser(params[1]);
				}
				else if (params[0].compare("dest_path_level") == 0)
				{
					setDestPathLevel(atoi(params[1].c_str()));
				}
				else
				{
					
				}
			}
		}
	}
	
	return 0;
}


void GlobalParams::setIsLocalCopy(int is_local_copy)
{
	m_isLocalCopy = is_local_copy;
}

int GlobalParams::getIsLocalCopy()
{
	return m_isLocalCopy;
}
	
void GlobalParams::setDestIp(string dest_ip)
{
	m_destIp = dest_ip;
}
	
string GlobalParams::getDestIp()
{
	return m_destIp;
}

void GlobalParams::setFtpPath(string ftp_path)
{
	m_ftpPath = ftp_path;
}

string GlobalParams::getFtpPath()
{
	return m_ftpPath;
}

void GlobalParams::setDestPicpath(string dest_pic_path)
{
	m_destPicpath = dest_pic_path;
}

string GlobalParams::getDestPicpath()
{
	return m_destPicpath;
}

void GlobalParams::setDestUser(string dest_user)
{
	m_destUser = dest_user;
}

string GlobalParams::getDestUser()
{
	return m_destUser;
}

void GlobalParams::setDestPathLevel(int dest_path_level)
{
	m_destPathLevel = dest_path_level;
}

int GlobalParams::getDestPathLevel()
{
	return m_destPathLevel;
}
