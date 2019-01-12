#include "drv8830-i2c.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>

typedef uint8_t byte;

//#define DRV8830_TRACE 
#ifdef DRV8830_TRACE
#define TRACE(fmt, ...) printf("##TRACE## " fmt "\n", ##__VA_ARGS__); fflush(stdout)
#else
#define TRACE(fmt, ...) (void)sizeof(printf(fmt,##__VA_ARGS__))
#endif

int drv8830_writebytes(drv8830_conn_t* conn, uint8_t *data, uint8_t count){
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    int ret;

    messages[0].addr = conn->address;
    messages[0].flags = 0;
    messages[0].len = count;
    messages[0].buf = (unsigned char *)data;

    packets.msgs = messages;
    packets.nmsgs = 1;

    ret = ioctl(conn->file, I2C_RDWR, &packets);

#ifdef DRV8830_TRACE
    int _trace_i;
    printf("write -> ");
    for(_trace_i = 0; _trace_i < count; _trace_i++){ 
        printf(" %02x", data[_trace_i]); 
    }
    printf("\n");
#endif

    return ret;
}

int drv8830_readbytes(drv8830_conn_t* conn, uint8_t reg, uint8_t *dest, uint8_t count){
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    int ret = 0;
    /* write the register we want to read from */
    messages[0].addr = conn->address; 
    messages[0].flags = 0;        
    messages[0].len = 1;         
    messages[0].buf = (unsigned char *)&reg;

    /* read */
    messages[1].addr = conn->address;  
    messages[1].flags = I2C_M_RD;
    messages[1].len = count;    
    messages[1].buf = (unsigned char *)dest;

    packets.msgs = messages;
    packets.nmsgs = 2;

    ret = ioctl(conn->file, I2C_RDWR, &packets);
   
#ifdef DRV8830_TRACE
    int _trace_i;
    printf("read [%02x] -> ", reg);
    for(_trace_i = 0; _trace_i < count; _trace_i++){ 
        printf(" %02x", dest[_trace_i]); 
    }
    printf("\n");
#endif

    return ret;
}

static byte read8(drv8830_conn_t* conn, drv8830_reg_t reg){
    byte val = 0;
    drv8830_readbytes(conn, reg, &val, 1);
    return val;
}

int drv8830_open(drv8830_conn_t* conn, const char* i2cdev, uint8_t address, int timeout_10ms){
    int fd, ret;

    if ((fd = open (i2cdev, O_RDWR)) < 0)
        return fd;
    if ((ret = ioctl (fd, I2C_SLAVE, address)) < 0)
        return ret;
    if ((ret = ioctl (fd, I2C_TIMEOUT, timeout_10ms)) < 0)
        return ret;
 
    conn->file = fd;
    conn->address = address;
    
    return fd;
}

int drv8830_move(drv8830_conn_t* conn, float vol){
    int func = 0; 
    int vset = 0; 
    uint8_t data[2] = {0};
    static int count = 0; 
    vset = (int)(fabs(vol) * 8.0F / 1.285F);
    if(vset >  63) vset = 63;
    if(vset <  6){ 
        vset = 0;
        func = 0;
    }
    if(vol < 0) func = 2;
    if(vol > 0) func = 1;
    data[1] = vset << 2 | func;
    count++; 
    return drv8830_writebytes(conn, data, 2);
}

int drv8830_readfault(drv8830_conn_t* conn){
    return read8(conn, DRV8830_FAULT_ADDR);
}
