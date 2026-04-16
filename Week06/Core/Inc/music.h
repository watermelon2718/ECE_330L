/*
 * music.h
 *
 *  Created on: Feb 19, 2026
 *      Author: erandy.lopez
 */

#ifndef INC_MUSIC_H_
#define INC_MUSIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
  Song note event:
  - note: timer threshold (your TIM7 ISR compares note <= TONE)
  - size: note length divisor (tempo/size)
  - tempo: base duration number (you used 1400)
  - space: silence at end of note (in SysTick counts)
  - end: 1 means last note
*/
typedef struct
{
  int note;
  int size;
  int tempo;
  int space;
  int end;
} Music;

/* Duration divisors (you already use these names in main.c) */
#define whole     1
#define half      2
#define quarter   4
#define _8th      8
#define _16th     16

/* ------------------------
   Notes (these are *not* Hz)
   These are thresholds used by:
     if (Song[INDEX].note <= TONE) { toggle pin; TONE=0; }
   So smaller number -> higher pitch.
   ------------------------ */
#define rest      0

/* A simple scale set (tweak if your lab provided exact values) */
#define C4        382
#define Cs4_Db4   361
#define D4        340
#define Ds4_Eb4   321
#define E4        303
#define F4        286
#define Fs4_Gb4   270
#define G4        255
#define Gs4_Ab4   241
#define A4        227
#define As4_Bb4   214
#define B4        202

#define C5        191
#define Cs5_Db5   180
#define D5        170
#define Ds5_Eb5   160
#define E5        151
#define F5        143
#define Fs5_Gb5   135
#define G5        127
#define Gs5_Ab5   120
#define A5        113

/* --------- Shared globals used by ISRs (defined in main.c) --------- */
extern Music Song[100];

extern char ramp;
extern char RED_BRT;
extern char GREEN_BRT;
extern char BLUE_BRT;
extern char DIM_Enable;
extern char Music_ON;

extern int TONE;
extern int COUNT;
extern int INDEX;
extern int Save_Note;

extern int Vibrato_Depth;
extern int Vibrato_Rate;
extern int Vibrato_Count;

#ifdef __cplusplus
}
#endif

#endif /* INC_MUSIC_H_ */
