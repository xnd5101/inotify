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


using namespace std;

#include "global.h"
#include "monitor.h"
 
 
int main(int argc,char **argv)
{
	string config_path = "config";
	int ret = GlobalParams::getSingTonInstance()->initParams(config_path);
    if (ret < 0)
    {
        cout << "Failed in initializing config!" << endl;
        exit(0);
    }

    ret = CMonitor::getSingTonInstance()->init();
    if(ret < 0)
    {
    	cout << "monitor server init fail!" << endl;
    	exit(0);
    }

    while(true)
    {
        sleep(1);
    }

    return 0;
}
