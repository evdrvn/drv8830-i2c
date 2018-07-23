all: drv8830-i2c-example 

drv8830-i2c-example: ../src/drv8830-i2c.c src/main.c
	gcc -Wall -O2 -I../src ../src/drv8830-i2c.c src/main.c -lm -lpthread -o drv8830-i2c-example 

clean:
	rm -f drv8830-i2c-example
