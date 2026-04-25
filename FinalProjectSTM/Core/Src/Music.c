#include "Music.h"
#include "stm32f4xx_hal.h"

void MusicInit(Music *m, const Note *n, int vibratoRate, int vibratoDepth) {
 	// Set play state and starting position
    m->doPlay = 1;       // Start playing immediately
    m->notes = n;

	m->tone = 0;         // Reset frequency counter
	m->index = 0;        // Start at the first note
    m->counter = 0;      // Reset duration counter
    
	// Initialize Pitch and Vibrato
    // We load the first note's pitch into our working variables
    m->saveNote = m->notes[0].pitch;
    m->currentNote = m->saveNote;

	// Configure Vibrato Settings
	m->vibratoCounter = 0;
    m->vibratoRate = vibratoRate;  // Adjust this for speed (lower = faster)
    m->vibratoDepth = vibratoDepth;  // Adjust this for intensity (how many frequency units it shifts)
    m->vibratoDir = 1;
}
//switches the track
void PlayTrack (Music *m, const Note *t) {
    MusicInit(m, t, m->vibratoRate, m->vibratoDepth);
    m->returnIndex = m->index;
    m->returnNotes = m->notes;
}
//puts on a temporary effect
void PlayEffect(Music *m, const Note *effect) {
    if(m->returnIndex != m->index) {m->index = m->returnIndex;} //return to track if effect is already playing
    if(m->returnNotes != m->notes) {m->notes = m->returnNotes;} //return to track if effect is already playing

    m->returnIndex = m->index; // leave return address
    m->returnNotes = m->notes;
    MusicInit(m, effect, m->vibratoRate, m->vibratoDepth);
}

void MusicSystick(Music *m) {
    if (m->doPlay == 0) return;

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
	if ((m->doPlay > 0) && (m->currentNote > 0) && (((m->notes[m->index].tempo)/(m->notes[m->index].length) - m->notes[m->index].space) > m->counter)) {
		if (m->tone >= m->currentNote) { // Toggle when tone counter hits the target){
			GPIOD->ODR ^= 1;
			m->tone = 0;
		}
	} else if ((m->doPlay > 0) && (m->notes[m->index].tempo)/(m->notes[m->index].length) > m->counter) {
		m->tone = 0; // silence if playing time for note is over
	} else if ((m->doPlay > 0) && (m->notes[m->index].tempo)/(m->notes[m->index].length) == m->counter) {
		m->counter = 0; // zero out for a new note
		m->tone = 0;
		if (!(m->notes[m->index].end)) { //set to new note if present
			m->index++;
			m->saveNote = m->notes[m->index].pitch;
			m->currentNote = m->notes[m->index].pitch;
		}
	} else if (m->doPlay == 0) { 
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
    MusicInit(m, m->notes, m->vibratoRate, m->vibratoDepth);
    m->notes = m->returnNotes;
    m->index = m->returnIndex;
    m->returnIndex = 0;
}