/**
* 定义通用方法
**/
#ifndef _UTIL_H
#define _UTIL_H 
//C
#include <dirent.h>  
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 
#include <sys/stat.h>

//C++
#include <algorithm>
#include <iostream> 
#include <string> 
#include <vector>  
#include <iconv.h>


using namespace std;

class CommonUtil {
public:
    static CommonUtil* getSingTonInstance();

    /*
    *字符分割函数
    */
    std::vector<std::string> split(const std::string &str, const std::string &delim);

    //linux
    /*
    *获取一个目录下的所有文件名
    */
    int getFiles(const std::string &path, std::vector<std::string> &files);

    /*
    *标准时间转成时间戳,//标准时间格式例如：1970-01-01 00:00:00
    */
    int changeStandardToStamp(char *str_time);

    /*
    时间戳转成标准时间
    1 - 20190506t093912Z
    2 - 20190506093912
    */
    string stampToStandard(const char *timeStamp, int format);

    /*
    base 64编码字符串
    */
    string base64Encode(unsigned char *data, unsigned int len);

    /*
    base64解码字符串
    */
    string base64Decode(std::string &encoded_string);

    /*
    字符串格式转换
    */
    int codeConvert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen);

    /*
    获取文件大小
    */
    long getFileSize(const string &file);

    /*
    从字符串中获取ip地址
    */
    void getHostIP(char *hostIP, const char *sourceStr);

    /*
    url的utf-8中文编码
    */
    std::string urlEncode(const std::string &str);

    /*
    url的utf-8中文解码
    */
    std::string urlDecode(const std::string &str);

    /**
     *create m3u8 file
     *@param rootPath: m3u8 file location
     *param  uuid:unique Identification
     *@param ip: 
     *@param duration: m3u8 file duration
     *@param start_timestamp: m3u8 file start time 
     *@param vodUrl: m3u8 file root url
    */
    string  createM3u8File(string rootPath, string uuid, string ip,int duration, string start_timestamp, string vodUrl);

    /**
     *create file dir
     *@param rootPath: m3u8 file location
     *param  uuid:
     *@param ip: 
     *@param start_time: 
    */
    string  makeDir(string rootPath, int start_time, string uuid, string ip);

    /**
     *copy file from in_filename to out_filename
     *@param rootPath: m3u8 file location
     *param  in_filename:
     *@param out_filename: 
    */
    void copy(string in_filename, string out_filename, string rootPath);

    /**
     * 修改文件权限
     * @param fileName
     * @param mode
     * @return
     */
    int chmod(string fileName, int mode);

    /**
     * 获取操作系统
     * @return
     */
    string getSystemName();

    /**
     * 获取文件数据
     * @param file
     * @return
     */
    string getFileData(const string &file);
private:
    static CommonUtil* p_singleTon;
};

#endif
