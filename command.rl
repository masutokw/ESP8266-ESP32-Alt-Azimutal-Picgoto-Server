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

    %%{
        machine command;
        write data;
    }%%




    %%{
#Acciones
        action getgrads {ADD_DIGIT(deg,fc); }
        action getmin {ADD_DIGIT(min,fc); }
        action getsec {ADD_DIGIT(sec,fc); }
        action neg { neg=-1;}
        action dir {mount_move(telescope,stcmd);}
        action Goto {goto_ra_dec(telescope,mount.ra_target*15.0*SEC_TO_RAD,mount.dec_target*SEC_TO_RAD); sprintf(tmessage,"0");APPEND;}
        action stop {mount_stop(telescope,stcmd);}
        action rate {select_rate(telescope,stcmd); }
        action return_ra { lxprintra1(tmessage, st_current.ra); APPEND;}
        action return_dec {lxprintde1(tmessage, st_current.dec); APPEND;}
        action return_az { lxprintaz1(tmessage, st_current.az); APPEND;}
        action return_alt {lxprintde1(tmessage, st_current.alt); APPEND;}
        action return_ra_target { lxprintra1(tmessage, st_target.ra); APPEND;}
        action return_dec_target {lxprintde1(tmessage, st_target.dec); APPEND;}
        action return_date {lxprintdate();}
        action return_site { lxprintsite();}
        action ok {;} # {sprintf(tmessage,"1");}
        action return_longitude {lxprintlong1(tmessage,telescope->longitude);APPEND;}
        action return_lat {lxprintlat1(tmessage,telescope->lat);APPEND;}
        action return_sid_time { ;}
        action sync {sync_all();}
        action rafrac {deg+=(fc-'0')*6;}
        action return_local_time { ltime();}
        action set_cmd_exec {
            set_cmd_exe(stcmd,(neg*(deg )));
            sprintf(tmessage,"1");
            APPEND;
            deg=sec=min=0;
        }
        action addmin {deg=deg*3600+min*60;}
        action addsec {deg+=sec;}
        action storecmd {stcmd=fc;}
        action setdate {set_date(sec,min,deg);}
        action return_align{sprintf(tmessage,"A"); APPEND; }

#definicion sintaxis LX terminos auxiliares
        sexmin =  ([0-5][0-9])$getmin@addmin ;
        sex= ([0-5][0-9] )$getsec@addsec;
        deg =(([\+] | [\-]@neg) |(digit @getgrads))(digit @getgrads){2} any sexmin ([\:]  sex)? ;
        RA = ([0-2] digit) $getgrads   ':' sexmin ('.'digit@rafrac | ':' sex) ;
        date = digit{2}$getmin "/" digit{2}$getsec "/" digit{2}$getgrads ;
#Definicion sintaxis comandos
        Poll= 'G'( 'R'%return_ra | 'D'%return_dec| 'Z'%return_az |'A'%return_alt |'r'%return_ra_target | 'd'%return_dec_target | 'L'%return_local_time |'S'%return_local_time|'C'%return_date|'M'%return_site|'t'%return_longitude|'g'%return_lat);
        Move = 'M' ([nswe]@storecmd %dir | 'S'%Goto);
        Rate = 'R' [CGMS]@storecmd (''|[0-4]) %rate;
        Set='S'(((([dazgt]@storecmd (''|space) deg ) | ([rLS]@storecmd (''|space) RA))%set_cmd_exec)|'C 'date%setdate|'w 3'%ok);
        Sync = "CM"(''|'R')%sync;
        Stop ='Q' (''|[nsew])@storecmd %stop;
        Mode =''@return_align;
        main :=  ((0x06)Mode |''|'#') (':' (Set | Move | Stop|Rate | Sync | Poll) '#')* ;

# Initialize and execute.
        write init;
        write exec;
    }%%

//---------------------------------------------------------------------------------------------------------------------
    if ( cs < command_first_final )
        //	fprintf( stderr, "LX command:  error\n" );

        return  neg;
};



