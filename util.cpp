/**
* 定义通用方法
**/
#include "util.h"

#define RE_ERR 0
#define RE_OK 1

CommonUtil* CommonUtil::p_singleTon = new CommonUtil();

CommonUtil* CommonUtil::getSingTonInstance()
{
    return p_singleTon;
}
 
/*从url获取ip*/
void CommonUtil::getHostIP(char* hostIP,const char* sourceStr)
{
	char nbuf[4];
	int sn[4];
	int i,ret,start,n,m;
	int runnum=0;
	start = 0; 
 
	while (true)
	{  
		runnum++;
		//第一个数大于255的情况
		if (runnum > 1)
		{
			ret=RE_ERR;
			for (i = start+1;i < (int)strlen(sourceStr);i++)
			{
				if (sourceStr[i]<'1' || sourceStr[i]>'9')
				{
					start = i;
					ret=RE_OK;
					break;
				}
			}
 
			if(ret==RE_ERR)
			{
				break;
			}
		}  
 
		//查找第一个数为数字的 
		ret=RE_ERR;
		for (i=start;i< (int)strlen(sourceStr);i++)
		{
			if (sourceStr[i]>='1' && sourceStr[i]<='9')
			{
				start = i;
				ret=RE_OK;
				break;
			}
		}
 
		if(ret==RE_ERR)
		{
			break;
		}
 
		//在剩余的字串中是否含有三个点
		n=0;
		for (i=start;i< (int)strlen(sourceStr);i++)
		{
			if (sourceStr[i]=='.')
			{
				n++;
			}
		}
 
		if (n<3)
		{
			ret = RE_ERR;
			break;
		}
 
 
		ret=RE_ERR;
		memset(nbuf,0,sizeof(nbuf));
		for (i=start,m=0,n=0;i< (int)strlen(sourceStr);i++)
		{
 
			if (sourceStr[i]>='0' && sourceStr[i]<='9')
			{    
 
				if(n<3)
				{
					nbuf[n]=sourceStr[i];
					ret=RE_OK;
				}
				else
				{
					ret=RE_ERR;
					break;
				}
				n++;
			}
			else if (sourceStr[i]=='.')
			{
				if (strlen(nbuf)==0)
				{
					ret=RE_ERR;
					break;
				}
 
				sn[m] = atoi(nbuf);
				if (sn[m] <= 255)
				{
					m++;
					n=0;
					memset(nbuf,0,sizeof(nbuf));
					if (m==4)
					{
						ret=RE_OK;
						break;
					}  
				}
				else
				{
					ret=RE_ERR;
					break;
 
				}
			}
			else
			{
				break;
			}
		}
 
		if (m==3)
		{
			if (ret==RE_OK)
			{
				if (strlen(nbuf)==0)
				{
					ret=RE_ERR;
				}
				else
				{
					sn[3]=atoi(nbuf);
					if (sn[3] <= 255)
					{
						ret=RE_OK;
					}
					else
					{
						ret=RE_ERR;
					}
				}
			}
		}
		else
		{
			if (m!=4)
			{
				ret=RE_ERR;
			}   
		}
 
		if (ret==RE_OK)
		{
			break;
		}
		else
		{
			continue;
		}
	}
 
	if (ret==RE_OK)
	{
		sprintf(hostIP,"%u.%u.%u.%u",sn[0],sn[1],sn[2],sn[3]);
	}
}
/*
*字符分割函数
*/
std::vector<std::string> CommonUtil::split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> res;
	if(str.empty())
	{
		return res;
	}
	
	//先将要切割的字符串从string类型转换为char*类型
	char * src = new char[str.length() + 1] ; //不要忘了
	strcpy(src, str.c_str());
 
	char * dst = new char[delim.length() + 1];
	strcpy(dst, delim.c_str());
 
	char *p = strtok(src, dst);
	while(p) 
	{
		std::string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, dst);
	}

	delete[] src;
	delete[] dst;
 
	return res;
}

//linux
/*
*获取一个目录下的所有文件名
*/
int CommonUtil::getFiles(const std::string & path, std::vector<std::string> & files)
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


int CommonUtil::changeStandardToStamp(char *str_time)
{  	
	//标准时间格式例如：2016-08-02 12:12:30	
	struct tm stm;
	memset(&stm,0,sizeof(stm));

    int iYear, iMonth, iDay, iHour, iMin, iSec;
    iYear = atoi(str_time);
	iMonth = atoi(str_time + 5);  	
	iDay = atoi(str_time + 8);  	
	iHour = atoi(str_time + 11);  	
	iMin = atoi(str_time + 14);  	
	iSec = atoi(str_time + 17);   	
	stm.tm_year = iYear - 1900;  	
	stm.tm_mon = iMonth - 1;  	
	stm.tm_mday = iDay;  	
	stm.tm_hour = iHour;  	
	stm.tm_min = iMin;  	
	stm.tm_sec = iSec;   	
	//printf("%d-%0d-%0d %0d:%0d:%0d\n", iYear, iMonth, iDay, iHour, iMin, iSec);   
	return (int)mktime(&stm);  
} 

