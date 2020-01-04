#include "binRwClass.h"
#include <string.h>
#include <unistd.h>


using namespace std;

FILE* StorageFile::mFileArray[StorageFile::scMaxFile] = {0};
bool StorageFile::mInit = false;
unsigned char StorageFile::mFileId =0;
string StorageFile::mPath = string("haha");
signed int StorageFile::init()
{
    unsigned char id = 0;
    for (; id<scMaxFile; id++)
    {
        if(0 == mFileArray[id])
	{
	    break;
	}
    }
    if (scMaxFile == id)
    { 
	printf("Error! %d file have been opened!",scMaxFile);
    }
    mFileId = id;
    mFileArray[mFileId] = fopen(mPath.c_str(), "r+b");
    if (0 == mFileArray[mFileId])
    {
	mFileArray[mFileId] = fopen(mPath.c_str(), "w+b");
	if(0 == mFileArray[mFileId])
	{
	     printf("Erroe! %s open failed!",mPath.c_str());
	     mFileId = StorageFile::scMaxFile -1;
	     return -1;
	}
    }
    if(mInit)
    {
	return 0;
    }
    mInit = true;
    return 0;
}
signed int StorageFile::deinit()
{
    if ((false == mInit) || (0 == mFileArray[mFileId]))
    {
	return -1;
    }
    return 0;
}

signed int StorageFile::read(unsigned int pos, unsigned char* pData, unsigned short size)
{
    int readSize =0;
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open", mPath.c_str());
    }
    //using 10 bytes to save mark[storage addr],so read start addr is pos + 10
    fseek(mFileArray[mFileId], pos + 10, SEEK_SET);
    readSize = fread(pData,1,size,mFileArray[mFileId]);
    if (readSize != (unsigned int )size)
    {
	memset(pData, 0x00, size);
	return -1;
    }
    return 0;
}

signed int StorageFile::write(unsigned int pos, const unsigned char* pData, unsigned short size)
{
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open", mPath.c_str());
    }
    Data_Info_s stDataInfo = {0,0,0,0};
    stDataInfo.pData = new (std::nothrow) unsigned char[size];
    if(0 == stDataInfo.pData)
    {
	printf("Error!,mem alloc fail");
    }   
    memcpy(stDataInfo.pData, pData, size);
    stDataInfo.addrPos = pos;
    stDataInfo.size = size;
    stDataInfo.id   = mFileId;
    save(stDataInfo.id, stDataInfo.addrPos, stDataInfo.pData, stDataInfo.size);
    return 0;
}
signed int StorageFile::clear()
{
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open", mPath.c_str());
    }
    mFileArray[mFileId] = freopen(mPath.c_str(), "w+b",mFileArray[mFileId]);  
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s fail re-open", mPath.c_str());
   	return -1;
    }
    return 0;
}
//size is position & context of position
signed int StorageFile::mark(unsigned int size)
{
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open\n", mPath.c_str());
    }

    fseek(mFileArray[mFileId], size,SEEK_SET);
    int writeSize = fwrite(&size,1,sizeof(unsigned int),mFileArray[mFileId]);
    if (writeSize != sizeof(unsigned int ))
    {
	printf("Error!mark failed!\n");
	return -1;
    } 
    fflush(mFileArray[mFileId]);
    int fd = fileno(mFileArray[mFileId]);
    fsync(fd);
    return 0; 
}
//read position of "size",if rerurn value = size verify success
signed int StorageFile::verify(unsigned int size)
{
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open\n", mPath.c_str());
    }
    fseek(mFileArray[mFileId], size,SEEK_SET);
    unsigned int readValue = 0;
    int readSize =0;
    fread(&readValue, sizeof(unsigned int), 1, mFileArray[mFileId]);
    if(readSize != 1 || readValue != size)
    {
	printf("Error! Mark verify failed!\n");
	return -1;
    } 
    return 0;
}

signed int StorageFile::save(unsigned char id, unsigned int addrPos, const unsigned char* pData, unsigned short size)
{
    if(0 == mFileArray[mFileId])
    {
	printf("Error!,file %s is not open", mPath.c_str());
    }
    //using 10 bytes to save mark[storage addr],so write start addr is pos + 10
    fseek(mFileArray[mFileId], addrPos + 10,SEEK_SET);  
    int writeSize = fwrite(pData,1,size,mFileArray[mFileId]);
    if(writeSize != size)
    {
	printf("Error!,fileID:%u,data pos:%u!",id, addrPos);
    }
    fflush(mFileArray[id]);
    int fd = fileno(mFileArray[mFileId]);
    fsync(fd);
    return 0; 
}



