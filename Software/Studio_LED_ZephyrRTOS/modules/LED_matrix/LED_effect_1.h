#ifndef LED_EFFECT_1_H
#define LED_EFFECT_1_H

#include "LED_matrix.h"

#define FONT5X8_WIDHT       3
#define FONT5X8_HEIGHT      5

const uint8_t font5x8[11][5] = {
    // '0'
    { 0b111,   // ###
      0b101,   // # #
      0b101,   // # #
      0b101,   // # #
      0b111 }, // ###

    // '1'
    { 0b010,   //  #
      0b110,   // ##
      0b010,   //  #
      0b010,   //  #
      0b111 }, // ###

    // '2'
    { 0b111,   // ###
      0b001,   //   #
      0b111,   // ###
      0b100,   // #  
      0b111 }, // ###

    // '3'
    { 0b111,   // ###
      0b001,   //   #
      0b111,   // ###
      0b001,   //   #
      0b111 }, // ###

    // '4'
    { 0b101,   // # #
      0b101,   // # #
      0b111,   // ###
      0b001,   //   #
      0b001 }, //   #

    // '5'
    { 0b111,   // ###
      0b100,   // #  
      0b111,   // ###
      0b001,   //   #
      0b111 }, // ###

    // '6'
    { 0b111,   // ###
      0b100,   // #  
      0b111,   // ###
      0b101,   // # #
      0b111 }, // ###

    // '7'
    { 0b111,   // ###
      0b001,   //   #
      0b001,   //   #
      0b001,   //   #
      0b001 }, //   #

    // '8'
    { 0b111,   // ###
      0b101,   // # #
      0b111,   // ###
      0b101,   // # #
      0b111 }, // ###

    // '9'
    { 0b111,   // ###
      0b101,   // # #
      0b111,   // ###
      0b001,   //   #
      0b111 }, // ###

    // '.'
    { 0b000,   //   
      0b000,   //  
      0b000,   //    
      0b000,   //   
      0b100 }  // #
};


class effect_1_clock
{
    public:
        effect_1_clock(LED_matrix &matrix); // Constructor accepts reference
        void run(uint8_t hr, uint8_t min, uint8_t sec);

    private:
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
