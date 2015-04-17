
#define pi 3.14159
#define A 5 // this is the amplitude of the sine waves. between 1-5

/*
void wtsync()
{
asm     mov     dx,3DAh
WaitVR1:
asm     in      al,dx
asm     test    al,8
asm     jne     WaitVR1
WaitVR2:
asm     in      al,dx
asm     test    al,8
asm     je      WaitVR2
}
*/

void Intro()
{

StartGfx();
MakeRainbow(GlobalPalette);
SetPalette(GlobalPalette);

int MX=0;
int MY=0;

int Y=0;
int X;
int Y1;
int C=1;
int sinlist[1000];

for(X=0;X<500;X++) //create a sin list to increase speed
	sinlist[X]=(A*sin(((X+C)*(pi/180))*3)+50);

int T=1;

int Color;
int K=50;
int X1=0;
int C1=0;


RGBSignature Col;
Col.Red=0;
Col.Grn=0;
Col.Blu=0;


for(T=256;T<320;T++)
	{
	SetRGB(T,Col);
	GlobalPalette[T]=Col;
	Col.Red+=2;
	Col.Grn+=2;
	}

do
{
for(X=51;X<250;X++) //draw entire flag for loop
	{
	Y=sinlist[X+C];
	T=sinlist[Y*6]-50;
	for(Y1=Y;Y1<Y+100;Y1++)
		{

		X1=sinlist[Y1+C]+X;
		Color=72-T;//;T;//red = 72
		if(Y1>Y+30)
			Color=122+T;//GREEN = 122
		if(Y1>Y+66)
			Color=280+T;//YELLOW
		if(Y1==Y)Color=0;
		if(Y1==Y+99)Color=0;
		if(X==51)Color=0;
		if(X==249)Color=0;


		Pixel(X1-40,Y1,Color);
		}
	}
   C1=C;
   C++;
   if (C==119)
	C=1;
}while(!kbhit());

getch();
PaletteType Pal;//creating a temporary fade palette
for(X=0;X<256;X++)
	Pal[X]=GlobalPalette[X];

do
{
	CyclePalette(Pal);
	SetPalette(Pal);
	delay(10);
}while(!kbhit());

for(X=0;X<64;X++)//fading the temporary fade palette to white
	{
	for(T=0;T<256;T++)
		{
		if (Pal[T].Red<X)
			Pal[T].Red++;
		if (Pal[T].Grn<X)
			Pal[T].Grn++;
		if (Pal[T].Blu<X)
			Pal[T].Blu++;
		}
	SetPalette(Pal);
//	wtsync();
	delay(10);
	}
for(X=0;X<64;X++)//fading the temporary fade palette to black
	{
	for(T=0;T<256;T++)
		{
		if (Pal[T].Red>0)
			Pal[T].Red--;
		if (Pal[T].Grn>0)
			Pal[T].Grn--;
		if (Pal[T].Blu>0)
			Pal[T].Blu--;
		}
	SetPalette(Pal);
//	wtsync();
	delay(10);
	}
//DestroyVirtualPage(Page);
//EndGfx();
delete sinlist;
}

