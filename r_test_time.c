#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define WIN32
#ifdef WIN32
#include <windows.h>
#else
//#include <pthread.h>
#include <sys/select.h>
#endif
// =========================== 结构体 ===================================
typedef struct {        /* time struct */
    time_t time;        /* time (s) expressed by standard time_t */
    double sec;         /* fraction of second under 1 s */
} gtime_t;
#define MAXLEAPS    64                  /* max number of leap seconds table */

/*
str2time：输入的是字符串"2022 1 1 14 52 10.5"；
epoch2time：输入的是数组ep[6]，前5个数字的如果是小数会直接舍弃小数部分；

*/
// =========================== 函数声明 ==================================
extern int str2time(const char *s, int i, int n, gtime_t *t);
extern gtime_t epoch2time(const double *ep);
extern void time2epoch(gtime_t t, double *ep);
extern gtime_t gpst2time(int week, double sec);
extern double time2gpst(gtime_t t, int *week);
extern gtime_t gst2time(int week, double sec);
extern double time2gst(gtime_t t, int *week);
extern gtime_t bdt2time(int week, double sec);
extern double time2bdt(gtime_t t, int *week);
extern gtime_t timeadd(gtime_t t, double sec);
extern double timediff(gtime_t t1, gtime_t t2);
extern gtime_t timeget(void);
extern void timeset(gtime_t t);
extern void timereset(void);
extern gtime_t gpst2utc(gtime_t t);
extern gtime_t utc2gpst(gtime_t t);
extern gtime_t gpst2bdt(gtime_t t);
extern gtime_t bdt2gpst(gtime_t t);
static double time2sec(gtime_t time, gtime_t *day);

extern void time2str(gtime_t t, char *s, int n);
extern char *time_str(gtime_t t, int n);
extern double time2doy(gtime_t t);

// =========================== constant ==================================
static const double gpst0[]={1980,1, 6,0,0,0}; /* gps time reference */
static const double gst0 []={1999,8,22,0,0,0}; /* galileo system time reference */
static const double bdt0 []={2006,1, 1,0,0,0}; /* beidou time reference */
static double leaps[MAXLEAPS+1][7]={ /* leap seconds (y,m,d,h,m,s,utc-gpst) */
    {2017,1,1,0,0,0,-18},
    {2015,7,1,0,0,0,-17},
    {2012,7,1,0,0,0,-16},
    {2009,1,1,0,0,0,-15},
    {2006,1,1,0,0,0,-14},
    {1999,1,1,0,0,0,-13},
    {1997,7,1,0,0,0,-12},
    {1996,1,1,0,0,0,-11},
    {1994,7,1,0,0,0,-10},
    {1993,7,1,0,0,0, -9},
    {1992,7,1,0,0,0, -8},
    {1991,1,1,0,0,0, -7},
    {1990,1,1,0,0,0, -6},
    {1988,1,1,0,0,0, -5},
    {1985,7,1,0,0,0, -4},
    {1983,7,1,0,0,0, -3},
    {1982,7,1,0,0,0, -2},
    {1981,7,1,0,0,0, -1},
    {0}
};
// =========================== 函数实现 ==================================
extern int str2time(const char *s, int i, int n, gtime_t *t)
{
    double ep[6];
    char str[256],*p=str;
    
    if (i<0||(int)strlen(s)<i||(int)sizeof(str)-1<i) return -1;
    for (s+=i;*s&&--n>=0;) *p++=*s++;
    *p='\0';
    if (sscanf(str,"%lf %lf %lf %lf %lf %lf",ep,ep+1,ep+2,ep+3,ep+4,ep+5)<6)
        return -1;
    if (ep[0]<100.0) ep[0]+=ep[0]<80.0?2000.0:1900.0;
    *t=epoch2time(ep);
    return 0;
}

extern gtime_t epoch2time(const double *ep)
{
    const int doy[]={1,32,60,91,121,152,182,213,244,274,305,335};
    gtime_t time={0};
    int days,sec,year=(int)ep[0],mon=(int)ep[1],day=(int)ep[2];
    
    if (year<1970||2099<year||mon<1||12<mon) return time;
    
    /* leap year if year%4==0 in 1901-2099 */
    days=(year-1970)*365+(year-1969)/4+doy[mon-1]+day-2+(year%4==0&&mon>=3?1:0);
    sec=(int)floor(ep[5]);
    time.time=(time_t)days*86400+(int)ep[3]*3600+(int)ep[4]*60+sec;
    time.sec=ep[5]-sec;
    return time;
}

