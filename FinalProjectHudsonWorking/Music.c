#include "Music.h"

void SongSystick(Song *s) {
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

void SongTIM7Tick(Song *s) {

    s->TONE++;
	ramp++;

	/* This code plays the song from the song array structure */
	if ((Music_ON > 0) && (Song[INDEX].note > 0) && ((Song[INDEX].tempo/Song[INDEX].size - Song[INDEX].space) > COUNT))
	{

		if (Song[INDEX].note <= TONE)
		{
			GPIOD->ODR ^= 1;
			TONE = 0;
		}
	}
	else if ((Music_ON > 0) && Song[INDEX].tempo/Song[INDEX].size > COUNT)
	{
		TONE = 0;
	}
	else if ((Music_ON > 0) && Song[INDEX].tempo/Song[INDEX].size == COUNT)
	{
		COUNT = 0;
		TONE = 0;
		if (!(Song[INDEX].end))
				{
					INDEX++;
					Save_Note = Song[INDEX].note;
				}
	}
	else if (Music_ON == 0)
		{
			TONE = 0;
			COUNT = 0;
		}


	/* This code dims the RGB LEDs using PWM */
	if (DIM_Enable > 0)
	{
		if (RED_BRT <= ramp)
		{
			GPIOD->ODR |= (1 << 15);
		}
		else
		{
			GPIOD->ODR &= ~(1 << 15);
		}
		if (BLUE_BRT <= ramp)
		{
			GPIOD->ODR |= (1 << 14);
		}
		else
		{
			GPIOD->ODR &= ~(1 << 14);
		}
		if (GREEN_BRT <= ramp)
		{
			GPIOD->ODR |= (1 << 13);
		}
		else
		{
			GPIOD->ODR &= ~(1 << 13);
		}
	}
  /* USER CODE END TIM7_IRQn 0 */



}