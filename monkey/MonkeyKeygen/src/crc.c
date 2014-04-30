/*****************************
 *
 *	CRC32 by Cyclops
 *	+ Poly, string crc
 *
 *****************************/
  
#include <stdio.h>

unsigned long getcrc(char *name);
void crcgen();
unsigned long crcTable[256];

void CRC32(char *str,char *hash)
{
	crcgen();
	sprintf(hash,"%08X", getcrc(str));
	return;
}
unsigned long getcrc(char *name)
{
	register unsigned long crc;
	int c;
	int i=0;
	crc = 0xFFFFFFFF;
	while((c=name[i++])!='\0')
	{
		crc = ((crc>>8) & 0x00FFFFFF) ^ crcTable[ (crc^c) & 0xFF ];
	}
	return( crc^0xFFFFFFFF );
}
void crcgen()
{
	unsigned long	crc, poly;
	int	i, j;

	poly = 0xEDB88320L;
	for (i=0; i<256; i++) {
		crc = i;
		for (j=8; j>0; j--) {
			if (crc&1) {
				crc = (crc >> 1) ^ poly;
			} else {
				crc >>= 1;
			}
		}
		crcTable[i] = crc;
	}
}

