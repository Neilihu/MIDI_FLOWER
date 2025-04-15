#include "CAPTOUCH.h"
#include "timers.h"
#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

#define num_of_avg 7

unsigned int store[num_of_avg] = {0, 0, 0, 0, 0, 0,0};
unsigned int count = 0;
unsigned int avg = 0;
unsigned int temp = 0;


void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    temp = IC4BUF;
    temp = temp & 0xFFFF;//It accdentially get T3
    
    TMR2 = 0x0;
    store[count] = temp;
    count++;
    if (count == num_of_avg) {
        count = 0;
    }
    for (int i = 0; i < num_of_avg; i++) {
        temp += store[i];
    }
    temp = temp / num_of_avg;
    avg = temp;
    temp = 0;
}

char CAPTOUCH_Init(void) {
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011; //4:1
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;

    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1; //timer 2
    IC4CONbits.ICM = 0b010; //every fall edge
    
    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    // whatever else you need to do to initialize your module
    return SUCCESS;
}

unsigned int CAPTOUCH_num(void) {
    return avg;
}


