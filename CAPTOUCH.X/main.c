/* 
 * File:   main.c
 * Author: nehu
 *
 * Created on April 18, 2023, 11:30 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "CAPTOUCH.h"
#include "serial.h"
#include "timers.h"
#include "AD.h"

#define num_of_avg 5

/*
 * 
 */
unsigned int store[num_of_avg] = {0, 0, 0,0,0};
int count = 0, count2 = 0;
int tem = 0;
void all_init(void);//initialize everything


int main(int argc, char** argv) {
    all_init();
    printf("hello\n");
    while (1) {
        store[count] = CAPTOUCH_IsTouched();//directly get reading from ISR
        count++;
        if(count == num_of_avg){
            count = 0;
        }
        for(int i = 0;i<num_of_avg; i++ ){
            tem += store[i];
        }
        tem = tem/num_of_avg;
        
        printf("%d\n",tem);
        tem = 0;
        for (int i = 0; i < 30000; i++);//delay

    }
    return (EXIT_SUCCESS);
}

void all_init(void) {
    BOARD_Init();
    TIMERS_Init();
    SERIAL_Init();
    CAPTOUCH_Init();
    AD_Init();
}
