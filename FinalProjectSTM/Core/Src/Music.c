#include "Music.h"
#include "stm32f4xx_hal.h"

void MusicInit(Music *m, const Note *n, int vibratoRate, int vibratoDepth) {
    m->effectIsOn = 0; // no effect on by default
    m->doPlay = 1;       //music playing by default
 	// Set play state and starting position
    m->notes = n; // notes array
    m->index = 0;        // Start at the first note
    m->returnNotes = n;
    m->returnIndex = 0; // return values to jump back to song after effect is played

	m->tone = 0;         // Reset frequency counter
    m->counter = 0;      // Reset duration counter
    m->vibratoCounter = 0; // set vibrato counter to 0
	// Initialize Pitch and Vibrato
    // We load the first note's pitch into our working variables
    m->saveNote = m->notes[0].pitch;
    m->currentNote = m->saveNote;
	// Configure Vibrato Settings
    m->vibratoRate = vibratoRate;  // Adjust this for speed (lower = faster)
    m->vibratoDepth = vibratoDepth;  // Adjust this for intensity (how many frequency units it shifts)
    m->vibratoDir = 1;
}

//helper function which sets notes array
void SetNotesArray(Music *m, const Note *n) {
    m->notes = n; // set the notes array
}

void ResetPlayer(Music *m){
    m->tone = 0;         // Reset frequency counter
    m->counter = 0;      // Reset duration counter
    m->vibratoCounter = 0;
    m->saveNote = m->notes[m->index].pitch;
    m->currentNote = m->saveNote;
}

//switches the track
void PlayTrack (Music *m, const Note *t) {
    m->effectIsOn = 0; // no effect is on
    //set new track array
    SetNotesArray(m, t);
    m->index = 0; // start at the start of the track
    //wipe old status data
    m->returnNotes = m->notes; // return to this track
    m->returnIndex = 0; // return to the start of this track
    ResetPlayer(m);
}

//puts on a temporary effect
void PlayEffect(Music *m, const Note *effect) {
    if(m->effectIsOn == 0) { // only change the return address if we're going from a track to an effect, not from effect to effect
        m->effectIsOn = 1; // we are playing an effect
        //push addresses for return to the track we were playing
        m->returnIndex = m->index;
        m->returnNotes = m->notes;
    }
    SetNotesArray(m, effect); // set effect notes array
    m->index = 0; // start effect from the top

    ResetPlayer(m); //kick player
}

void MusicSystick(Music *m) {
    if (m->doPlay == 0 && m->effectIsOn == 0) return;

    m->counter++; 
    m->vibratoCounter++; 

    if (m->vibratoCounter >= m->vibratoRate) {
        m->vibratoCounter = 0; 
        
        if (m->saveNote > 0) { // Don't apply vibrato to rests
            // Move pitch in the current direction
            m->currentNote += (m->vibratoDepth * m->vibratoDir);

            // Check boundaries and flip direction if reached
            if (m->currentNote >= (m->saveNote + m->vibratoDepth)) {
                m->vibratoDir = -1; // Start going down
            } 
            else if (m->currentNote <= (m->saveNote - m->vibratoDepth)) {
                m->vibratoDir = 1;  // Start going up
            }
        }
    }
}

void MusicTIM7Tick(Music *m) {

    m->tone++; // increment tone generator counter
	
	/* This code plays the song from the song array structure */
	if ((m->doPlay > 0 ||m->effectIsOn > 0) && (m->currentNote > 0) && (((m->notes[m->index].tempo)/(m->notes[m->index].length) - m->notes[m->index].space) > m->counter)) {
		if (m->tone >= m->currentNote) { // Toggle when tone counter hits the target){
			GPIOD->ODR ^= 1;
			m->tone = 0;
		}
	} else if ((m->doPlay > 0 ||m->effectIsOn > 0) && (m->notes[m->index].tempo)/(m->notes[m->index].length) > m->counter) {
		m->tone = 0; // silence if playing time for note is over
	} else if ((m->doPlay > 0 ||m->effectIsOn > 0) && (m->notes[m->index].tempo)/(m->notes[m->index].length) == m->counter) {
		m->counter = 0; // zero out for a new note
		m->tone = 0;
		if (!(m->notes[m->index].end)) { //set to new note if present
			m->index++;
			m->saveNote = m->notes[m->index].pitch;
			m->currentNote = m->notes[m->index].pitch;
		}
	} else if (m->doPlay == 0 &&m->effectIsOn == 0) { 
			m->tone = 0;
			m->counter = 0;
	}

    //loop if on last note
    if(m->notes[m->index].end) {
        //returns to the return song array, loops for songs, jumps back from effects
        MusicReturn(m);
    }
}

void MusicReturn(Music *m) {
    m->effectIsOn = 0;
    m->notes = m->returnNotes;
    m->index = m->returnIndex;

    ResetPlayer(m); // play from where we left off
}