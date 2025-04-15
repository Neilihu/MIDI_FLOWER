/* 
 * File:   plant.c
 * Author: siton
 *
 * Created on May 30, 2023, 6:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "AD.h"
#include "serial.h"
#include "BOARD.h"
#include "ttfaf.h"
#include "ToneGeneration.h"


#define AD_A0 (1<<0) // NOte that this is also the pot on board
#define AD_A1 (1<<1)
#define AD_A2 (1<<2)
#define AD_A3 (1<<3)
#define AD_A4 (1<<4)
#define AD_A5 (1<<5)
/*
 * 
 */
int main(int argc, char** argv) {

    BOARD_Init();
    AD_Init();
    AD_AddPins(AD_A1);
    AD_AddPins(AD_A2);
    ToneGeneration_Init();
    
    int * tones = loadTones();
    int * durations = loadDurations();
    const int songEnd = sizeof(tones)/sizeof(int);
    
    int flex=0;
    int captouch=0;
    int delay=0;//Current plan: Increment delay every loop, if it's equal to the current slot in durations, go to the next tone.
    //Loop at the end.
    int noteNum=0;
    ToneGeneration_SetFrequency(tones[0]);
    
    while (1){
        flex = AD_ReadADPin(AD_A1); //red wire
        //Pin AD1. Floats around 260 when at rest, increases to about 460 at most. 

        captouch = AD_ReadADPin(AD_A2); //black wire
        //Pin AD2. So noisy that the signal gets lost.
        if (captouch !=0 && captouch !=1023)
            printf("%i\n", captouch);
        int vol = (flex-260) * 255 / 200; //Send this to speaker output
        
        
        delay++;
        if(delay >= durations[noteNum]){ //End of note
            delay = 0;
            noteNum++;
        }
        if(delay == 0){ // Start next note. Should only trigger if we went through the last if statement.
            if(noteNum>=songEnd) noteNum=0; //End of song, start from top
            ToneGeneration_SetFrequency(tones[noteNum]);
        }
    }
    
    return (EXIT_SUCCESS);
}

