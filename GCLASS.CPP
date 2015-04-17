#define Amp 60  // height of jump
#define Amp2 30 // Banana[0] throw


class Gorilla
{
	private:
		Sprite Player[6],Banana[2];
		apstring Name;

		int Period;
		int Direction;
		int X;
		int Y;
		int isJumping;
		int isMoving;
		int OldY;
		int OldX;
		/////Banana Stuff/////
		int Enough;
		int isThrowing;
		int BananaStartX;
		int BananaStartY;
		int BananaDir;

	public:

		int Points;
		int B;
		int P;
		int JumpTable[100];
		int BananaTable[200];
		int BananaX(){return Banana[0].X;}
		int BananaY(){return Banana[0].Y;}
		int TellThrowing(){return isThrowing;}
		int RetX(){return Player[0].X;}
		int RetY(){return Player[0].Y;}
		void Left();
		void Right();
		Gorilla(apstring Nam,char *gorl1,char *gorr1,char *gorl2,
		char *gorr2,char *gortl,char *gortr,int XX,int YY);
		~Gorilla();
		void Jump(int H);
		int Score() {return Points;}
		int Throwing(int H);
		int Dir() {return Direction;}
		void MakeTables();
		int Gravity();
		friend void Move(Gorilla &Player1,Gorilla &Player2,char *&Page,int P1,int P2,int B1,int B2);
		int ShowMe() {return Period;}
		friend int Collision(Gorilla &Player1,Gorilla &Player);
		friend int GorillaLose(Gorilla &Player1,Gorilla &Player2,char *&Page);
		friend void NewScore(Gorilla &Player1,Gorilla &Player2,char *&Page);


};

void Move(Gorilla &Player1,Gorilla &Player2,char *&Page,int P1,int P2,int B1,int B2)
{
int Go1=Player1.Throwing(2);
int Go2=Player2.Throwing(2);
GetSprite(Player1.Player[P1],Page);
if(Go1)
	GetSprite(Player1.Banana[0],Page);
GetSprite(Player2.Player[P2],Page);
if(Go2)
	GetSprite(Player2.Banana[0],Page);

PutSprite(Player1.Player[P1],Page);
if(Go1)
	PutSprite(Player1.Banana[0],Page);
PutSprite(Player2.Player[P2],Page);
if(Go2)
	PutSprite(Player2.Banana[0],Page);

FlipSprite(Player1.Player[P1],Page);
if(Go1)
	FlipSprite(Player1.Banana[0],Page);
FlipSprite(Player2.Player[P2],Page);
if(Go2)
	FlipSprite(Player2.Banana[0],Page);

EraseSprite(Player1.Player[P1],Page);
if(Go1)
	EraseSprite(Player1.Banana[0],Page);
EraseSprite(Player2.Player[P2],Page);
if(Go2)
	EraseSprite(Player2.Banana[0],Page);


if(!Go1 && Player1.Enough==0)
	{FlipPortion(Player1.Banana[0],Page);
	Player1.Enough=1;}

if(!Go2 && Player2.Enough==0)
	{FlipPortion(Player2.Banana[0],Page);
	Player2.Enough=1;}



}

void Gorilla::Gorilla(apstring Nam,char *gorl1,char *gorr1,char *gorl2,
		char *gorr2,char *gortl,char *gortr,int XX,int YY)
{
Name=Nam;
int Direction=0;
int Period=0;
for(int Z=0;Z<6;Z++)
{
Player[Z].X=Player[Z].OldX=X=OldX=XX;
Player[Z].Y=Player[Z].OldY=Y=OldY=YY;
Player[Z].Height=50;
Player[Z].Width=40;
Player[Z].Pic = (char *) malloc(2000);
Player[Z].Behind = (char *) malloc(2000);
}
LoadSprite(Player[0],gorl1);
LoadSprite(Player[1],gorr1);
LoadSprite(Player[2],gorl2);
LoadSprite(Player[3],gorr2);
LoadSprite(Player[4],gortl);
LoadSprite(Player[5],gortr);


Banana[0].X=Banana[0].OldX=1;
Banana[0].Y=Banana[0].OldY=1;
Banana[0].Height=20;
Banana[0].Width=20;

Banana[0].Pic = (char *) malloc(400);
Banana[0].Behind = (char *) malloc(400);
isJumping=0;
LoadSprite(Banana[0],"Banana.raw");
isThrowing=0;
if(Name=="P2")
	P=0;
else{P=1;}
MakeTables();
BananaStartX=0;
BananaStartY=0;
BananaDir=0;
isMoving=0;
Enough=0;
Points=100;
B=0;
}

void Gorilla::~Gorilla()
{
}

int Gorilla::Gravity()
{
int C=2;
// return true if you are on top of something
// return false if you need to fall some more
//gotoxy(6,1);
//if(Name=="P2")
//cout << Player[0].X <<" " <<Player[0].Y << " ";

//on the right shoulder // good
if(Player[0].Y<65 && Player[0].Y>60 && Player[0].X>140 && Player[0].X<182)
	return 1;
//on the left shoulder
if(Player[0].Y<65 && Player[0].Y>60 && Player[0].X>74 && Player[0].X<115)
	return 1;
//on top of the bananas
if(Player[0].Y<35 && Player[0].Y>30 && Player[0].X>7 && Player[0].X<60)
	return 1;
//on top of the ledge
else if(Player[0].Y<95 && Player[0].Y>90 && Player[0].X>195)
	return 1;
else if(Player[0].Y<150)
	{
	for(int Z=0;Z<6;Z++)
		{
		Player[Z].OldY=Player[Z].Y;
		Player[Z].Y+=C;
		}
	return 0;
	}
else{return 1;}
}

