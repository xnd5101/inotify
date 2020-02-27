/*
global params
*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 
#include <sys/time.h>
#include <time.h>
#include <fstream>
//C++
#include <iostream>  
#include <map>
#include <string>
#include <thread>
#include <pthread.h>
#include <vector>
#include <deque>
#include <queue> 
#include <list>
#include <exception>
#include <functional>
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

using namespace std;


class GlobalParams
{
public:
	~GlobalParams();
	static GlobalParams* getSingTonInstance();
	int initParams(string file);

	void setIsLocalCopy(int is_local_copy);
	int getIsLocalCopy();
	
	void setDestIp(string dest_ip);
	string getDestIp();

	void setFtpPath(string ftp_path);
	string getFtpPath();

	void setDestPicpath(string dest_pic_path);
	string getDestPicpath();

	void setDestUser(string dest_user);
	string getDestUser();

	void setDestPathLevel(int dest_path_level);
	int getDestPathLevel();

private:
	GlobalParams() {}
	static GlobalParams* pSingleTon;
	
	int m_isLocalCopy;
	string m_destIp;
	string m_ftpPath;
	string m_destPicpath;
	string m_destUser;
	int m_destPathLevel;
};
#endif
