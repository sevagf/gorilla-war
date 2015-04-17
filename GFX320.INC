/////////////////////////////////////////////////////////////////////////
// GFX320.INC
// TESTED 04-07-99
/////////////////////////////////////////////////////////////////////////

#include <fstream.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <mem.h>
#include "BOOL.H"


/**********************************************************************/
/***                    TYPE DEFINITIONS                            ***/
/**********************************************************************/

typedef unsigned short Byte;   // byte type definition                 /
typedef unsigned int Word;     // word type definition                 /

struct Coord
{
   int X;                      // x coordinate                         /
   int Y;                      // y coordinate                         /
}; // struct coord

typedef Coord huge StackType[20000];  // stack type                    /


/**********************************************************************/
/***                  CONSTANT DECLARATIONS                         ***/
/**********************************************************************/

const VGA_RAM = 0xA000;        // base segment address of vga memory   /
char *VGAMEM = (char*) MK_FP(0xA000,0); // pointer to vga memory       /

const MINX    = 0;             // minimum  X coordinate value          /
const MIDX    = 159;           // midpoint X coordinate value          /
const MAXX    = 319;           // maximum  X coordinate value          /
const MINY    = 0;             // minimum  Y coordinate value          /
const MIDY    = 99;            // midpoint Y coordinate value          /
const MAXY    = 199;           // maximum  Y coordinate value          /
const MAXC    = 256;           // maximum number of colors             /

const float AR    = 0.88;      // aspect ratio: make circle symmetric  /
const float PI    = 3.14159;   // approximation of PI                  /
const float TWOPI = 6.28;      // approximation of 2 * PI              /

const FLOODSIZE = 20000;       // maximum flood fill size              /

const bool CLOSED = true;      // constant to draw filled-in rectangle /
const bool OPEN   = false;     // constant to draw open rectangle      /


/**********************************************************************/
/***                     GLOBAL VARIABLES                           ***/
/**********************************************************************/

float SinTab[630];             // sin lookup table                     /
float CosTab[630];             // cos lookup table                     /
int TopOfStack;                // top of stack                         /
Byte OldColor;                 // last base color for floodfill        /
StackType Stack;               // artifical stack                      /



/**********************************************************************/
/*                       FUNCTION PROTOTYPES                          */
/**********************************************************************/

void SetMode(Byte Mode);
// sets the video mode to "mode"

void StartGfx();
// starts the mcga (320x200x256) graphics mode

void EndGfx();
// restores standard text mode (80x25x16)

void Stop();
// pauses the program until a key is pressed

void Pixel(int X, int Y, Byte Color);
// plots a pixel of specified color at (x,y)

Byte GetPixel(int X, int Y);
// returns the color of the pixel at (x,y)

void Line(int X1, int Y1, int X2, int Y2, Byte Color);
// draws a line from (x1,y1) to (x2,y2)

void Rectangle(int X1, int Y1, int X2, int Y2,
	       Byte Color, bool Filled);
// draws a rectangle with corners (x1,y1), (x1,y2), (x2,y1), (x2,y2);
// it has a parameter "filled" that will draw a solid rectangle,
// rather than an outline

void MakeTables();
// creates sin and cos lookup tables

void Circle(int CX, int CY, int Radius, Byte Color);
// draws a circle with center (cx,cy) and radius "radius"

void Ellipse(int CX, int CY, int XR, int YR, Byte Color);
// draws an ellipse with center (cx,cy) and eccentricity of xr, for x,
// and yr, for y

void Arc(int CX, int CY, int XR, int YR,
	 float Start, float Finish, Byte Color);
// draws a portion of an ellipse with center (cx,cy) and eccentricity
// xr, for x, and yr, for y

void FloodFill(int X, int Y, Byte NewColor);
// fills an enclosed area starting at (x,y) with the specified color;
// every pixel inside the enclosed area with the same color as (x,y)
// will be altered

void ClearGfx(Byte Color);
// clears the graphics screen to the specified color

// begin stack-related functions
void Push(Coord N);
void Pop(Coord &N);
// end stack-related functions

