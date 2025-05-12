
#ifndef lidar_alan_h
#define lidar_alan_h

#include "mbed.h"

extern char dataLidar;
extern char etatScan;
extern int distance_scan[360];
extern char dataScanCpt;

extern char ADRESSE_LIDAR;
extern char LIDAR_RESET;
extern char LIDAR_STOP;
extern char LIDAR_SCAN;


void initLidar(UnbufferedSerial &UserLidar, PwmOut &UserPwmPin, int UserPwm);
void lidarStartScan(void);
void lidarStopScan(void);
void interuptionLidar(void);

#endif