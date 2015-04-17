#include <iostream.h>
#include <conio.h>
#include <iomanip.h>
#include <conio.h>
#include <dos.h>

#ifdef __cplusplus
#define __CPPARGS ...
#else
#define __CPPARGS
#endif

unsigned char key_table[83];

void interrupt Keyboard_ISR(__CPPARGS);    /* NEW interrupt prototype */
void interrupt (*OldInterrupt)(__CPPARGS); /* Old interrupt function pointer */
void KeysOn() {OldInterrupt  = _dos_getvect(0x09);
  _dos_setvect(0x09,Keyboard_ISR);}
void KeysOff() { _dos_setvect(0x09,OldInterrupt);}

void interrupt Keyboard_ISR(...)
{
 asm{ sti};
 unsigned char key;
 union REGS regs;
 regs.h.al=inp(0x60);
 regs.h.ah=0;
 key = regs.x.ax;
 regs.h.al=inp(0x61);
 regs.h.al |=0x82;
 outp(0x61,regs.h.al);
 regs.h.al &= 0x7f;
 outp(0x61,regs.h.al);
 outp(0x20,0x20);
 asm{cli};

 if(key < 84)
   {key_table[key]=1;
   }
 else if(key < 212)
   {key_table[key-128]=0;
   }
}

void main()
{
KeysOn();
int X=0;
int OldX=0;
do
{
for(X=0;X<83;X++)
	{
	if(key_table[X]==1 && X!=OldX)
		{
		OldX=X;
		cout << X << " ";
		}
	}
}while(key_table[1]!=1);
KeysOff();
}