int Round(float X);
// rounds decimal number (x) to an integer

void SaveImage(char *FName, int Left, int Top, int Right, int Bottom);
// save image in area bounded by (left,top) and (right,bottom)

void LoadImage(char *FileName, int Left, int Top);
// load image from file to (left,top)


/**********************************************************************/
/*                    FUNCTION IMPLEMENTATIONS                        */
/**********************************************************************/

void SetMode(Byte Mode)
{
   _AH = 0x00;               // select "set video mode" function (00h)
   _AL = Mode;               // set mode selected
   geninterrupt(0x10);       // call video interrupt (10h)
}  // void function setmode


void StartGfx()
{
   SetMode(0x13);            // set MCGA 320x200x256 mode (13h)
   MakeTables();             // make sin/cos lookup tables
}  // void function startgfx


void EndGfx()
{
   SetMode(0x03);            // set standard text mode (03h)
}  // void function endgfx


void Stop()
{
   getch();
}  // void function stop


void Pixel(int X, int Y, Byte Color)
{
   pokeb(VGA_RAM,X+Y*320,Color);
}  // void function pixel


Byte GetPixel(int X, int Y)
{
   return peekb(VGA_RAM,X+Y*320);
}  // byte function getpixel


void Line(int X1, int Y1, int X2, int Y2, Byte Color)
{
   int XDist,YDist;          // x and y distances                      /
   int XDir,YDir;            // x and y directions                     /
   int XCount,YCount;        // x and y counters                       /
   int XYInc;                // x and y incrementor                    /

   int Sign(int Number);     // returns sign of number                 /

   if (X1 == X2)
   {
      int X = X1;
      if (Y1 > Y2)
      {
	 int Temp = Y1; Y1 = Y2; Y2 = Temp;
      }  // if (y1 > y2)
      for (int Y = Y1; Y <= Y2; Y++)
	 Pixel(X,Y,Color);
   }  // if (x1 == x2)
   else
   {
      if (Y1 == Y2)
      {
	 int Y = Y1;
	 if (X1 > X2)
	 {
	    int Temp = X1; X1 = X2; X2 = Temp;
	 }  // if (x1 > x2)
	 for (int X = X1; X <= X2; X++)
	    Pixel(X,Y,Color);
      }  // if (y1 == y2)
   }  // else

   if (!((X1 == X2) || (Y1 == Y2)))
   {
      XDist = X2 - X1;
      YDist = Y2 - Y1;
      XDir = Sign(XDist);
      YDir = Sign(YDist);
      XDist = abs(XDist);
      YDist = abs(YDist);
      XCount = X1;
      YCount = Y1;
      XYInc = 0;
      while ((XCount != X2) || (YCount != Y2))
      {
	 Pixel(XCount,YCount,Color);
	 if (YDist < XDist)
	 {
	    XYInc += YDist;
	    XCount += XDir;
	    if (XYInc >= XDist)
	    {
	       XYInc -= XDist;
	       YCount += YDir;
	    }  // if (xyinc >= xdist)
	 }  // if (ydist < xdist)
	 else
	 {
	    XYInc += XDist;
	    YCount += YDir;
	    if (XYInc >= YDist)
	    {
	       XYInc -= YDist;
	       XCount += XDir;
	    }  // if (xyinc >= ydist)
	 }  // else
      }  // while ((xcount != x2) || (ycount != y2))
      Pixel(XCount,YCount,Color);
   }  // if (!((X1 == X2) || (Y1 == Y2)))
}  // void function line


   int Sign(int Number)
   {
      if (Number < 0) return -1; else return +1;
   }  // int sign (used with line and boundedline)

   void DoNothing()
   {
   }  // void function donothing


