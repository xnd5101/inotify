#include "monitor.h"

#define BUF_LEN 10000

CMonitor* CMonitor::pSingleTon = new CMonitor();

CMonitor* CMonitor::getSingTonInstance()
{
	return pSingleTon;
}

CMonitor::CMonitor()
{

}

CMonitor::~CMonitor()
{

}

int CMonitor::init()
{
	char buf[BUF_LEN];
	ssize_t numRead;
	char *p;
	struct inotify_event *event;
	int flags;

	inotifyFd = inotify_init();
	if(inotifyFd == -1)
	{
		printf("初始化失败");
		return 0;
	}

	string ftpPath = GlobalParams::getSingTonInstance()->getFtpPath();
 
	int wd = inotify_add_watch(inotifyFd, ftpPath.c_str(), IN_ALL_EVENTS);
	if(wd == -1)
	{
		printf("inotify_add_watch error\n");
		return 0;
	}
 
	printf("Watching %s using wd %d\n",ftpPath.c_str(), wd);
	scan_loop_listen(ftpPath);
 
	while(1)
	{
		numRead = read(inotifyFd,buf,BUF_LEN);
		if(numRead == -1)
		{
			printf("read error\n");
		}
 
		printf("Read %ldbytes from inotify fd\n",(long)numRead);
		for(p=buf;p < buf+numRead;)
		{
			event = (struct inotify_event *)p;
			displayInotifyEvent(event, ftpPath);
			p+=sizeof(struct inotify_event) + event->len;
		}
	}

	return 0;
}

void CMonitor::scan_loop_listen(string path)
{
//循环遍历传入的目录
	bool bRet = isDir(path);
	if(!bRet)
	{
		return;
	}
	std::vector<string> fileList;
	int ret = getFiles(path, fileList);
	if(!fileList.empty())
	{
		cout << "not empty dir" << endl;
		for(int i = 0; i < fileList.size();i++)
		{
			string subPath = path + "/" + fileList[i];
			bool bRet = isDir(subPath);
			if(bRet)
			{
				cout << "subPath:" << subPath << endl;
				//目录建立监听
				std::thread th(scan_thread, subPath, this);
				th.detach();
			}
			scan_loop_listen(subPath);
		}
	}
}

