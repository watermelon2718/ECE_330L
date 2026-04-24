#include "Music.h"

void SongSystick(Song *s) {
    if (s->doPlay == 0) return;

    s->counter++; 
    s->vibratoCount++; 

    if (s->vibratoCount >= s->vibratoRate) {
        s->vibratoCount = 0; 
        
        if (s->saveNote > 0) { // Don't apply vibrato to rests
            // Move pitch in the current direction
            s->currentNote += (s->vibratoDepth * s->vibratoDir);

            // Check boundaries and flip direction if reached
            if (s->currentNote >= (s->saveNote + s->vibratoDepth)) {
                s->vibratoDir = -1; // Start going down
            } 
            else if (s->currentNote <= (s->saveNote - s->vibratoDepth)) {
                s->vibratoDir = 1;  // Start going up
            }
        }
    }
}

void SongTIM7Tick(Song *s) {

    s->tone++; // increment tone generator counter
	
	/* This code plays the song from the song array structure */
	if ((s->doPlay > 0) && (s->currentNote > 0) && (((s->notes[s->index].tempo)/(s->notes[s->index].length) - s->notes[s->index].space) > s->counter)) {
		if (s->tone >= s->currentNote) { // Toggle when tone counter hits the target){
			GPIOD->ODR ^= 1;
			s->tone = 0;
		}
	} else if ((s->doPlay > 0) && (s->notes[s->index].tempo)/(s->notes[s->index].length) > s->counter) {
		s->tone = 0; // silence if playing time for note is over
	} else if ((s->doPlay > 0) && (s->notes[s->index].tempo)/(s->notes[s->index].length) == s->counter) {
		s->counter = 0; // zero out for a new note
		s->tone = 0;
		if (!(s->notes[s->index].end)) { //set to new note if present
			s->index++;
			s->saveNote = s->notes[s->index].pitch;
			s->currentNote = s->notes[s->index].pitch;
		}
	} else if (s->doPlay == 0) { 
			s->tone = 0;
			s->counter = 0;
	}
}