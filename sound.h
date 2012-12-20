#ifndef __SOUND_H__
#define __SOUND_H__

#include "macros.h"

#define SOUND_DDRX DDRB
#define SOUND_PIN 1

#define T(x) (F_CPU/(x) - 1)

#define NOTE(dl, oct, note) {dl, T(_BV(oct-1)*note)}
#define PAUSE(dl) {dl,0}
#define STOP {0,0}

/*
 Melody examples
const melody_t melody1[] =
{
	NOTE(dl32, 3, _A),
	NOTE(dl32, 3, _F),
	NOTE(dl32, 3, _Am),
	NOTE(dl32, 3, _D),

	PAUSE(dl32),

	NOTE(dl32, 3, _A),
	NOTE(dl32, 3, _F),
	NOTE(dl32, 3, _Am),
	NOTE(dl32, 3, _D),
	PAUSE(dl32),

	NOTE(dl32, 3, _A),
	NOTE(dl32, 3, _F),
	NOTE(dl32, 3, _Am),
	NOTE(dl32, 3, _D),

	PAUSE(dl16),
	PAUSE(dl32),

	STOP
};

const melody_t melody2[] =
{
	NOTE(dl8, 1, _C),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _G),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _F),
	NOTE(dl8, 1, _C),
	NOTE(dl8, 1, _C),
	NOTE(dl8, 1, _C),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _A),
	NOTE(dl8, 1, _B),
	NOTE(dl8, 1, _G),
	NOTE(dl4, 2, _C),

	STOP
};
*/

typedef struct { uint8_t len; uint16_t note; } melody_t;

// Длительности
#define dl1    0x40 //в 64-х
#define dl2    0x20
#define dl4    0x10
#define dl8    0x08
#define dl16   0x04
#define dl32   0x02
#define dl64   0x01

#define F1 261.63
#define F2 277.18
#define F3 293.66
#define F4 311.13
#define F5 329.63
#define F6 349.23
#define F7 369.99
#define F8 392.00
#define F9 415.30
#define F10 440.00
#define F11 466.16
#define F12 493.88

#define _C   F1
#define _Cd  F2
#define _Dm  F2
#define _D   F3
#define _Dd  F4
#define _Em  F4
#define _E   F5
#define _F   F6
#define _Fd  F7
#define _Gm  F7
#define _G   F8
#define _Gd  F9
#define _Am  F9
#define _A   F10
#define _Ad  F11
#define _B   F11
#define _H   F12

extern void sound_init(void);
extern void start_sound(const melody_t* melody, uint8_t loop);
extern void stop_sound(void);

#endif
