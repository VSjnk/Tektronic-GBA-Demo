/*-------------------------Temlplate By:3DSage---------------------------------
                 https://www.youtube.com/c/3DSage
  ------------------Modifying it to work:VSink (Me!)---------------------------				 
	    https://www.youtube.com/channel/UCVUIhx_5Iuo8Fil8mYQNlxw
-----------------------------------------------------------------------------*/

//---#defines---
#include "gba.h"
//---Math functions---
#include <math.h> 
//---Load textures---
#include "SplashScreen.c"
//---Load Audio---
#include "sound.c"
//---Global variables---
#define  GBA_SW 160                                        //actual gba screen width
#define  SW     120                                        //game screen width
#define  SH      80                                        //game screen height
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))                //15 bit, 0-31, 5bit=r, 5bit=g, 5bit=b 
int lastFr=0,FPS=0;                                        //for frames per second


void clearBackground()
{int x,y;
 for(x=0;x<SW;x++)
 {                                                         //rgb values 0-31
  for(y=0;y<SH;y++){ VRAM[y*GBA_SW+x]=RGB(8,12,16);}       //clear all 120x80 pixels
 }
}

void drawImage(int w,int h, int xo,int yo, const u16* map, int to)
{int x,y,c;          //image w/h, position offset,  texture name, texture offset
 for(x=0;x<w;x++) 
 {                
  for(y=0;y<h;y++){ c=map[(y+to*h)*w+x]; if(c>0){ VRAM[(y+yo)*GBA_SW+x+xo]=c;}} 
 }
}
void init()
{
	//needed to compile for some reason.
}
//---Sound Stuff---   (Probably what youre after)
//Yes I used Ctrl-c Ctrl-v, this took to long to make and I don't know how it works :)
//also I still need to figure out ow to stop the song.

u16 _soundLength[] = {131};// Length of all sound in file sound.h. Add to the list with the length of the song in seconds
u16 _soundLengthInSec;//Length of sound
u16 _soundPlayedInSec; //Number of second played the sound


void PlaySound1() {
	//Turn on sound circuit - Master sound enable and enable Sound 1-4
	REG_SOUNDCNT_X = 0x80;

	//Sound 1-4 Output level and Stereo control
	//Full volume, enable sound 1 to both left and right
	REG_SOUNDCNT_L = 0x1177;

	// Overall: output ratio
	REG_SOUNDCNT_H = 2;

	// Configure for sound channel 1
	REG_SOUND1CNT_L = 0x0046; //Sound 1 Sweep control //Low
	REG_SOUND1CNT_H = 0xfAF7; //Sound 1 Length, wave duty and envelope control //High
	REG_SOUND1CNT_X = 0xCEAD; //Sound 1 Frequency, reset and loop control //Frequency
}

//****
//Modified from: http://belogic.com/gba/ - Direct sound
//****
//soundNum: 4 songs
void PlaySoundDMA(u8 soundNum) {
	_soundPlayedInSec = 1; // number of seconds has passed
	_soundLengthInSec = _soundLength[soundNum - 1]; // total length in seconds of this song

	//Play a mono sound at 16khz
	//Use timer 0 as sampling rate source
	
	REG_SOUNDCNT_H = 0x0B0F; //0x0b0F; //enable DS A&B + FIFO reset + use timer0 + max volume to L and R
	REG_SOUNDCNT_X = 0x0080; //turn sound chip on
	switch (soundNum) {
	case 1:
		REG_DMA1SAD = (u32) _JojiDemo; // src
		break;
	}

	//DMA1 destination
	REG_DMA1DAD = 0x040000a0; //write to FIFO A address 
	REG_DMA1CNT_H = 0xb600; //DMA control: DMA enabled+ start on FIFO+32bit+repeat+increment source&dest

	//REG_TM0D
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq) 
	REG_TM0CNT_L =  0xFBE6; //16khz playback frequency
	REG_TM0CNT_H = 0x0080; //enable timer
}

void StopDMASound() {
	REG_TM0CNT_H = 0; //disable timer 0
	REG_DMA1CNT_H = 0x00; //stop DMA
	dprint("Stop DMA Sound");
}

int main()
{
 //Init mode 5------------------------------------------------------------------
 *(u16*)0x4000000 = 0x405;                                           //mode 5 background 2
 *(u16*)0x400010A = 0x82;                                            //enable timer for fps
 *(u16*)0x400010E = 0x84;                                            //cnt timer overflow

 //scale small mode 5 screen to full screen-------------------------------------
 REG_BG2PA=256/2;                                                    //256=normal 128=scale 
 REG_BG2PD=256/2;                                                    //256=normal 128=scale 
  
 init();                                                             //init game variables

 while(1) 
 { 
  
	drawImage(120,80, 0,0, SplashScreen_Map, 0);   
	PlaySoundDMA(1);
	if(KEY_A){StopDMASound();}
	
   //frames per second---------------------------------------------------------- 
   //VRAM[15]=0; VRAM[FPS]=RGB(31,31,0);                               //draw fps 
   FPS+=1; if(lastFr>REG_TM2D>>12){ FPS=0;}                          //increase frame
   lastFr=REG_TM2D>>12;                                              //reset counter

   //swap buffers---------------------------------------------------------------
   while(*Scanline<160){}	                                         //wait all scanlines 
   if  ( DISPCNT&BACKB){ DISPCNT &= ~BACKB; VRAM=(u16*)VRAM_B;}      //back  buffer
   else{                 DISPCNT |=  BACKB; VRAM=(u16*)VRAM_F;}      //front buffer  
  
 }
}

