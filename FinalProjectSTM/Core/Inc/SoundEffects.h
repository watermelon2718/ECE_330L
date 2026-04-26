#ifndef SOUND_EFFECTS_
#define SOUND_EFFECTS_
#include "Music.h" // include music definitions


//effect to play when switching from one player to another
static const Note effect_switch[] = {
    {E3,        _8th,  4000,  10,    0}, // 0
    {C3,        _8th,  4000,  10,    0}, // 0
    {E3,        _8th,  4000,  10,    0}, // 0
    {C3,     quarter,  4000,  10,    0},

    {rest,      _8th,  4000,  40,    1}
};

//play when ship placed
static const Note effect_place[] = {
    {A2,        _8th,  1000,  10,    0}, // 0
    {rest,      _8th,  1000,  40,    0},

    {rest,      _8th,  4000,  40,    1}
};

//play when guess is a miss
static const Note effect_miss[] = {
    {As4_Bb4,  _8th,   800,  10,    0}, // 0
    {A4,        _8th,  800,  10,    0}, // 0
    {Gs4_Ab4,   _8th,  800,  10,    0}, // 0
    {G4,     quarter,  800,  10,    0},

    {rest,      quarter,  3000,  40,    0},
    
    // {E3,        _8th,  4000,  10,    0}, // 0
    // {C3,        _8th,  4000,  10,    0}, // 0
    // {E3,        _8th,  4000,  10,    0}, // 0
    // {C3,     quarter,  4000,  10,    0},

    {rest,      _8th,  4000,  40,    1}
};

//play when player wins
static const Note effect_win[] = {
    {E3,        _8th,  4000,  10,    0}, // 0
    {C3,        _8th,  4000,  10,    0}, // 0
    {E3,        _8th,  4000,  10,    0}, // 0
    {C3,     quarter,  4000,  10,    0}, // 0

    {rest,      _8th,  4000,  40,    1}
};

//play when ship hit
static const Note effect_hit[] = {
    {C3,        _8th,  1500,  0,    0}, // 0
    {G3,        _8th,  1500,  0,    0}, // 0
    {D4,        _8th,  1500,  0,    0}, // 0
    {Gs4_Ab4,   _8th,  1500,  0,    0}, // 0
    {C5,       whole,  1500,  0,    0}, // 0

    {rest,      _8th,  1000,  40,    1}
};

#endif
