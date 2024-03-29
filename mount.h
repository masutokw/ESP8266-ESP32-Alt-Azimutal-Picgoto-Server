#ifndef MOUNT_H_INCLUDED
#define MOUNT_H_INCLUDED
#include "conf.h"
#include <FS.h>
#include "piclevel.h"
#include "motor.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "taki.h"
#define SID_RATE 15.04106711786691
#define ARC_SEC_LMT 1.0
#define AZ_RED 8000*6*180
#define ALT_RED  8000*6*180
#define RATE_GUIDE 0.5
#define RATE_CENTER 8
#define RATE_FIND 50
#define RATE_SLEW 290
#define LOCAL_LONGITUDE -4.2
#define LOCAL_LATITUDE 36.72
#define ALT_ID 0XFD
#define AZ_ID 0xFE
#define TIME_ZONE 1
typedef enum {ALTAZ,EQ,ALT_ALT} mount_mode_t;
typedef struct
{
  mount_mode_t mount_mode;
  motor_t *altmotor,*azmotor;
    double dec_target,ra_target;//radians
    double alt_target,az_target;
    double lat,longitude;
    double rate[4][2];
    double prescaler;
    double maxspeed[2];
    int srate;
    int track;
    int time_zone;
    char is_tracking;
    char sync;
    int smode;
} mount_t;

mount_t* create_mount(void);
int readconfig(mount_t *mt);
int destroy_mount(mount_t* m);
void mount_move(mount_t *mt,char direction);
int mount_stop(mount_t *mt,char direction);
void select_rate(mount_t *mt,char rate);
int sync_ra_dec(mount_t *mt);
void thread_motor(mount_t* m);
void thread_motor2(mount_t* m);
int get_pierside(mount_t *mt);
int goto_ra_dec(mount_t *mt,double ra,double dec);
void mount_park(mount_t *mt);
void mount_home_set(mount_t *mt);
void  tak_init(mount_t *mt);
void track(mount_t *mt1);
void align_sync_all(mount_t *mt,long ra,long dec);
void mount_track_off(mount_t *mt);
#endif
