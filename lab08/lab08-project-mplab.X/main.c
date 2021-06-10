/* 
 * File:   main.c
 * Author: carlomiras
 * 
 * SN: 2017-00076
 * Freq: 20 kHz
 * Duty: 76%
 *
 * Created on June 9, 2021, 10:41 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <pic32m_builtins.h>

#define SYS_FREQ 8000000L;

void setup_pins();
void setup_oc();

/*
 * 
 */
int main(int argc, char** argv) {
    setup_oc();
    setup_pins();
    
    while(1) {}
    
    return (EXIT_SUCCESS);
}

void setup_pins() {
    // unlock and modify sysreg
    __builtin_disable_interrupts();
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPCONbits.IOLOCK = 1;
    RPOR4bits.RP19R = 6;     // remap PWM signal to RP19
    SYSKEY = 0x00000000;
    __builtin_enable_interrupts();
    
    TRISCbits.TRISC9 = 0; // set RC9 as output
}

void setup_oc() {
    // Set SCCP operating mode
    CCP2CON1bits.CCSEL = 0;     // Set SCCP operating mode (OC mode)
    CCP2CON1bits.MOD = 0b0101;  // Set mode (Buffered Dual-Compare/PWM mode)
    
    // Configure SCCP Timebase
    CCP2CON1bits.T32 = 0;
    CCP2CON1bits.TMRSYNC = 1;
    CCP2CON1bits.CLKSEL = 0b000;
    CCP2CON1bits.TMRPS = 0b00;
    CCP2CON1bits.TRIGEN = 0;
    CCP2CON1bits.SYNC = 0b00000;
    
    // Configure SCCP output for PWM signal
    CCP2CON2bits.OCAEN = 1;
    CCP2CON3bits.POLACE = 0;
    CCP2TMRbits.TMRL = 0x0000;
    
    // SN: 2017-00076
    // Freq: 20kHz
    // Duty: 76%
    CCP2PR = 400;               // 8MHz / 20kHz
    CCP2RA = 0;                 // edge-aligned PWM
    CCP2RB = 304;               // 400 * 0.76
        
    CCP2CON1bits.ON = 1;
}

