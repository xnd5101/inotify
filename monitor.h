#ifndef _MONITOR_H
#define _MONITOR_H

#include<stdio.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/inotify.h>
#include <sys/time.h>
#include<limits.h>
#include<fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <dirent.h>  
#include <time.h>
#include <sys/stat.h>

#include <iostream>
#include <string>
#include <thread>
#include <ctime>
#include <vector>
#include <iconv.h>
#include <algorithm>

#include "global.h"
#include "util.h"
using namespace std;

class CWatchPath
{
public:
	int wd;
	string path;
};

class CMonitor
{
public:
	CMonitor();
	~CMonitor();
	static CMonitor* getSingTonInstance();
	std::vector<CWatchPath> getWatchPath();

	int init();
private:
	void scan_loop_listen(string path);
	void displayInotifyEvent(struct inotify_event *i, string path);
	static void* scan_thread(string path, void* ptr);
	bool isDir(string path);
	int getFiles(const std::string & path, std::vector<std::string> & files);
private:
	static CMonitor* pSingleTon;
	int inotifyFd;
	std::vector<CWatchPath> m_watchPath;

};

#endif