extern void time2epoch(gtime_t t, double *ep)
{
    const int mday[]={ /* # of days in a month */
        31,28,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31,
        31,29,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31
    };
    int days,sec,mon,day;
    
    /* leap year if year%4==0 in 1901-2099 */
    days=(int)(t.time/86400);
    sec=(int)(t.time-(time_t)days*86400);
    for (day=days%1461,mon=0;mon<48;mon++) {
        if (day>=mday[mon]) day-=mday[mon]; else break;
    }
    ep[0]=1970+days/1461*4+mon/12; ep[1]=mon%12+1; ep[2]=day+1;
    ep[3]=sec/3600; ep[4]=sec%3600/60; ep[5]=sec%60+t.sec;
}

extern gtime_t gpst2time(int week, double sec)
{
    gtime_t t=epoch2time(gpst0);
    
    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

extern double time2gpst(gtime_t t, int *week)
{
    gtime_t t0=epoch2time(gpst0);
    time_t sec=t.time-t0.time;
    int w=(int)(sec/(86400*7));
    
    if (week) *week=w;
    return (double)(sec-(double)w*86400*7)+t.sec;
}

extern gtime_t gst2time(int week, double sec)
{
    gtime_t t=epoch2time(gst0);
    
    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

extern double time2gst(gtime_t t, int *week)
{
    gtime_t t0=epoch2time(gst0);
    time_t sec=t.time-t0.time;
    int w=(int)(sec/(86400*7));
    
    if (week) *week=w;
    return (double)(sec-(double)w*86400*7)+t.sec;
}

extern gtime_t bdt2time(int week, double sec)
{
    gtime_t t=epoch2time(bdt0);
    
    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

extern double time2bdt(gtime_t t, int *week)
{
    gtime_t t0=epoch2time(bdt0);
    time_t sec=t.time-t0.time;
    int w=(int)(sec/(86400*7));
    
    if (week) *week=w;
    return (double)(sec-(double)w*86400*7)+t.sec;
}

extern gtime_t timeadd(gtime_t t, double sec)
{
    double tt;
    
    t.sec+=sec; tt=floor(t.sec); t.time+=(int)tt; t.sec-=tt;
    return t;
}

extern double timediff(gtime_t t1, gtime_t t2)
{
    return difftime(t1.time,t2.time)+t1.sec-t2.sec;
}

static double timeoffset_=0.0;        /* time offset (s) */

extern gtime_t timeget(void)
{
    gtime_t time;
    double ep[6]={0};
#ifdef WIN32
    SYSTEMTIME ts;
    
    GetSystemTime(&ts); /* utc */
    ep[0]=ts.wYear; ep[1]=ts.wMonth;  ep[2]=ts.wDay;
    ep[3]=ts.wHour; ep[4]=ts.wMinute; ep[5]=ts.wSecond+ts.wMilliseconds*1E-3;
#else
    struct timeval tv;
    struct tm *tt;
    
    if (!gettimeofday(&tv,NULL)&&(tt=gmtime(&tv.tv_sec))) {
        ep[0]=tt->tm_year+1900; ep[1]=tt->tm_mon+1; ep[2]=tt->tm_mday;
        ep[3]=tt->tm_hour; ep[4]=tt->tm_min; ep[5]=tt->tm_sec+tv.tv_usec*1E-6;
    }
#endif
    time=epoch2time(ep);
    
#ifdef CPUTIME_IN_GPST /* cputime operated in gpst */
    time=gpst2utc(time);
#endif
    return timeadd(time,timeoffset_);
}

extern void timeset(gtime_t t)
{
    timeoffset_+=timediff(t,timeget());
}

extern void timereset(void)
{
    timeoffset_=0.0;
}

extern gtime_t gpst2utc(gtime_t t)
{
    gtime_t tu;
    int i;
    
    for (i=0;leaps[i][0]>0;i++) {
        tu=timeadd(t,leaps[i][6]);
        if (timediff(tu,epoch2time(leaps[i]))>=0.0) return tu;
    }
    return t;
}

extern gtime_t utc2gpst(gtime_t t)
{
    int i;
    
    for (i=0;leaps[i][0]>0;i++) {
        if (timediff(t,epoch2time(leaps[i]))>=0.0) return timeadd(t,-leaps[i][6]);
    }
    return t;
}

extern gtime_t gpst2bdt(gtime_t t)
{
    return timeadd(t,-14.0);
}

extern gtime_t bdt2gpst(gtime_t t)
{
    return timeadd(t,14.0);
}

static double time2sec(gtime_t time, gtime_t *day)
{
    double ep[6],sec;
    time2epoch(time,ep);
    sec=ep[3]*3600.0+ep[4]*60.0+ep[5];
    ep[3]=ep[4]=ep[5]=0.0;
    *day=epoch2time(ep);
    return sec;
}

extern void time2str(gtime_t t, char *s, int n)
{
    double ep[6];
    
    if (n<0) n=0; else if (n>12) n=12;
    if (1.0-t.sec<0.5/pow(10.0,n)) {t.time++; t.sec=0.0;};
    time2epoch(t,ep);
    sprintf(s,"%04.0f/%02.0f/%02.0f %02.0f:%02.0f:%0*.*f",ep[0],ep[1],ep[2],
            ep[3],ep[4],n<=0?2:n+3,n<=0?0:n,ep[5]);
}

extern char *time_str(gtime_t t, int n)
{
    static char buff[64];
    time2str(t,buff,n);
    return buff;
}

extern double time2doy(gtime_t t)
{
    double ep[6];
    
    time2epoch(t,ep);
    ep[1]=ep[2]=1.0; ep[3]=ep[4]=ep[5]=0.0;
    return timediff(t,epoch2time(ep))/86400.0+1.0;
}

// ============================ test ===================================

int main(int argc, char* argv[]){
    
    // ============================ 输入 ===================================
    // 输入要计算的时间
    // 所有时间都要转化为 utc的gtime_t
    gtime_t time={0};
    char *str_t;
    double doy,sod,ep[6],sec;
    int week,year;

    // 1. 输入当前时间utc
    #if 1
    // timeoffset_=8*3600.0;
    time = timeget();
    #endif

    // 2. 输入字符串utc    
    #if 0
    str_t= "1971 1 3 0 0 0.55";
        str2time(str_t,0, strlen(str_t),&time);
    #endif

    // 3. 输入6元数组utc
    #if 1
    double e[6] = {2022, 8, 2, 12, 52, 10.6};
        memcpy(ep,e,sizeof(double)*6);
        time = epoch2time(ep);
    #endif

    // 4. 输入年积日utc
    #if 0 
    year = 2022;
    doy = 1;
    sod = 65.2;
        ep[0]=year;
        ep[1]=ep[2]=1;
        ep[3]=ep[4]=ep[5]=0.0;
        time = epoch2time(ep);
        time.time += (doy-1)*24*3600 + floor(sod);
        time.sec = sod-floor(sod);
    #endif

    // 5. 输入gpst；
    #if 0
    week = 0;
    sec = 100;
    time = gpst2time(week,sec);
    time = gpst2utc(time);
    #endif

    // 6. 输入bdt;
    #if 0
    week = 0;
    sec = 100;
    time = bdt2time(week,sec); // bdt
    time = bdt2gpst(time); // 变为gpst
    time = gpst2utc(time); // 变为utc
    #endif

    // ============================ 输出 ===================================
    printf("\n各种格式的时间：");
    // 输出gtime_t
    printf("\n\n✅ gtime_t: time=%ld sec=%.2f",time.time,time.sec);

    // 输出字符串
    str_t = time_str(time,1);
    printf("\n\n✅ 字符串格式：%s",str_t);

    // 输出6元数组
    time2epoch(time,ep);
    printf("\n\n✅ 6元数组：ep[]=[%4d, %2d, %2d, %2d, %2d, %5.2f]",(int)ep[0],
        (int)ep[1],(int)ep[2],(int)ep[3],(int)ep[4],ep[5]);
    
    // 输出年积日
    gtime_t time_temp;
    doy = time2doy(time);
    sod = time2sec(time,&time_temp);
    time2epoch(time_temp,ep);
    printf("\n\n✅ 年积日： year=%4d doy=%03d sod=%.2f",(int)ep[0],(int)doy,sod);

    // 输出gpst
    time_temp = utc2gpst(time);
    sec = time2gpst(time_temp,&week);
    printf("\n\n✅ GPS时间：week=%d sec=%.2f",week,sec);

    // 输出bdt
    time_temp = utc2gpst(time);
    time_temp = gpst2bdt(time_temp);
    sec = time2bdt(time_temp,&week);
    printf("\n\n✅ 北斗时间：week=%d sec=%.2f",week,sec);


    
    printf("\n\n");
    return 0;
}