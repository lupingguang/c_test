#include <stdio.h>
#include <string.h>
void hexdump(char *_data, unsigned int len)
{
	unsigned char *data = _data;
	unsigned count;
	for( count = 0; count < len; count++)
	{
		//0\16\32\.... low 4 bit is 0, and with 1111 is always 0000
		if((count & 0b1111) == 0)
		{
			fprintf(stderr, "[%04d]: ", count);
		}
		fprintf(stderr, "|%02x %c", *data, (*data < 32) || (*data > 126)? '.' : *data);
	
		data++;
		//15\31\47\.... low 4 bit is 1, and with 1111 is always 1111
		if((count & 0b1111) == 15)
                {
                        fprintf(stderr, "|\n");
                }
	}

        if((count / 15) != 0)
        {
                fprintf(stderr, "\n");
        }

}

int main (void)
{
	unsigned char a[40];
	a[0] = 'a';
	a[1] = 'b';
	a[2] = 'c';
	a[3] = 0x10;
	a[4] = '~';
	a[5] = 'A';
        a[6] = 'B';
        a[7] = 'C';
        a[8] = 0x86;
        a[9] = '~';
	a[10] = 'a';
        a[11] = 'b';
        a[12] = 'c';
        a[13] = 0x11;
        a[14] = '~';
        a[15] = 'A';
        a[16] = 'B';
        a[17] = 'C';
        a[18] = 0x87;
        a[19] = '~';
	
	a[20] = 'a';
        a[21] = 'b';
        a[22] = 'c';
        a[23] = 0x11;
        a[24] = '~';
        a[25] = 'A';
        a[26] = 'B';
        a[27] = 'C';
        a[28] = 0x87;
        a[29] = '~';

        a[30] = 'a';
        a[31] = 'b';
        a[32] = 'c';
        a[33] = 0x11;
        a[34] = '~';
        a[35] = 'A';
        a[36] = 'B';
        a[37] = 'C';
        a[38] = 0x87;
        a[39] = '~';

	hexdump(a, 40);
}
