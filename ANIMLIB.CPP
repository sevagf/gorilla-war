#include <fstream.h>
#include <iostream.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <mem.h>
#include <stdio.h>
#include <alloc.h>
#include "gfx320.inc"

const SCREEN_SIZE = 64000;

struct Sprite
{
char *Pic;
char *Behind;
int OldX,OldY;
int X,Y;
int Height,Width;
};

void FlipPage(char *Source,char *Destination)
	{  memcpy(Destination,Source,SCREEN_SIZE);}
void LoadSprite(Sprite &Pic,char *Filename);
void GetSprite(Sprite &Pic,char *Page);
void PutSprite(Sprite Pic,char *&Page);
void FlipSprite(Sprite Pic,char *Page);
void EraseSprite(Sprite &Pic,char *Page);
void FlipPortion(Sprite &Pic,char *Page);

void LoadSprite(Sprite &Pic,char *Filename)
{
ifstream infile(Filename,ios::binary);
char C;
int Count=0;
while(infile >> C)
	{
	Pic.Pic[Count]=C;
	Pic.Behind[Count]=C;
	Count++;
	}
Count=0;
}

void GetSprite(Sprite &Pic,char *Page)
{
int X,Y;
int Count=0;
char Color;
for(Y=Pic.Y;Y<Pic.Y+Pic.Height;Y++)
	for(X=Pic.X;X<Pic.X+Pic.Width;X++)
		{
		Pic.Behind[Count]=Page[X+Y*320];
		Count++;
		}

}
void PutSprite(Sprite Pic,char *&Page)
{
int X,Y;
int Count=0;
char Color;
for(Y=Pic.Y;Y<Pic.Y+Pic.Height;Y++)
	for(X=Pic.X;X<Pic.X+Pic.Width;X++)
		{
		Color=Pic.Pic[Count];
		if(Color)
		    if(Y>0 && Y<200)
			if(X>0 && X<320)
				Page[X+Y*320]=Color;
		Count++;
		}
}

void FlipSprite(Sprite Pic,char *Page)
{
int X,Y;
char Color;
for(Y=Pic.Y;Y<Pic.Y+Pic.Height;Y++)
	for(X=Pic.X;X<Pic.X+Pic.Width;X++)
		{
		Color=Page[X+Y*320];
		if (Color)
			pokeb(VGA_RAM,X+Y*320,Color);
		}

for(Y=Pic.OldY;Y<Pic.OldY+Pic.Height;Y++)
	for(X=Pic.OldX;X<Pic.OldX+Pic.Width;X++)
		{
		Color=Page[X+Y*320];
		if (Color)
			pokeb(VGA_RAM,X+Y*320,Color);

		}


}
void EraseSprite(Sprite &Pic,char *Page)
{
int X,Y;
int Count=0;
char Color;
for(Y=Pic.Y;Y<Pic.Y+Pic.Height;Y++)
	for(X=Pic.X;X<Pic.X+Pic.Width;X++)
		{
		Page[X+Y*320]=Pic.Behind[Count];
		Count++;
		}
}

void FlipPortion(Sprite &Pic,char *Page)
{

int X,Y;
int Count=0;
char Color;
for(Y=Pic.OldY;Y<Pic.OldY+Pic.Height;Y++)
	for(X=Pic.OldX;X<Pic.OldX+Pic.Width;X++)
		{
		Color=Page[X+Y*320];
		if (Color)
			pokeb(VGA_RAM,X+Y*320,Color);

		}




}
