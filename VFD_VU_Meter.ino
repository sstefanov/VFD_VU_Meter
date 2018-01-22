/*
VU meter 16x1
24 custom characters for  L and R
*/

/*
  0:    1:    2:    3:    4:    5:
  00000 70000 77000 77700 77770 77777
  70000 70000 70000 70000 70000 70000
0 X.... X.... X.... X.... X.... X.... 
1 X.... X.... X.... X.... X.... X....
2 X.... X.... X.... X.... X.... X....
3 ..... ..... ..... ..... ..... .....
4 ..... X.... XX... XXX.. XXXX. XXXXX
5 ..... X.... XX... XXX.. XXXX. XXXXX
6 ..... X.... XX... XXX.. XXXX. XXXXX


  6:    7:    8:    9:    a:    b:

0 XX... XX... XX... XX... XX... XX... 
1 XX... XX... XX... XX... XX... XX...
2 XX... XX... XX... XX... XX... XX...
3 ..... ..... ..... ..... ..... .....
4 ..... X.... XX... XXX.. XXXX. XXXXX
5 ..... X.... XX... XXX.. XXXX. XXXXX
6 ..... X.... XX... XXX.. XXXX. XXXXX


  c:    d:    e:    f:    10:   11:

0 XXX.. XXX.. XXX.. XXX.. XXX.. XXX.. 
1 XXX.. XXX.. XXX.. XXX.. XXX.. XXX..
2 XXX.. XXX.. XXX.. XXX.. XXX.. XXX..
3 ..... ..... ..... ..... ..... .....
4 ..... X.... XX... XXX.. XXXX. XXXXX
5 ..... X.... XX... XXX.. XXXX. XXXXX
6 ..... X.... XX... XXX.. XXXX. XXXXX


  12:   13:   14:   15:   16:   17:

0 XXXX. XXXX. XXXX. XXXX. XXXX. XXXX. 
1 XXXX. XXXX. XXXX. XXXX. XXXX. XXXX.
2 XXXX. XXXX. XXXX. XXXX. XXXX. XXXX.
3 ..... ..... ..... ..... ..... .....
4 ..... X.... XX... XXX.. XXXX. XXXXX
5 ..... X.... XX... XXX.. XXXX. XXXXX
6 ..... X.... XX... XXX.. XXXX. XXXXX


  18:   19:   1a:   1b:   1c:   1d:

0 XXXXX XXXXX XXXXX XXXXX XXXXX XXXXX
1 XXXXX XXXXX XXXXX XXXXX XXXXX XXXXX
2 XXXXX XXXXX XXXXX XXXXX XXXXX XXXXX
3 ..... ..... ..... ..... ..... .....
4 ..... X.... XX... XXX.. XXXX. XXXXX
5 ..... X.... XX... XXX.. XXXX. XXXXX
6 ..... X.... XX... XXX.. XXXX. XXXXX


*/

#include <SPI.h>
#include "M162SD1XAA.h"

M162SD13AA vfd;

#define DEBUG

#define CSPIN 10
// high nibble - l
// low nibble - r


const uint8_t ccharset[36][5]={
//l=0                              r 
	{0x00,0x00,0x00,0x00,0x00},	// 0
	{0x07,0x00,0x00,0x00,0x00}, // 1
	{0x07,0x07,0x00,0x00,0x00}, // 2
	{0x07,0x07,0x07,0x00,0x00}, // 3
	{0x07,0x07,0x07,0x07,0x00}, // 4
	{0x07,0x07,0x07,0x07,0x07}, // 5
//l=1
	{0x70,0x00,0x00,0x00,0x00},
	{0x77,0x00,0x00,0x00,0x00},
	{0x77,0x07,0x00,0x00,0x00},
	{0x77,0x07,0x07,0x00,0x00},
	{0x77,0x07,0x07,0x07,0x00},
	{0x77,0x07,0x07,0x07,0x07},
//l=2
	{0x70,0x70,0x00,0x00,0x00},
	{0x77,0x70,0x00,0x00,0x00},
	{0x77,0x77,0x00,0x00,0x00},
	{0x77,0x77,0x07,0x00,0x00},
	{0x77,0x77,0x07,0x07,0x00},
	{0x77,0x77,0x07,0x07,0x07},
//l=3
	{0x70,0x70,0x70,0x00,0x00},
	{0x77,0x70,0x70,0x00,0x00},
	{0x77,0x77,0x70,0x00,0x00},
	{0x77,0x77,0x77,0x00,0x00},
	{0x77,0x77,0x77,0x07,0x00},
	{0x77,0x77,0x77,0x07,0x07},
// l=4
	{0x70,0x70,0x70,0x70,0x00},
	{0x77,0x70,0x70,0x70,0x00},
	{0x77,0x77,0x70,0x70,0x00},
	{0x77,0x77,0x77,0x70,0x00},
	{0x77,0x77,0x77,0x77,0x00},
	{0x77,0x77,0x77,0x77,0x07},
// l=5
	{0x70,0x70,0x70,0x70,0x70},
	{0x77,0x70,0x70,0x70,0x70},
	{0x77,0x77,0x70,0x70,0x70},
	{0x77,0x77,0x77,0x70,0x70},
	{0x77,0x77,0x77,0x77,0x70},
	{0x77,0x77,0x77,0x77,0x77}

};

