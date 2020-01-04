#ifndef __BINRW_H__
#define __BINRW_H__

#include <string>
#include <list>
#include <stdio.h>


using namespace std;

class StorageFile
{
    struct Data_Info_s
    {
	unsigned int addrPos;
        unsigned char* pData;
        unsigned char id;
        unsigned short size;
    };
public:
    StorageFile(const string& path);
    ~StorageFile();
    signed int init();
    signed int deinit();
    signed int read(unsigned int pos, unsigned char* pData, unsigned short size);
    signed int write(unsigned int pos, const unsigned char* pData, unsigned short size);
    signed int clear();
    signed int mark(unsigned int size);
    signed int verify(unsigned int size);
private:
    static const unsigned char scMaxFile = 16;
    static string mPath;
    static unsigned char mFileId;
    static FILE*  mFileArray[StorageFile::scMaxFile];
    static bool   mInit;
    static signed int save(unsigned char id, unsigned int addrPos, const unsigned char* pData, unsigned short size);
};
inline StorageFile::StorageFile(const string& path)
{
   mFileId =0;
   mPath = path;
   printf("%s\n",mPath.c_str());
}
inline StorageFile::~StorageFile(){}
#endif
