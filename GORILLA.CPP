#include "apstring.h"
#include "animlib.cpp"
#include "keylib.cpp"
#include "gclass.cpp"
#include "intro.cpp"

void main()
{


StartGfx();

Gorilla Player1("P1","gor1l1.raw","gor1r1.raw","gor1l2.raw","gor1r2.raw","gor1tl.raw","gor1tr.raw",50,100);
Gorilla Player2("P2","gor2l1.raw","gor2r1.raw","gor2l2.raw","gor2r2.raw","gor2tl.raw","gor2tr.raw",200,100);

char *Page;
Page = (char *) malloc(64000);

Sprite War;
War.X=1;
War.Y=1;
War.Width=320;
War.Height=40;
War.Pic = (char *) malloc(64000);
War.Behind = (char *) malloc(64000);
LoadSprite(War,"gorwar.raw");
PutSprite(War,VGAMEM);

Sprite P1;
P1.X=5;
P1.Y=140;
P1.Width=40;
P1.Height=50;
P1.Pic = (char *) malloc(2000);
P1.Behind = (char *) malloc(2000);
LoadSprite(P1,"gor1r1.raw");
PutSprite(P1,VGAMEM);
Sprite P2;
P2.X=280;
P2.Y=140;
P2.Width=40;
P2.Height=50;
P2.Pic = (char *) malloc(2000);
P2.Behind = (char *) malloc(2000);
LoadSprite(P2,"gor2l1.raw");
PutSprite(P2,VGAMEM);
gotoxy(13,21);
cout << "Programmed by:";
gotoxy(17,22);
cout << "Supa-S, YuBo";
gotoxy(17,23);
cout << "Stix, LiuDaChris";

Intro();
delay(1000);
getch();
free(P1.Pic);
free(P2.Pic);
free(P1.Behind);
free(P2.Behind);

EndGfx();
clrscr();
cout << endl << endl << endl;
cout << "     Player2 Controls:                             Player1 Controls:  " << endl << endl;
cout << "                    [R]                                 [^]           " << endl;
cout << "                [D] [ ]  [G]                       [<]  [ ] [>]       " << endl;
cout << "           [Ctrl]  to fire                       [Enter] to fire    " << endl;
cout << "           [Shift] to Jump                       [Shift] to Jump    " << endl << endl;
cout << "                       Press Any Key to Continue" << endl;



getch();
StartGfx();
Z:;

/////////Set BackGround///////////
Sprite Background;
Background.X=0;
Background.Y=0;
Background.Height=200;
Background.Width=320;
Background.Pic = (char *) malloc(64000);
Background.Behind = (char *) malloc(64000);
LoadSprite(Background,"back.raw");
PutSprite(Background,Page);

FlipPage(Page,VGAMEM);

KeysOn();
int P1DidJump=0;
int P1Jump=0;
int P1Throw=0;
do
{

if(key_table[75]==1)
	Player2.Left();
if(key_table[77]==1)
	Player2.Right();

if(key_table[32]==1)
	Player1.Left();
if(key_table[34]==1)
	Player1.Right();

if(key_table[42]==1 && Player1.Gravity())
	Player1.Jump(1);
Player1.Jump(0);

if(key_table[54]==1 && Player2.Gravity())
	Player2.Jump(1);
Player2.Jump(0);

if(key_table[28]==1)
	Player2.Throwing(1);

Player2.Throwing(0);

if(key_table[29]==1)
	Player1.Throwing(1);

Player1.Throwing(0);

if(Collision(Player1,Player2));
	NewScore(Player2,Player1,Page);

if(Collision(Player2,Player1));
	NewScore(Player2,Player1,Page);

delay(5);
Move(Player1,Player2,Page,Player1.P,Player2.P,Player1.B,Player2.B);
}while (key_table[1]!=1 && !GorillaLose(Player1,Player2,Page));
if(key_table[1]==1)
	{
	EndGfx();
	StartGfx();
	}

cout << "Game Over!" << endl;
delay(2000);
EndGfx();
delete key_table;



}
