#include "mount.h"
#include "misc.h"

extern long sdt_millis;
extern c_star  st_now, st_target, st_current;
char sel_flag;
char volatile one=FALSE;
mount_t* create_mount(void)

{
    int maxcounter = AZ_RED;
    int maxcounteralt = ALT_RED;
    mount_t *m;
    m = (mount_t*)malloc(sizeof(mount_t));
    //if (m) return NULL;
    m->azmotor = (motor_t*)malloc(sizeof(motor_t));
    m->altmotor = (motor_t*)malloc(sizeof(motor_t));
    m->track = 0;
    m->rate[3][0] = RATE_SLEW;
    m->rate[2][0] = RATE_FIND;
    m->rate[1][0] = RATE_CENTER;
    m->rate[0][0] = RATE_GUIDE;
    m->rate[3][1] = RATE_SLEW;
    m->rate[2][1] = RATE_FIND;
    m->rate[1][1] = RATE_CENTER;
    m->rate[0][1] = RATE_GUIDE;
    m->srate = 0;
    m->maxspeed[0] = (m->rate[3][0] * SID_RATE * SEC_TO_RAD);
    m->maxspeed[1] = (m->rate[3][1] * SID_RATE * SEC_TO_RAD);
    m->longitude = LOCAL_LONGITUDE;
    m->lat = LOCAL_LATITUDE;
    m->time_zone = TIME_ZONE;
    m->prescaler = 0.4;
//  init_motor( m->azmotor, AZ_ID, maxcounter, SID_RATE * SEC_TO_RAD, m->prescaler, m->maxspeed[0]);
//  init_motor( m->altmotor,  ALT_ID, maxcounteralt, 0, m->prescaler, m->maxspeed[1]);
    m->is_tracking=TRUE;
    m->mount_mode=ALTAZ;
    m->sync=FALSE;
    return m;
}

int  destroy_mount(mount_t* m)
{
    free(m->azmotor);
    free(m->altmotor);
    free(m);
}

void thread_motor(mount_t* m)
{
    speed_up_down(((mount_t*)m)->altmotor);
    speed_up_down(((mount_t*)m)->azmotor);
}
void thread_motor2(mount_t* m)
{
    speed_up_down(((mount_t*)m)->altmotor);
    speed_up_down(((mount_t*)m)->azmotor);
    if (sel_flag)
    {
        pollcounters( AZ_ID);
        sel_flag = false;
    }

}

int goto_ra_dec(mount_t *mt,double ra,double dec)
{
    st_target.ra=ra;
    st_target.dec=dec;

}

int sync_ra_dec(mount_t *mt)
{
   // one=FALSE;

    while (Serial.available()) Serial.read();
    st_current.timer_count =((millis()-sdt_millis)/ 1000.0);//chrono_read(&ti);
    st_current.dec= st_target.dec=mt->dec_target;
    st_current.ra=st_target.ra=mt->ra_target;
    to_alt_az(&st_current);

    setposition(mt->azmotor,st_current.az/mt->azmotor->resolution);

    if (st_current.alt>=0.0)
        setposition(mt->altmotor,st_current.alt/mt->altmotor->resolution);
    else
        setposition(mt->altmotor,( M_2PI+st_current.alt)/mt->altmotor->resolution);
      mt->sync=FALSE;
}

int mount_stop(mount_t *mt, char direction)
{
    char n=0;
    char top=200;
    mt->altmotor->slewing= mt->azmotor->slewing=FALSE;
    switch (direction)
    {
    case 'n':
    case 's':
        mt->altmotor->targetspeed=0.0;
        do
        {
            yield();
            delay(5);
            n++;
        }
        while ((n<top) && (fabs(mt->altmotor->current_speed)>0.0));
        break;


    case 'w':
    case 'e':
        mt->azmotor->targetspeed=0.0;
        do
        {
            yield();
            delay(5);
            n++;
        }
        while ((n<top) && (fabs(mt->azmotor->current_speed)>0.0));

        break;

    default:
        mt->altmotor->targetspeed=0.0;

        break;
    };
    one=FALSE;
    mt->is_tracking=TRUE;

}

void mount_move(mount_t *mt, char dir)
{
    mt->altmotor->slewing = mt->azmotor->slewing = FALSE;
    mt->is_tracking=FALSE;
    int srate = mt->srate;
//  int invert = (get_pierside(mt)) ? -1 : 1;
    switch (dir)
    {
    case 'n':
        mt->altmotor->targetspeed = SID_RATE * mt->rate[srate][1] * SEC_TO_RAD;
        break;
    case 's':
        mt->altmotor->targetspeed = -SID_RATE * mt->rate[srate][1] * SEC_TO_RAD;
        break;
    case 'w':
        mt->azmotor->targetspeed = SID_RATE * mt->rate[srate][0]  * SEC_TO_RAD;
        break;
    case 'e':
        mt->azmotor->targetspeed = -SID_RATE * mt->rate[srate][0]  * SEC_TO_RAD;
        break;
    };
}

void select_rate(mount_t *mt, char dir)
{
    switch (dir)
    {
    case 'C':
        mt->srate = 1;
        break;
    case 'G':
        mt->srate = 0;
        break;
    case 'M':
        mt->srate = 2;
        break;
    case 'S':
        mt->srate = 3;
        break;


    };

}



int get_pierside(mount_t *mt)
{

    return (((mt->altmotor->counter) > (mt->altmotor->maxcounter / 4 )) && ((mt->altmotor->counter) < (3 * mt->altmotor->maxcounter / 4 )));
}

void mount_lxde_str(char* message, mount_t *mt)

