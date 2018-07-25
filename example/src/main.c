#include <drv8830-i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_I2C_DEVICE "/dev/i2c-1"

static void error(const char* message, const char* target){
    char buf[BUFSIZ];
    snprintf(buf, BUFSIZ - 1, "%s in \"%s\" ", message, target);
    printf(buf);
    exit(-1);
}

int main(int argc, char *argv[]){
    drv8830_conn_t conn;
    const char* dev = DEFAULT_I2C_DEVICE;
    int i = 0;

    if(argc > 1) dev = argv[1];
    if(0 > drv8830_open(&conn, dev, DRV8830_ADDRESS + 4, 2)) error("open error", dev);
    
    for(i = 1; i <= 8; i++){
    	if(0 > drv8830_move(&conn, i * 8)) error("move error", dev);
    	sleep(3);
    }

    for(i = 7; i >= -8; i--){
    	if(0 > drv8830_move(&conn, i * 8)) error("move error", dev);
    	sleep(3);
    }

    for(i = -7; i <= 0; i++){
    	if(0 > drv8830_move(&conn, i * 8)) error("move error", dev);
    	sleep(3);
    }

    return 0;
}
