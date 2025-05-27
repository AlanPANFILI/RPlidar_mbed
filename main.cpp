#include "lidar_alan.h"
#include "mbed.h"


UnbufferedSerial lidar(PA_9, PA_10, 256000);
PwmOut rotation(PA_8);

int main() {
	
	    initLidar(lidar, rotation, 20);
        lidarStartScan();
		while(1){
			for (int i = 0 ; i < 360 ; i++{
				printf("distance a %d est de %d mm \n\r",i,distance_scan[i]);
			}				
		}
}