void CMonitor::displayInotifyEvent(struct inotify_event *i, string path)
{
	printf("  wd = %2d; ",i->wd);
 
	if(i->cookie > 0)
	{
		printf("cokkie = %4d; ",i->cookie);
	}
 
	//printf("mask = ");
 
 	if(i->len > 0)
	{
		printf("name = %s\n",i->name);

		if(i->mask & IN_OPEN)   
		{
			printf("IN_OPEN\n");
			time_t tt = time(0);
			cout << "time:" << tt << endl;
		}

		if(i->mask & IN_CREATE)   
		{
			printf("IN_CREATE\n");
			int64_t tt = time(0);
			cout << "time:" << tt << endl;
			string subPath = path + string(i->name);
			bool bRet = isDir(subPath);
			if(bRet)
			{
				//目录建立监听
				std::thread th(scan_thread, subPath, this);
				th.detach();
			}
			else
			{

			}
		}

		if(i->mask & IN_MODIFY)
		{
			printf("IN_MODIFY\n");
		}  

		if(i->mask & IN_CLOSE_WRITE)   
		{
			printf("IN_CLOSE_WRITE\n");
			int64_t tt = time(0);
			cout << "time:" << tt << endl;
			//复制数据
			int type = GlobalParams::getSingTonInstance()->getIsLocalCopy();
			string destPath = GlobalParams::getSingTonInstance()->getDestPicpath();
			string destIp = GlobalParams::getSingTonInstance()->getDestIp();
			string user = GlobalParams::getSingTonInstance()->getDestUser();
			int level = GlobalParams::getSingTonInstance()->getDestPathLevel();
			string tmpPath;
			std::vector<CWatchPath> watchPath = CMonitor::getSingTonInstance()->getWatchPath();
			for(int j= 0; j < watchPath.size();j++)
			{
				if(watchPath[j].wd == i->wd)
				{
					tmpPath = watchPath[j].path;
					break;
				}
			}
			cout << "tmpPath---" << tmpPath << endl;

			if(type == 1)//本地复制
			{
				string cmd = "cp ";
				cmd += tmpPath;
				cmd += "/";
				cmd += i->name;
				cmd += " ";
				cmd += destPath;
				cout << "cmd:" << cmd << endl;
				system((char*)cmd.c_str());
			}
			else if(type == 2)//跨机器复制
			{
				string cmd = "scp -r ";
				cmd += tmpPath;
				cmd += "/";
				cmd += i->name;
				cmd += " ";
				cmd += user;
				cmd += "@";
				cmd += destIp;
				cmd += ":";
				cmd += destPath;
				if(level == 1)
				{
					std::vector<string> strList = CommonUtil::getSingTonInstance()->split(tmpPath, "/");
					string cam_ip = strList[strList.size() - 1];
					cout << "cam_ip:" << cam_ip << endl;
					cmd += "/";
					cmd += cam_ip;
					cmd += "/";
				}
				cout << "cmd:" << cmd << endl;
				system((char*)cmd.c_str());
			}
			else
			{

			}
		}


		if(i->mask & IN_ACCESS)   
		{
			printf("IN_ACCESS\n");
		}

		if(i->mask & IN_DELETE_SELF)   
		{
			printf("IN_DELETE_SELF\n");
		}
		

		if(i->mask & IN_ATTRIB)   
		{
			printf("IN_ATTRIB\n");
		}
		
		if(i->mask & IN_CLOSE_NOWRITE)   
		{
			printf("IN_CLOSE_NOWRITE\n");
		}
		
		if(i->mask & IN_DELETE)   
		{
			printf("IN_DELETE\n");
		}
		if(i->mask & IN_MOVE_SELF)   
		{
			printf("IN_MOVE_SELF\n");
		}
		if(i->mask & IN_MOVED_FROM)   
		{
			printf("IN_MOVED_FROM\n");
		}
		if(i->mask & IN_MOVED_TO)   
		{
			printf("IN_MOVED_TO\n");
		}
	}
}

void* CMonitor::scan_thread(string path, void* ptr)
{
	CMonitor* p_monitor = static_cast<CMonitor *>(ptr);
	char buf[BUF_LEN];
	ssize_t numRead;
	char *p;
	struct inotify_event *event;

	int wd = inotify_add_watch(p_monitor->inotifyFd, path.c_str(), IN_ALL_EVENTS);
	if(wd == -1)
	{
		printf("error\n");
	}
	CWatchPath item;
	item.wd = wd;
	item.path = path;
	p_monitor->m_watchPath.push_back(item);
 
	printf("Watching %s using wd %d\n",path.c_str(),wd);
 
	while(1)
	{
		numRead = read(p_monitor->inotifyFd, buf, BUF_LEN);
		if(numRead == -1)
		{
			printf("read error\n");
		}
 
		for(p=buf;p < buf+numRead;)
		{
			event = (struct inotify_event *)p;
			p_monitor->displayInotifyEvent(event, path);
			p+=sizeof(struct inotify_event) + event->len;
		}
	}
}

int CMonitor::getFiles(const std::string & path, std::vector<std::string> & files)
{
	DIR *dir;  
	struct dirent *ptr;  

	if ((dir = opendir(path.c_str())) == NULL)  
    {  
    	perror("Open dir error...");  
        return -1; 
    }  

	while ((ptr = readdir(dir)) != NULL)  
    {  
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir  
        {
           continue;  
    	}
        else if(ptr->d_type == 8)    ///file 
    	{
        	files.push_back(ptr->d_name);  
    	}
        else if(ptr->d_type == 10)    ///link file  
    	{
        	continue;  
    	}
        else if(ptr->d_type == 4)    ///dir  
        {  
            files.push_back(ptr->d_name);   
        }  
    }  
    closedir(dir);

	sort(files.begin(), files.end()); 
     return 0; 
}

bool CMonitor::isDir(string path)
{
	struct stat buffer; 
	if (stat(path.c_str(), &buffer) < 0) { 
	    return false;
	}

	if (S_ISDIR(buffer.st_mode)) {
	    return true;
	}

	return false;
}

std::vector<CWatchPath> CMonitor::getWatchPath()
{
	return m_watchPath;
}