/*
0  
1   X X
2  
3  X   x
4   xxx
5
6
*/

#define MAXSEG 80
char outb[18];

uint32_t a0=0;
uint32_t a1=0;
#ifdef DEBUG
	int da0=1;	// delta A0
	int da1=0;	// delta A1
#endif


void setup()
{
	Serial.begin(115200);
	vfd.init(CSPIN);
// initital segments
	vfd.setCustomCharacter(1,ccharset[35]);
	vfd.setCustomCharacter(2,ccharset[0]);

	vfd.setDimming(200);
	vfd.setGrayLevel(0,60);
	vfd.setGrayLevel(1,240);
}


void loop() {
	uint8_t i;
//	static uint32_t nextT=0;
	uint32_t l,r;
	uint8_t sl,sr,rl,rr;
	uint8_t cs,es,p1,p2;

	// read L and R values
#ifdef DEBUG
	if (a0 > 1023) {
		if (a1==0) {
			da0=0;
			da1=1;
		}
	}
	if (a1 > 1023) {
		if (a0 > 1023) {
			da0=-1;
			da1=0;
		}
	}
	if (a0 == 0) {
		if (a1>1023) {
			da0=0;
			da1=-1;
		}
		if (a1==0) {
			da0=1;
			da1=0;
		}
	}
	a0+=da0;
	a1+=da1;
#else
	a0=analogRead(A0);
	a1=analogRead(A1);
#endif

	l=(a0*MAXSEG);
	r=(a1*MAXSEG);
	l=l/1024;
	r=r/1024;
//	number of segments for L and R
	sl=l/5;
	sr=r/5;
	rl=l%5;
	rr=r%5;

//	calculate both segments
	if ((l<6) && (r<6)) {
		cs=0;
		es=0;
		p1=l*6;
		vfd.setCustomCharacter(2,ccharset[p1]);
		outb[0]=2;
		i=1;
	}
	else {
		if (l>r) {
			cs=sr;
			es=sl;
			p2=rl*6;
			p1=rr+30;
			vfd.setCustomCharacter(2,ccharset[p1]);
			vfd.setCustomCharacter(3,ccharset[30]);
			vfd.setCustomCharacter(4,ccharset[p2]);
		}
		else {
			cs=sl;
			es=sr;
			p1=(rl+1)*6-1;
			p2=rr;
			vfd.setCustomCharacter(2,ccharset[p1]);
			vfd.setCustomCharacter(3,ccharset[5]);
			vfd.setCustomCharacter(4,ccharset[p2]);
		}

//	fill the buffer
	//	common segments=chr(1)
	// 	when first end = chr(2)
	//  when it is only one channel = chr(3)
	//  when second end = chr(4)	
		for (i=0; i<cs; i++) {
			outb[i]=1;
		}
		outb[i++]=2;
		while (i<es) {
			outb[i++]=3;
		}
		outb[i++]=4;
	}
	while (i<sizeof(outb)-1) {
		outb[i++]=' ';
	}
	outb[i]=0;
	vfd.writeString(1,outb);

//	delay(10);

}