void Rectangle(int X1, int Y1, int X2, int Y2,
	       Byte Color, bool Filled)
{
   void HLine(int X1, int X2, int Y1, Byte Color);
   void VLine(int Y1, int Y2, int X1, Byte Color);

   if (Filled)
      for (int Y=Y1; Y <= Y2; Y++)
	 HLine(X1,X2,Y,Color);
   else
   {
      HLine(X1,X2,Y1,Color);
      HLine(X1,X2,Y2,Color);
      VLine(Y1,Y2,X1,Color);
      VLine(Y1,Y2,X2,Color);
   }  // if not filled
}  // void function rectangle

   void HLine(int X1, int X2, int Y1, Byte Color)
   {
      for (int X = X1; X <= X2; X++)
	 Pixel(X,Y1,Color);
   }  // void function rectangle::hline

   void VLine(int Y1, int Y2, int X1, Byte Color)
   {
      for (int Y=Y1; Y <= Y2; Y++)
	 Pixel(X1,Y,Color);
   }  // void function rectangle::vline




void MakeTables()
{
   float Radian;             // radian measure

   for (int X=0; X <= 629; X++)
   {
      Radian = float(X) / 100;
      SinTab[X] = sin(Radian);
      CosTab[X] = cos(Radian);
   }  // for x = 0 to 629
}  // void function maketables


void Circle(int CX, int CY, int Radius, Byte Color)
{
   int   X,Y;                // x and y coordinates

   for (int K=1; K <= 629; K++)
   {
      X = Round(Radius * CosTab[K]) + CX;
      Y = Round(Radius * SinTab[K] * AR) + CY;
      pokeb(VGA_RAM,X+Y*320,Color);
   }  // for k = 1 to 629
}  // void function circle


void Ellipse(int CX, int CY, int XR, int YR, Byte Color)
{
   int X,Y;                 // X and Y coordinate

   for (int K=1; K <= 629; K++)
   {
      X = Round(XR * CosTab[K]) + CX;
      Y = Round(YR * SinTab[K] * AR) + CY;
      pokeb(VGA_RAM,X+Y*320,Color);
   }  // for k
}  // void function ellipse


void Arc(int CX, int CY, int XR, int YR,
		float Start, float Finish, Byte Color)
{
   int X,Y;                 // x and y coordinate
   int RealK;               // array index; check for k wrap-around

   int Start2  = Start  * 100;
   int Finish2 = Finish * 100;
   int Direction = (Finish > Start) ? 1 : -1;
   for (int K=Start2; K <= Finish2; K += Direction)
   {
      RealK = (K > 629) ? K - 629 : K;
      X = Round(XR * CosTab[RealK]) + CX;
      Y = Round(YR * SinTab[RealK] * AR) + CY;
      pokeb(VGA_RAM,X+Y*320,Color);
   }  // for k = start2 to finish2
}  // void function arc


