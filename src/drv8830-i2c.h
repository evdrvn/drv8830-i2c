#ifdef __DRV8830_I2C_h__

#else

#define __DRV8830_I2C_h__

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define DRV8830_ADDRESS (0x60)

typedef enum
{
    DRV8830_CONTROL_ADDR      = 0x00,
    DRV8830_FAULT_ADDR        = 0x01
} drv8830_reg_t;

typedef struct
{
    int file;
    uint8_t address;
} drv8830_conn_t;

extern int drv8830_open(drv8830_conn_t* conn, const char* i2cdev, uint8_t address, int timeout_10ms);
extern int drv8830_move(drv8830_conn_t* conn, int8_t target);
extern int drv8830_readfault(drv8830_conn_t* conn);

#endif
