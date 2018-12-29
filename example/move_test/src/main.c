#include <drv8830-i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_I2C_DEVICE "/dev/i2c-1"

static void error(const char* message, const char* target){
    char buf[BUFSIZ];
    snprintf(buf, BUFSIZ - 1, "%s in \"%s\" \n", message, target);
    printf(buf);
    exit(-1);
}

int main(int argc, char *argv[]){
    drv8830_conn_t conn;
    const char* dev = DEFAULT_I2C_DEVICE;
    int address_offset = 4;
    int i = 0;

    if(argc > 1) dev = argv[1];
    if(argc > 2) address_offset = atoi(argv[2]);
    if(0 > drv8830_open(&conn, dev, DRV8830_ADDRESS + address_offset, 2)) error("open error", dev);
    
    for(i = 6; i <= 24; i++){
    	if(0 > drv8830_move(&conn, i)) error("move error", dev);
        printf("current speed = %d\n", i);
    	sleep(1);
    }

    for(i = 23; i >= 6; i--){
    	if(0 > drv8830_move(&conn, i)) error("move error", dev);
        printf("current speed = %d\n", i);
    	sleep(1);
    }

    for(i = -6; i >= -24; i--){
    	if(0 > drv8830_move(&conn, i)) error("move error", dev);
        printf("current speed = %d\n", i);
    	sleep(1);
    }

    for(i = -23; i <= -6; i++){
    	if(0 > drv8830_move(&conn, i)) error("move error", dev);
        printf("current speed = %d\n", i);
    	sleep(1);
    }

    if(0 > drv8830_move(&conn, 0)) error("move error", dev);
    
    return 0;
}
