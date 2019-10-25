
/* #line 1 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
/*
 * Parses LX200 protocol.
 */

#define ADD_DIGIT(var,digit) var=var*10+digit-'0';
#define APPEND strcat(response,tmessage);
#define SYNC_MESSAGE "sync#"
//#define SYNC_MESSAGE "Coordinates     matched.        #"

#include <string.h>
#include <stdio.h>
#include "mount.h"
#include "misc.h"
#include <math.h>

char response [200];
char tmessage[50];
const int month_days[] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
extern c_star st_now, st_target, st_current;
struct _telescope_
{   long dec_target,ra_target;
    long alt_target,az_target;
    long current_dec,current_ra;
    long lat,longitude;
    char day,month,year,dayofyear;
}
mount;
extern mount_t *telescope;
void lxprintdate(void)
{
    printf ("%02d/%02d/%02d#",mount.month,mount.day,mount.year);
}
void lxprintsite(void)
{
    printf("Site Name#");
};

void ltime(void)
{
    long pj =(long)1;
}

void set_cmd_exe(char cmd,long date)
{
    switch (cmd)
    {
    case 'r':
        mount.ra_target=date;
        break;
    case 'd':
        mount.dec_target=date ;
        break;
    case 'a':
        mount.alt_target=date;
        break;
    case 'z':
        mount.az_target=date ;
        break;
    case 't':
        mount.lat=date ;
        telescope->lat=date;
        break;
    case 'g':
        telescope->longitude=date ;
         telescope->longitude;
        break;
    case 'L' :
        //timer0SetOverflowCount((long) (30.518 *date));
        break;
    case 'S':
        break;

    }
}
void set_date( int day,int month,int year)
{

    mount.month=month;
    mount.day=day;
    mount.year=year;
    mount.dayofyear=day+month_days[month-1];
    if  ((month>2)&&(year%4==0)) mount.dayofyear++;

}
void sync_all(void)
{int temp;
    // mount_test->track=FALSE;
   telescope->altmotor->slewing= telescope->azmotor->slewing=FALSE;
   telescope->ra_target=mount.ra_target*15.0*SEC_TO_RAD;
   telescope->dec_target=mount.dec_target*SEC_TO_RAD;
   telescope->sync=TRUE;
   //sync_ra_dec(telescope);
    sprintf(tmessage,"sync#");
    APPEND

};


//----------------------------------------------------------------------------------------
long command( char *str )
{
    char *p = str, *pe = str + strlen( str );
    int cs;
    char stcmd;
    long deg=0;
    int min=0;
    int sec=0;
    int neg = 1;
    tmessage[0]=0;
    response[0]=0;

    
/* #line 2 "command.cpp" */
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 1, 18, 1, 
	19, 1, 20, 1, 21, 1, 22, 1, 
	25, 1, 26, 2, 1, 23, 2, 2, 
	24
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 6, 7, 9, 10, 22, 23, 
	24, 25, 26, 27, 28, 29, 30, 31, 
	32, 33, 38, 39, 40, 45, 46, 50, 
	53, 54, 64, 65, 67, 69, 70, 72, 
	74, 75, 77, 79, 80, 85, 87, 89, 
	90, 92, 94, 96, 98, 99, 101, 103, 
	110, 114, 116, 118, 118, 120, 122, 124, 
	125, 126, 127, 130
};

static const char _command_trans_keys[] = {
	67, 71, 77, 81, 82, 83, 77, 35, 
	82, 35, 65, 67, 68, 76, 77, 82, 
	83, 90, 100, 103, 114, 116, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	35, 83, 101, 110, 115, 119, 35, 35, 
	35, 101, 110, 115, 119, 35, 67, 71, 
	77, 83, 35, 48, 52, 35, 67, 76, 
	83, 97, 100, 103, 114, 116, 119, 122, 
	32, 48, 57, 48, 57, 47, 48, 57, 
	48, 57, 47, 48, 57, 48, 57, 35, 
	32, 9, 13, 48, 50, 48, 50, 48, 
	57, 58, 48, 53, 48, 57, 46, 58, 
	48, 57, 35, 48, 53, 48, 57, 32, 
	43, 45, 9, 13, 48, 57, 43, 45, 
	48, 57, 48, 57, 48, 57, 48, 53, 
	48, 57, 35, 58, 32, 51, 35, 6, 
	35, 58, 58, 0
};

static const char _command_single_lengths[] = {
	0, 6, 1, 2, 1, 12, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 5, 1, 1, 5, 1, 4, 1, 
	1, 10, 1, 0, 0, 1, 0, 0, 
	1, 0, 0, 1, 1, 0, 0, 1, 
	0, 0, 2, 0, 1, 0, 0, 3, 
	2, 0, 0, 0, 0, 0, 2, 1, 
	1, 1, 3, 1
};

static const char _command_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 1, 1, 0, 1, 1, 
	0, 1, 1, 0, 2, 1, 1, 0, 
	1, 1, 0, 1, 0, 1, 1, 2, 
	1, 1, 1, 0, 1, 1, 0, 0, 
	0, 0, 0, 0
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 7, 9, 12, 14, 27, 29, 
	31, 33, 35, 37, 39, 41, 43, 45, 
	47, 49, 55, 57, 59, 65, 67, 72, 
	75, 77, 88, 90, 92, 94, 96, 98, 
	100, 102, 104, 106, 108, 112, 114, 116, 
	118, 120, 122, 125, 127, 129, 131, 133, 
	139, 143, 145, 147, 148, 150, 152, 155, 
	157, 159, 161, 165
};

