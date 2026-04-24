#include "Music.h"

void SongTick(Song *s) {
    s->counter++;  // Increment note duration counter
    s->vibratoCount++; // Increment the note vibrato effect counter

    /* This code applies vibrato to the current note that is playing  */
    if (s->vibratoCount >= s->vibratoRate) {
	    s->vibratoCount = 0; // reset vibrato counter
	    if (s->notes[s->index].pitch > 0){ // check if the note pitch is greater than 0
			s->notes[s->index].pitch += s->vibratoDepth; // if so, add vibrato

            if (s->notes[s->index].pitch > (s->saveNote + s->vibratoDepth)) {
                s->notes[s->index].note = s->saveNote - s->vibratoDepth;
            }
		}
}
}