void Gorilla::Jump(int H)
{
if (H==1 && isJumping==0)
	{
	OldY=Player[0].Y;
	for(int Z=0;Z<6;Z++)
		{
		Player[Z].OldY=Player[Z].Y;
		Player[Z].Y=OldY-JumpTable[isJumping];
		}
	Y=OldY-JumpTable[isJumping];
	isJumping++;
	if(isJumping==25)
		isJumping=0;
	}

if(H==0 && isJumping!=0)
	{
	for(int Z=0;Z<6;Z++)
		{
		Player[Z].OldY=Player[Z].Y;
		Player[Z].Y=OldY-JumpTable[isJumping];
		}
	Y=OldY-JumpTable[isJumping];
	isJumping++;
	if(isJumping==25)
		isJumping=0;
	}
if(isJumping==0)
	Gravity();
}

void Gorilla::Left()
{
if(Player[0].X-1>2)
for(int Z=0;Z<6;Z++)
{
	Player[Z].OldX=Player[Z].X;
	Player[Z].X-=1;
	X-=1;
}
Direction=-1;
Period++;
if(Period>10)
	P=2;
else{P=0;}
if(Period>=20)
	Period=0;
if(Period<0)
	Period=0;
if(isThrowing<70 && isThrowing>0)
	P=4+(Direction==1);

}

void Gorilla::Right()
{
if(Player[0].X<282)
for(int Z=0;Z<6;Z++)
{
	Player[Z].OldX=Player[Z].X;
	Player[Z].X+=1;
	X+=1;
}
Direction=1;
Period++;
if(Period>10)
	P=1;
else{P=3;}
if(Period>=20)
	Period=0;
if(isThrowing<70 && isThrowing>0)
		P=4+(Direction==1);

}

void Gorilla::MakeTables()
{
for(int MakeX=0;MakeX<100;MakeX++)
	JumpTable[MakeX]=(-.128*(MakeX)*(MakeX-50));

for(MakeX=0;MakeX<200;MakeX+=3)
	BananaTable[MakeX]=(-.016*(MakeX)*(MakeX-125));
}

int Gorilla::Throwing(int H)
{
if(H==2 && Banana[0].Y<1)
	return 1;

if(H==3)
	{
	Banana[0].Y=250;
	isThrowing=0;
	}

if(H==2)
	return (isThrowing!=0);
if(H==1 && isThrowing==0)
	{
	Enough=0;
	P=4+(Direction==1);

	Banana[0].OldX=Banana[0].X=BananaStartX=Player[0].X;
	Banana[0].OldY=Banana[0].Y=BananaStartY=Player[0].Y;
	BananaDir=Direction;
	Banana[0].Y=BananaStartY-BananaTable[isThrowing];
	Banana[0].X+=BananaDir*3;
	isThrowing+=3;
	return 1;
	}

if(H==0 && isThrowing>0)
	{
	if(isThrowing<70)
		P=4+(Direction==1);

	Banana[0].OldY=Banana[0].Y;
	Banana[0].OldX=Banana[0].X;
	Banana[0].Y=BananaStartY-BananaTable[isThrowing];
	Banana[0].X+=BananaDir*3;
	isThrowing+=3;
	if((Banana[0].Y>180)
	 || (Banana[0].X+BananaDir*3>306)
	 || (Banana[0].X+BananaDir*3<-6))
		{
		isThrowing=0;
		Banana[0].Y=250;
		}

	return 1;
	}
}

int Collision(Gorilla &Player1,Gorilla &Player2)
{
int Y;
for(int X=Player1.RetX();X<Player1.RetX()+40;X++)
	for(Y=Player1.RetY();Y<Player1.RetY()+50;Y++)
		if(Player2.BananaX()==X)
			if(Player2.BananaY()==Y && Player2.TellThrowing()>90)
				{Player2.Throwing(3);
				Player2.Points-=5;
				return 1;
				}
return 0;
}

int GorillaLose(Gorilla &Player1,Gorilla &Player2,char *&Page)
{
if(Player1.Points==0)
	{
	KeysOff();
	EndGfx();
	StartGfx();
	PutSprite(Player2.Player[Player2.P],VGAMEM);
	PutSprite(Player1.Player[Player1.P],VGAMEM);
	gotoxy(1,1);
	for(int Q=1;Q<3;Q++)
	cout << "Player1! You are Dead!" << endl;
	delay(700);

	getch();
	return 1;

	}
if(Player2.Points==0)
	{
	KeysOff();
	EndGfx();
	StartGfx();
	PutSprite(Player1.Player[Player1.P],VGAMEM);
	PutSprite(Player2.Player[Player2.P],VGAMEM);
	gotoxy(1,1);
	for(int Q=1;Q<3;Q++)
	cout << "Player2! You are Dead!" << endl;
	delay(700);
	getch();
	return 1;

	}
return 0;

}

void NewScore(Gorilla &Player1,Gorilla &Player2,char *&Page)
{
for(int X=10;X<Player1.Points+10;X++)
	Line(X,7,X,13,1);
for(X=Player1.Points+10;X<110;X++)
	Line(X,7,X,13,4);

for(X=310;X>310-Player2.Points;X--)
	Line(X,7,X,13,1);
for(X=310-Player2.Points;X>210;X--)
	Line(X,7,X,13,4);


}