static const char _command_indicies[] = {
	0, 2, 3, 4, 5, 6, 1, 7, 
	1, 8, 9, 1, 8, 1, 10, 11, 
	12, 13, 14, 15, 13, 16, 17, 18, 
	19, 20, 1, 21, 1, 22, 1, 23, 
	1, 24, 1, 25, 1, 26, 1, 27, 
	1, 28, 1, 29, 1, 30, 1, 31, 
	1, 32, 33, 33, 33, 33, 1, 34, 
	1, 35, 1, 36, 37, 37, 37, 37, 
	1, 36, 1, 38, 38, 38, 38, 1, 
	39, 40, 1, 39, 1, 41, 42, 42, 
	43, 43, 43, 42, 43, 44, 43, 1, 
	45, 1, 46, 1, 47, 1, 48, 1, 
	49, 1, 50, 1, 51, 1, 52, 1, 
	53, 1, 54, 1, 55, 55, 56, 1, 
	56, 1, 57, 1, 58, 1, 59, 1, 
	60, 1, 61, 62, 1, 63, 1, 64, 
	1, 65, 1, 66, 1, 67, 68, 69, 
	67, 70, 1, 68, 69, 70, 1, 71, 
	1, 72, 1, 73, 74, 1, 75, 1, 
	64, 62, 1, 76, 1, 77, 1, 78, 
	1, 79, 79, 80, 1, 80, 1, 0
};

static const char _command_trans_targs[] = {
	2, 0, 5, 17, 20, 22, 25, 3, 
	59, 4, 6, 7, 8, 9, 10, 11, 
	12, 13, 14, 15, 16, 59, 59, 59, 
	59, 59, 59, 59, 59, 59, 59, 59, 
	18, 19, 59, 59, 59, 21, 23, 59, 
	24, 26, 36, 47, 55, 27, 28, 29, 
	30, 31, 32, 33, 34, 35, 59, 37, 
	38, 39, 40, 41, 42, 43, 45, 44, 
	59, 46, 44, 48, 49, 49, 49, 50, 
	51, 52, 53, 54, 56, 57, 59, 59, 
	1
};

static const char _command_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	39, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 23, 29, 19, 
	43, 31, 17, 21, 27, 37, 25, 35, 
	0, 47, 11, 9, 13, 47, 47, 15, 
	0, 0, 47, 47, 0, 0, 3, 3, 
	0, 5, 5, 0, 1, 1, 49, 0, 
	1, 1, 0, 3, 51, 0, 0, 41, 
	45, 5, 54, 0, 0, 7, 1, 1, 
	1, 0, 3, 51, 0, 0, 33, 0, 
	0
};

static const int command_start = 58;
static const int command_first_final = 58;
static const int command_error = 0;

static const int command_en_main = 58;


/* #line 115 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */





    
/* #line 135 "command.cpp" */
	{
	cs = command_start;
	}

/* #line 138 "command.cpp" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _command_indicies[_trans];
	cs = _command_trans_targs[_trans];

	if ( _command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 122 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ADD_DIGIT(deg,(*p)); }
	break;
	case 1:
/* #line 123 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ADD_DIGIT(min,(*p)); }
	break;
	case 2:
/* #line 124 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ADD_DIGIT(sec,(*p)); }
	break;
	case 3:
/* #line 125 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ neg=-1;}
	break;
	case 4:
/* #line 126 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{mount_move(telescope,stcmd);}
	break;
	case 5:
/* #line 127 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{goto_ra_dec(telescope,mount.ra_target*15.0*SEC_TO_RAD,mount.dec_target*SEC_TO_RAD); sprintf(tmessage,"0");APPEND;}
	break;
	case 6:
/* #line 128 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{mount_stop(telescope,stcmd);}
	break;
	case 7:
/* #line 129 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{select_rate(telescope,stcmd); }
	break;
	case 8:
/* #line 130 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ lxprintra1(tmessage, st_current.ra); APPEND;}
	break;
	case 9:
/* #line 131 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintde1(tmessage, st_current.dec); APPEND;}
	break;
	case 10:
/* #line 132 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ lxprintaz1(tmessage, st_current.az); APPEND;}
	break;
	case 11:
/* #line 133 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintde1(tmessage, st_current.alt); APPEND;}
	break;
	case 12:
/* #line 134 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ lxprintra1(tmessage, st_target.ra); APPEND;}
	break;
	case 13:
/* #line 135 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintde1(tmessage, st_target.dec); APPEND;}
	break;
	case 14:
/* #line 136 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintdate();}
	break;
	case 15:
/* #line 137 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ lxprintsite();}
	break;
	case 16:
/* #line 138 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{;}
	break;
	case 17:
/* #line 139 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintlong1(tmessage,telescope->longitude);APPEND;}
	break;
	case 18:
/* #line 140 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{lxprintlat1(tmessage,telescope->lat);APPEND;}
	break;
	case 19:
/* #line 142 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{sync_all();}
	break;
	case 20:
/* #line 143 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{deg+=((*p)-'0')*6;}
	break;
	case 21:
/* #line 144 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{ ltime();}
	break;
	case 22:
/* #line 145 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{
            set_cmd_exe(stcmd,(neg*(deg )));
            sprintf(tmessage,"1");
            APPEND;
            deg=sec=min=0;
        }
	break;
	case 23:
/* #line 151 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{deg=deg*3600+min*60;}
	break;
	case 24:
/* #line 152 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{deg+=sec;}
	break;
	case 25:
/* #line 153 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{stcmd=(*p);}
	break;
	case 26:
/* #line 154 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */
	{set_date(sec,min,deg);}
	break;
/* #line 297 "command.cpp" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 176 "C:\\Users\\Angel\\Documents\\Arduino\\espgtalt\\espgtalt\\command.rl" */


//---------------------------------------------------------------------------------------------------------------------
    if ( cs < command_first_final )
        //	fprintf( stderr, "LX command:  error\n" );

        return  neg;
};



