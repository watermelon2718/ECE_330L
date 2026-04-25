//header guard
#ifndef MUSIC_H_
#define MUSIC_H_

//structure for a note
typedef struct Note {
	int pitch; //freq
	int length; //quarter, half
	int tempo; // base speed 
	int space; // silence between notes
	char end; 
} Note;

//structure for a song
typedef struct Music {
    int doPlay; // init to true
    Note *notes; // pointer to notes array
    
    int tone; // tone counter
    int counter; // counter for note timing
    int index; // what note in the song we're on

    //note pitch
    int currentNote;
    int saveNote;

    //vibrato
    int vibratoDepth;
    int vibratoRate;
    int vibratoCounter;
    int vibratoDir; // init to 1
} Music;
void MusicInit(Music *m, int vibratoRate, int vibratoDepth);
void MusicSystick(Music *m);
void MusicTIM7Tick(Music *m);


/* music notes by name, 125Khz (250Khz/2) frequency will be divided by these constants for 1/2 period  *
 * note frequency = 125Khz / (NOTE * 2)                                                                */

#define C0 3823
#define Cs0_Db0 3608
#define D0 3405
#define Ds0_Eb0 3214
#define E0 3034
#define F0 2864
#define Fs0_Gb0 2703
#define G0 2551
#define Gs0_Ab0 2408
#define A0 2273
#define As0_Bb0 2145
#define B0 2025

#define C1 1911
#define Cs1_Db1 1803
#define D1 1703
#define Ds1_Eb1 1607
#define E1 1517
#define F1 1432
#define Fs1_Gb1 1351
#define G1 1275
#define Gs1_Ab1 1204
#define A1 1136
#define As1_Bb1 1073
#define B1 1012

#define C2 956
#define Cs2_Db2 902
#define D2 851
#define Ds2_Eb2 804
#define E2 758
#define F2 716
#define Fs2_Gb2 676
#define G2 638
#define Gs2_Ab2 602
#define A2 568
#define As2_Bb2 536
#define B2 506

#define C3 478
#define Cs3_Db3 451
#define D3 426
#define Ds3_Eb3 402
#define E3 379
#define F3 358
#define Fs3_Gb3 338
#define G3 319
#define Gs3_Ab3 301
#define A3 284
#define As3_Bb3 268
#define B3 253

#define C4 239
#define Cs4_Db4 225
#define D4 213
#define Ds4_Eb4 201
#define E4 190
#define F4 179
#define Fs4_Gb4 169
#define G4 159
#define Gs4_Ab4 150
#define A4 142
#define As4_Bb4 134
#define B4 127

#define C5 119
#define Cs5_Db5 113
#define D5 106
#define Ds5_Eb5 100
#define E5 95
#define F5 89
#define Fs5_Gb5 84
#define G5 80
#define Gs5_Ab5 75
#define A5 71
#define As5_Bb5 67
#define B5 63

#define C6 60
#define Cs6_Db6 56
#define D6 53
#define Ds6_Eb6 50
#define E6 47
#define F6 45
#define Fs6_Gb6 42
#define G6 40
#define Gs6_Ab6 38
#define A6 36
#define As6_Bb6 34
#define B6 32

/* note size constants to divide tempo value to get note duration */
#define whole 1
#define half 2
#define quarter 4
#define _8th 8
#define _16th 16
#define _32nd 32
#define rest 0










#endif

