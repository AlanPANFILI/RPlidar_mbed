#include "lidar_alan.h"
#include "mbed.h"

char ADRESSE_LIDAR = 0xA5;
char LIDAR_RESET = 0x40;
char LIDAR_STOP = 0x25;
char LIDAR_SCAN = 0x20;

char dataLidar = 0;
int angleLidar = 0;
int distanceLidar = 0;

char interrupLidarAttache = 0;

int distance_scan[360] = {0};
int data_recu[10] = {0};

UnbufferedSerial *lidarPointeur = NULL;
PwmOut *pwmPointeur = NULL;

void initLidar(UnbufferedSerial &UserLidar, PwmOut &UserPwmPin, int UserPwm) {

  lidarPointeur = &UserLidar;
  pwmPointeur = &UserPwmPin;

  if (interrupLidarAttache == 0) {
    lidarPointeur->attach(&interuptionLidar, UnbufferedSerial::RxIrq);
    interrupLidarAttache = 1;
  }

  pwmPointeur->period(1 / 25000.0); // 25kHz
  pwmPointeur->pulsewidth((UserPwm / 25000.0) * 0.01);
  wait_us(500000);

  lidarPointeur->write(&ADRESSE_LIDAR, 1);
  lidarPointeur->write(&LIDAR_RESET, 1);
  wait_us(500000);

  lidarPointeur->write(&ADRESSE_LIDAR, 1);
  lidarPointeur->write(&LIDAR_STOP, 1);
  wait_us(500000);
}

void lidarStartScan(void) {
  lidarStopScan();
  lidarPointeur->write(&ADRESSE_LIDAR, 1);
  lidarPointeur->write(&LIDAR_SCAN, 1);
  wait_us(500000);
}

void lidarStopScan(void) {
  lidarPointeur->write(&ADRESSE_LIDAR, 1);
  lidarPointeur->write(&LIDAR_STOP, 1);
  wait_us(500000);
}

void interuptionLidar(void) {

  lidarPointeur->read(&dataLidar, 1);

  data_recu[8] = dataLidar;
  for (int i = 0; i < 8; i++) {
    data_recu[i] = data_recu[i + 1];
  }

  if ((data_recu[0] & 0x01) != (data_recu[0] & 0x02) &&
      ((data_recu[1] & 0x1) == 1)) {

    angleLidar = data_recu[1] >> 7;
    angleLidar = angleLidar + (data_recu[2] << 1);
    angleLidar = angleLidar % 360;

    distanceLidar = data_recu[3] >> 2;
    distanceLidar = distanceLidar + (data_recu[4] << 6);

    distance_scan[angleLidar] = distanceLidar;

    for (int i = 0; i < 8; i++) {
      data_recu[i] = 0;
    }
  }
}
