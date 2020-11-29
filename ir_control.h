#include "conf.h"
#ifndef IR_CONTROL_H_INCLUDED
#define IR_CONTROL_H_INCLUDED
#define REMOTE_T 2
#if REMOTE_T ==1
#define EAST 0x16
#define WEST  0x14
#define NORTH 0x11
#define SOUTH 0x19
#define OK  0x15
#define FOCUS_F 0x0A
#define FOCUS_B 0x06
#define GUIDE 1
#define CENTER 2
#define FIND 4
#define SLEW 5
#endif

#if REMOTE_T ==2
#define EAST 0x8
#define WEST  90
#define NORTH 24
#define SOUTH 82
#define OK  28
#define FOCUS_F 67
#define FOCUS_B 9
#define GUIDE 69
#define CENTER 70
#define FIND 71
#define SLEW 68
#endif

#if REMOTE_T ==3
#define EAST 17
#define WEST 15
#define NORTH 19
#define SOUTH 16
#define OK  28
#define FOCUS_F 26
#define FOCUS_B 30
#define GUIDE 14
#define CENTER 29
#define FIND 25
#define SLEW 22
#endif
#if REMOTE_T ==4
#define EAST 91
#define WEST 88
#define NORTH 94
#define SOUTH 86
#define OK  90
#define FOCUS_F 18
#define FOCUS_B 2
#define GUIDE 13
#define CENTER 75
#define FIND 74
#define SLEW 79
#endif

void ir_init(void);
void ir_read(void);
#endif