string CommonUtil::stampToStandard(const char* timeStamp, int format)
{
	char now[66];
	time_t tt;
	struct tm *ttime;
	tt = atol(timeStamp);
	ttime = localtime(&tt);
	switch (format)
	{
	case 1:
		//20190506t093912Z
		strftime(now, 64, "%Y%m%dt%H%M%Sz", ttime);
		break;
	case 2:
		strftime(now, 64, "%Y%m%d%H%M%S", ttime);
		break;
    case 3:
        strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
        break;
	default:
		break;
	}
	
    return string(now);
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

string CommonUtil::base64Encode(unsigned char* data, unsigned int len)
{
	std::string buffer;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (len--) {
		char_array_3[i++] = *(data++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
			{
				buffer += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			buffer += base64_chars[char_array_4[j]];
		}

		while ((i++ < 3))
		{
			buffer += '=';
		}
	}

	return buffer;
}

static inline bool is_base64(const char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string CommonUtil::base64Decode(std::string & encoded_string)
{
	int in_len = (int)encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

int CommonUtil::codeConvert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;


	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		return -1;
	}
	
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == (size_t)-1)
	{
		return -1;
	}
	
	iconv_close(cd);
	*pout = '\0';

	return 0;
}

long CommonUtil::getFileSize(const string& file)
{
    long filesize = -1;
	struct stat statbuff;
	if (stat(file.c_str(), &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	return filesize;
}

unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

std::string CommonUtil::urlEncode(const std::string& str)
{
	std::string strTemp;
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y = 0;
	if (x >= 'A' && x <= 'Z')
	{
		y = x - 'A' + 10;
	}
	else if (x >= 'a' && x <= 'z')
	{
		y = x - 'a' + 10;
	}
	else if (x >= '0' && x <= '9')
	{
		y = x - '0';
	}
	else
	{
	}
	return y;
}

std::string CommonUtil::urlDecode(const std::string& str)
{
	std::string strTemp;
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+')
		{
            strTemp += ' ';
        }
		else if (str[i] == '%')
		{
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

string CommonUtil::createM3u8File(string rootPath, string uuid, string ip,int duration, string start_timestamp, string vodUrl)
{
	string filePath = makeDir(rootPath, atoi(start_timestamp.c_str()), uuid, ip);
	string m3u8 = "playlist_";
	m3u8.append(start_timestamp);
	m3u8.append(".m3u8");
	filePath.append(m3u8);

	time_t tt;
	tt = atoi(start_timestamp.c_str());
	struct tm *pStartTime = localtime(&tt);
	int dwYear = pStartTime->tm_year+1900; 
	int dwMonth = pStartTime->tm_mon+1;
	int dwDay = pStartTime->tm_mday;
	//string vodUrl = GlobalParams::getSingTonInstance()->getVodPullUrl();
	string dataStr = to_string(dwYear)+"_"
		+to_string(dwMonth)+"_"
		+to_string(dwDay);
	vodUrl.append("/");
	vodUrl.append(ip);
	vodUrl.append("/");
	vodUrl.append(dataStr);
	vodUrl.append("/");
	vodUrl.append(uuid);
	vodUrl.append("/");
	vodUrl.append(start_timestamp);
	vodUrl.append("/");
	vodUrl.append(m3u8);

	if (access(filePath.c_str(), F_OK) != -1)
	{
		remove(filePath.c_str());
	}

	FILE *fp;
	if ( (fp=fopen(filePath.c_str(), "w")) == 0 )
	{
	    return "";
	}
	fputs("#EXTM3U",fp);
	fputc('\n',fp);
	fputs("#EXT-X-VERSION:3",fp);
	fputc('\n',fp);
	fputs("#EXT-X-TARGETDURATION:2",fp);
	fputc('\n',fp);
	fputs("#EXT-X-MEDIA-SEQUENCE:1",fp);
	fputc('\n',fp);

	int offset = 0;
	int i = 0;
	while(offset < duration)
	{
		string ts = "";
		string dur = "";

		ts.append("output");
		ts.append(to_string(atoi(start_timestamp.c_str())+offset));
		//ts.append(to_string(i));
		ts.append(".ts");

		/*前15片以初值为2，公差为2递增切片，16片及以后为30s切片*/
		if(i < 15)
		{
			dur = to_string((i+1)*2);
			offset = offset + 2*(i+1);
		}else{
			if((duration - offset) >= 30)
			{
				dur = to_string(30);
				offset = offset + 30;
			}else{
				dur = to_string((duration - offset));
				offset = duration+1;
			}
		}
		string prefix = "#EXTINF:";
		string suffix = ".000000,";
		prefix.append(dur);
		prefix.append(suffix);
		fputs("#EXT-X-DISCONTINUITY",fp);
		fputc('\n',fp);
		fputs(prefix.c_str(),fp);
		fputc('\n',fp);
		fputs(ts.c_str(),fp);
		fputc('\n',fp);
		i++;
	}
	fputs("#EXT-X-ENDLIST",fp);
	fclose(fp);/*关闭文件*/
	chmod(filePath, 755);
	return vodUrl;
}

void CommonUtil::copy(string in_filename, string out_filename,string rootPath)
{
	//string rootPath = GlobalParams::getSingTonInstance()->getVodCachePath();
	string command = "cp ";
	command += rootPath;
	command += "vod_patch.ts";
	command += " ";
	command += out_filename;//cp /home/file1 /root/file2
	system((char*)command.c_str());//
	return;
}

string CommonUtil::makeDir(string rootPath, int start_time, string uuid, string ip)
{
	int ret = 0;
	//点播路径：日期/设备唯一标识/*.ts
	time_t tt;
	tt = start_time;
	struct tm *pStartTime = localtime(&tt);
	int dwYear = pStartTime->tm_year+1900; 
	int dwMonth = pStartTime->tm_mon+1;
	int dwDay = pStartTime->tm_mday;

	string dataStr = to_string(dwYear)+"_"
		+to_string(dwMonth)+"_"
		+to_string(dwDay);

	//string rootPath = GlobalParams::getSingTonInstance()->getVodCachePath();
	string ipPath = rootPath.append(ip);
	ipPath.append("/");
	ret = access(ipPath.c_str(), F_OK);
	if(ret == -1)
	{
		int isCreate = mkdir(ipPath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	   	if(isCreate == -1)
	   		return "";
	}

	string dataPath = ipPath.append(dataStr);
	dataPath.append("/");
	ret = access(dataPath.c_str(), F_OK);
	if(ret == -1)
	{
		int isCreate = mkdir(dataPath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	   	if(isCreate == -1)
	   		return "";
	}

	//string fullPath = dataPath;
	
	string uuidPath = dataPath.append(uuid);
	uuidPath.append("/");
	ret = access(uuidPath.c_str(), F_OK);
	if(ret == -1)
	{
		int isCreate = mkdir(uuidPath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	   	if(isCreate == -1)
	   		return "";
	}	

	string fullPath = uuidPath.append(to_string(start_time));
	fullPath.append("/");
	ret = access(fullPath.c_str(), F_OK);
	if(ret == -1)
	{
		int isCreate = mkdir(fullPath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	   	if(isCreate == -1)
	   		return "";
	}
	
   	return fullPath;   	
}

int CommonUtil::chmod(string fileName, int mode)
{
    string cmd = "chmod ";
    cmd.append(to_string(mode));
    cmd.append(" ");
    cmd.append(fileName);
    system(cmd.c_str());
    return 0;
}

string CommonUtil::getSystemName()
{
    string str;
    //需要系统安装了redhat-lsb
    //string cmd = "lsb_release -a | grep Distributor";
    string cmd;
    char buf[1024]={0};
    FILE *fp = popen(cmd.c_str(),"r");
    fread(buf,1024,1,fp);
    pclose(fp);
    string result = buf;

    vector<string> list = CommonUtil::getSingTonInstance()->split(result, ":");
    if(list.size() == 2)
    {
        str = list[1];

        str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                            std::not1(std::ptr_fun(::isspace))));

        str.erase(std::find_if(str.rbegin(), str.rend(),
                               std::not1(std::ptr_fun(::isspace))).base(), str.end());
    }
    
    return str;
}

string CommonUtil::getFileData(const string &file)
{
    string buf;
    FILE* fp = fopen(file.c_str(), "rb");   //待编码图片
    if (NULL == fp)
    {
        return buf;
    }
    //获取图片大小
    fseek(fp, 0L, SEEK_END);
    //图片字节数
    unsigned int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //分配内存存储整个图片
    char *data = (char *)malloc(sizeof(char)*size);
    if (NULL == data)
    {
        fclose(fp);
        return buf;
    }

    //读取图片
    long result = fread(data, 1, size, fp);
    if (result != size)
    {
        free(data);
        data = NULL;
        fclose(fp);
        return buf;
    }
    fclose(fp);

    buf = std::string(data, size);
    free(data);
    data = NULL;
    return buf;
}