{
    double ang = mt->altmotor->position;
    if (ang > 1.5 * M_PI) ang = ang - (M_PI * 2.0)
                                    ;
    else if (ang > M_PI / 2.0) ang = M_PI - ang;

    int x = ang * RAD_TO_DEG * 3600.0;
    char c = '+';
    if (x < 0)
    {
        x = -x;
        c = '-';
    }
    int gra = x / 3600;
    int temp = (x % 3600);
    int min = temp / 60;
    int sec = temp % 60;
    sprintf(message, "%c%02d%c%02d:%02d#", c, gra, 225, min, sec);


};

int readconfig(mount_t *mt)
{
    int maxcounter, maxcounteralt ;
    File f = SPIFFS.open("/mount.config", "r");
    if (!f) return -1;
    String s = f.readStringUntil('\n');
    maxcounter = s.toInt();
    s = f.readStringUntil('\n');
    maxcounteralt = s.toInt();
    for (int j = 0; j < 2; j++)
        for (int n = 0; n < 4; n++)
        {
            s = f.readStringUntil('\n');
            mt->rate[n][j] = s.toFloat();
        };
    mt->srate = 0;
    mt->maxspeed[0] = (mt->rate[3][0] * SID_RATE * SEC_TO_RAD);
    mt->maxspeed[1] = (mt->rate[3][1] * SID_RATE * SEC_TO_RAD);
    s = f.readStringUntil('\n');
    mt->prescaler = s.toFloat();
    if ((mt->prescaler < 0.3) || (mt->prescaler > 2.0)) mt->prescaler = 0.4;
    s = f.readStringUntil('\n');
    mt->longitude = s.toFloat();
    s = f.readStringUntil('\n');
    mt->lat = s.toFloat();
    s = f.readStringUntil('\n');
    mt->time_zone = s.toFloat();
    init_motor( mt->azmotor, AZ_ID, maxcounter, 0, mt->prescaler, mt->maxspeed[0]);
    init_motor( mt->altmotor,  ALT_ID, maxcounteralt, 0, mt->prescaler, mt->maxspeed[1]);
    return 0;


}
void mount_park(mount_t *mt)

{
    mt->altmotor->slewing = mt->azmotor->slewing = mt->is_tracking=FALSE;
    mt->altmotor->targetspeed = 0.0;
    mt->azmotor->targetspeed = 0.0;

    delay(100);
    save_counters(ALT_ID);
    delay(10);
    save_counters(AZ_ID);
    delay(10);
}


void  tak_init(mount_t *mt)
{

    reset_transforms(0.0, 0.0, 0.0);
    double temp=sidereal_timeGMT (mt->longitude, mt->time_zone)*15.0;
    sdt_millis=millis();
    if  (mt->mount_mode == ALTAZ)
    {

        set_star(&st_now, temp + 90.0, 0.0, 90.0, 0.0, 0);
        init_star(1, &st_now);
        set_star(&st_now,temp, mt->lat, 180.00, 89.99, 0);
        init_star(2, &st_now);

    }
    else if (mt->mount_mode == EQ)
    {
        double ra    ;
        set_star(&st_now,  temp, 0.0, 180.0, 0.0, 0);
        init_star(1, &st_now);
        ra = st_now.ra + M_PI / 2.0;
        if (ra < 0) ra += M_2PI;
        if (mt->lat >= 0.0)
            set_star(&st_now, ra * RAD_TO_DEG, 45, 90, 45, 0);
        else
            set_star(&st_now, ra * RAD_TO_DEG, -45, 270, 45, 0);
        init_star(2, &st_now);


    }
    compute_trasform();
    set_star(&st_now,temp,mt->lat, 0.0, 0.0,0);
    to_alt_az(&st_now);
    //  is_aligned=0;
    //  is_slewing='0';
    //  counter_x=counter_y=0;

}


void track(mount_t *mt)
{
    double d_az_r, d_alt_r;
    int aval = Serial.available();
    if (aval >= 18)
    {
        while (Serial.available() > 18) Serial.read();
        readcounter_n(mt->altmotor);
        readcounter(mt->azmotor);
        st_target.timer_count = st_current.timer_count = ((millis()-sdt_millis)/ 1000.0);
        st_current.az = mt->azmotor->position;
        st_current.alt = mt->altmotor->position;
        //compute ecuatorial current equatorial values to be send out from LX200 protocol interface
        to_equatorial(&st_current);

        if (mt->is_tracking && one)
        {
            //compute next alt/az mount values  for target next lap second
            st_target.timer_count += 1.0;
            to_alt_az(&st_target);
            //compute delta values :next values from actual values for desired target coordinates
            d_az_r = (st_target.az) - st_current.az;
            if (fabs(d_az_r) > (M_PI)) d_az_r -= M_2PI;
            d_alt_r = (st_target.alt) - st_current.alt;;
            if (fabs(d_alt_r) > (M_PI)) d_alt_r -= M_2PI;

            // Compute and set timer intervals for stepper  rates
            //speed_x_a(d_az_r);
            settargetspeed(mt->azmotor, d_az_r);
            settargetspeed(mt->altmotor, d_alt_r);
            // is_slewing='0';
        }
        else
        {
            // settargetspeed(mt->azmotor, az_frate);
            // settargetspeed(mt->altmotor, alt_frate);
            //  if (trackmode == 1) {
            //    tracking = (speed_y == 0.0) && (speed_x == 0.0);
            st_target.ra = st_current.ra;
            st_target.dec = st_current.dec;
            one= TRUE;
        }
    }
    while (Serial.available()) Serial.read();
    if (mt->sync) sync_ra_dec(mt);
    pollcounters( ALT_ID);
    sel_flag = true;

}
