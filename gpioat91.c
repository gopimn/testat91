/**
 * @file gpioAt91.c
 * @author Felipe Martínez
 * @agency Wisely.cl
 * This file contains GPIO functions using high
 * performance mmap of /dev/mem
 *
 * Especifically for the AT91sam9261 microcontroller
 *
 * Licensed under the GPL 3.0 Licence
 * 
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#define BLOCK_SIZE 4096UL
#define MAP_MASK (BLOCK_SIZE - 1)
#define GPIO_A_BASE 0xfffff400

static void *PIO_PER;   /* 0x0000 */
static void *PIO_OER;   /* 0x0010 */
static void *PIO_SODR;  /* 0x0030 */
static void *PIO_CODR;  /* 0x0034 */

#define GPIO_IN(pin) *(unsigned int *) PIO_PER = 1 << pin
#define GPIO_OUT(pin) *(unsigned int *) PIO_OER = 1 << pin

#define GPIO_SET(pin) *(unsigned int *) PIO_SODR = 1 << pin
#define GPIO_CLR(pin) *(unsigned int *) PIO_CODR = 1 << pin

#define PIN_SCS 24

int gpio_init() {
    int mem_fd;
    void *gpio_map;

    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        fprintf(stderr, "Cannot open /dev/mem\n");
        return -1;
    }

    gpio_map = mmap(
        0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
        GPIO_A_BASE & ~MAP_MASK
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
      fprintf(stderr, "mmap error %d\n",gpio_map);
        return -1;
    }

    PIO_PER = gpio_map + (GPIO_A_BASE & MAP_MASK);
    PIO_OER = gpio_map + ((GPIO_A_BASE + 0x10) & MAP_MASK);
    PIO_SODR = gpio_map + ((GPIO_A_BASE + 0x30) & MAP_MASK);
    PIO_CODR = gpio_map + ((GPIO_A_BASE + 0x34) & MAP_MASK);

    return 0;
}

int main ()
{
  
 gpio_init();
 GPIO_OUT(PIN_SCS);
 usleep(50);
 while (1)
   {
   
   GPIO_SET(PIN_SCS);
   sleep(1);
   GPIO_CLR(PIN_SCS);
   sleep(1);
   }
}
