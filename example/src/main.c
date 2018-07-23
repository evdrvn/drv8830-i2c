#include <drv8830-i2c.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_I2C_DEVICE "/dev/i2c-1"

static void error(const char* message, const char* target){
    char buf[BUFSIZ];
    snprintf(buf, BUFSIZ - 1, "%s in \"%s\" ", message, target);
    perror(buf);
    exit(-1);
}

int main(int argc, char *argv[]){
    drv8830_conn_t conn;
    const char* dev = DEFAULT_I2C_DEVICE;
    
    if(argc > 1) dev = argv[1];
    if(0 > drv8830_open(&conn, dev, DRV8830_ADDRESS, 2)) error("open error", dev);

    return 0;
}
