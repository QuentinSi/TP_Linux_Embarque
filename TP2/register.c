#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>

#define GPIO_ADRR 0xFF203000

u_int32_t *p;

int main(void){
    int fd=open("/dev/mem",O_RDWR);
    p=(u_int32_t*)mmap(NULL,4,PROT_WRITE|PROT_READ, MAP_SHARED,fd,GPIO_ADRR);
    *p=(1<<8);
    printf("%d\r\n",p);
    return 0;
}