void FloodFill(int X, int Y, Byte NewColor)
{
   int XTR,YTR;              // temporary x and y values

   void QuadFlood(int X, int Y, Byte NewColor);

   OldColor = GetPixel(X,Y);

   // draw horizonal line to left
   XTR = X; YTR = Y;
   do
   {
      pokeb(VGA_RAM,XTR+YTR*320,NewColor);
      XTR--;
   }  // do
   while ((GetPixel(XTR,YTR) == OldColor) && (XTR > 0));

   // draw horizontal line to right
   XTR = X; YTR = Y;
   do
   {
      pokeb(VGA_RAM,XTR+YTR*320,NewColor);
      XTR++;
   }  // do
   while ((GetPixel(XTR,YTR) == OldColor) && (XTR < MAXX));

   // draw vertical line to bottom
   XTR = X; YTR = Y;
   do
   {
      pokeb(VGA_RAM,XTR+YTR*320,NewColor);
      YTR++;
   }  // do
   while ((GetPixel(XTR,YTR) == OldColor) && (YTR < MAXY));

   // draw vertical line to top
   XTR = X; YTR = Y;
   do
   {
      pokeb(VGA_RAM,XTR+YTR*320,NewColor);
      YTR--;
   }  // do
   while ((GetPixel(XTR,YTR) == OldColor) && (YTR > 0));

   QuadFlood(X-1,Y-1,NewColor);
   QuadFlood(X+1,Y-1,NewColor);
   QuadFlood(X+1,Y+1,NewColor);
   QuadFlood(X-1,Y+1,NewColor);
}  // void function flood

   void QuadFlood(int X, int Y, Byte NewColor)
   {
      Coord Temp;            // temporary coordinate

      bool RangeOk(Coord Temp);
      void MoveToNext(Coord &N);

      TopOfStack = 1;
      Temp.X = X;
      Temp.Y = Y;
      do
      {
	 if ((GetPixel(Temp.X,Temp.Y) == OldColor) && RangeOk(Temp))
	 {
	    pokeb(VGA_RAM,Temp.X + Temp.Y * 320, NewColor);
	    Push(Temp);
	    MoveToNext(Temp);
	 }  // if getpixel(temp.x,temp.y)
	 else
	 {
	    Pop(Temp);
	    MoveToNext(Temp);
	 }  // else
      }  // do
      while (TopOfStack > 0);
   }  // void function flood::quadflood

      bool RangeOk(Coord N)
      {
	 return bool((N.X >= 0) && (N.X <= MAXX) &&
			(N.Y >= 0) && (N.Y <= MAXY));
      }  // boolean flood::quadflood::rangeok

      void MoveToNext(Coord &N)
      {
	 if ((GetPixel(N.X+1,N.Y) == OldColor))
	    N.X++;
	 else if ((GetPixel(N.X,N.Y-1) == OldColor))
	    N.Y--;
	 else if ((GetPixel(N.X-1,N.Y) == OldColor))
	    N.X--;
	 else if ((GetPixel(N.X,N.Y+1) == OldColor))
	    N.Y++;
      }  // void function flood::quadflood::movetonext


void Push(Coord N)
{
   if (TopOfStack >= FLOODSIZE)
      TopOfStack -= FLOODSIZE;
   TopOfStack++;
   Stack[TopOfStack] = N;
}  // void function push

void Pop(Coord &N)
{
   if (TopOfStack > 0)
   {
      N = Stack[TopOfStack];
      TopOfStack--;
   }  // if topofstack
}  // void function pop



void ClearGfx(Byte Color)
{
   Rectangle(0,0,MAXX,MAXY,Color,CLOSED);
}  // void function cleargfx



int Round(float X)
{
   return (floor(X + 0.5));
}  // function round


void SaveImage(char *FName, int Left, int Top, int Right, int Bottom)
{
   ofstream OutFile(FName,ios::binary);
   // set output file, select binary (unformatted) type
   unsigned int Width  = Right - Left + 1;
   unsigned int Height = Bottom - Top + 1;
   unsigned char Buffer[320];
   unsigned char Temp[2];

   Temp[1] = Width >> 8;          // temp[0] = first byte of width
   Temp[0] = Width;               // temp[1] = second byte of width
   OutFile.write(Temp,2);         // write width to file
   Temp[1] = Height >> 8;         // temp[0] = first byte of height
   Temp[0] = Height;              // temp[1] = second byte of height
   OutFile.write(Temp,2);         // write height to file
   for (int Row = Top; Row <= Bottom; Row++)
   {
      memcpy(Buffer,VGAMEM + Left + Row * 320,Width);
      OutFile.write(Buffer,Width);
   }  // for row
   OutFile.close();
}  // void function saveimage


void LoadImage(char *FileName, int Left, int Top)
{
   ifstream InFile(FileName,ios::binary);
   // set input file, select binary (unformatted) type
   unsigned int Width;            // width
   unsigned int Height;           // height
   unsigned char Buffer[320];     // buffer
   unsigned char Temp[2];         // temporary buffer

   InFile.read(Temp,2);
   Width = (Temp[1] << 8) + Temp[0];
   InFile.read(Temp,2);
   Height = (Temp[1] << 8) + Temp[0];
   for (int Row = Top; Row <= Top + Height - 1; Row++)
   {
      InFile.read(Buffer,Width);
      memcpy(VGAMEM + Row * 320 + Left,Buffer,Width);
   }  //  for int
   InFile.close();
}  // void function loadimage

