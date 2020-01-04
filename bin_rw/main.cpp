#include<fstream>
#include<string.h>
#include"binRwClass.h"

//when add new main storage pos, we should add verifyBin and mark opt
//further more, each pos can be spetated to small sub pos
#define MY_USER_MAIN_POS 5 
#define MY_USER_SUB_POS1  MY_USER_MAIN_POS + 20
#define MY_USER_SUB_POS2  MY_USER_SUB_POS1 + 20
#define MY_USER_SUB_POS3  MY_USER_SUB_POS2 + 20
#define MY_USER_MAIN_POS1 MY_USER_SUB_POS3 + 20

#define BIN_NAME    "test.bin"

typedef struct
{
 unsigned int chksum;
 unsigned int v1;
 unsigned int v2;
 unsigned int v3;
 unsigned int v4;

}MY_DATA_S;

void genCheckSum(unsigned char *buf, unsigned int bufLen, unsigned int &checksum)
{
    checksum =0;
    while (--bufLen)
    {
	checksum +=buf[bufLen];
    }
    checksum +=buf[0];
}

int verifyBin(unsigned int pos, const char* path)
{
    std::fstream bin_stream;
    bin_stream.open(path, ios::in);
    if(!bin_stream)
    {
        bin_stream.close();
        // printf("bin_stream open error\n");
        return -1;
    }
    else
    {
 	bin_stream.close();
        FILE* fp = fopen(path, "r+b");
        if(0 == fp)
        {
            //printf("FILE  open error\n");
	    return -1;
	}
        else
        {
	    fseek(fp, pos, SEEK_SET);
            unsigned int readValue =0;
            int readSize = fread(&readValue, sizeof(unsigned int), 1, fp);

	    if(readValue != pos )
            {
		fclose(fp);
		printf("[bin:%s][pos:%d][readValue:%x]error, verify failed\n",path,pos,readValue);
                remove(path);
	        return -1;
	    }
            else
            {
	        printf("[bin:%s][pos:%d][readValue:%x]verify success\n",path,pos,readValue);
		fclose(fp);
                return 0;  
	    }
	}
    }
}
int main(int argc, char** argv)
{
    MY_DATA_S wBuf = {0,4,3,2,1};
    MY_DATA_S rBuf = {0,0,0,0,0};
    unsigned int chksum = 0x09;
    
    // first run app or  user storage pos change, verify will failed,because bin is not exist or storage is not mark
    // this can help create new bin by below
    int ret =  verifyBin(MY_USER_MAIN_POS, BIN_NAME);
    ret    |=  verifyBin(MY_USER_MAIN_POS1, BIN_NAME);
    //open or create bin
    StorageFile worker(std::string(BIN_NAME));
    worker.init();
 
    if(ret !=0)
    {
        // if verify failed, set mark once, when app run again will verify success
        // marking opt is used for marking each storage start pos[really storage pos is +10 bytes offset]
        worker.mark(MY_USER_MAIN_POS);
        worker.mark(MY_USER_MAIN_POS1);
    }

    if (argc == 2)
    {
	//store to bin,mark context will be saved
        genCheckSum((unsigned char*)&wBuf +4, sizeof(MY_DATA_S),chksum);
        memcpy((unsigned char*)&wBuf,(unsigned char*)&chksum,4);

	worker.write(MY_USER_MAIN_POS,(unsigned char*)&wBuf,sizeof(MY_DATA_S));
        worker.write(MY_USER_MAIN_POS1,(unsigned char*)&wBuf,sizeof(MY_DATA_S));
    } 
    //read from bin
    worker.read(MY_USER_MAIN_POS, (unsigned char*)&rBuf, sizeof(MY_DATA_S));
    printf("chk:%d,v1:%d,v2:%d,v3:%d,v4:%d\n",rBuf.chksum,rBuf.v1,rBuf.v2,rBuf.v3,rBuf.v4);
}
