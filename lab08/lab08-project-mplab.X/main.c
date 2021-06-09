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

void delay_ms(int ms) {
    int i = 0;
    int delay = 1143 * ms;
    while (i < delay) {
         ++i; // 875ns delay
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    setup_pins();
    setup_oc();
    
    // testing
    int out = 0;
    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB0 = 0;
    
    while(1) {
        delay_ms(5);
        LATBbits.LATB0 = out;
        out = !out;
    }
    
    return (EXIT_SUCCESS);
}

void setup_pins() {
    // unlock and modify sysreg
    __builtin_disable_interrupts();
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPCONbits.IOLOCK = 1;
    RPOR2bits.RP9R = 6;
    SYSKEY = 0x00000000;
    __builtin_enable_interrupts();
    
    // remap PWM signal to RP9
}

void setup_oc() {
    // Set SCCP operating mode
    CCP1CON1bits.CCSEL = 0;     // Set SCCP operating mode (OC mode)
    CCP1CON1bits.MOD = 0b0101;  // Set mode (Buffered Dual-Compare/PWM mode)
    
    // Configure SCCP Timebase
    CCP1CON1bits.T32 = 0;
    CCP1CON1bits.TMRSYNC = 0;
    CCP1CON1bits.CLKSEL = 0b000;
    CCP1CON1bits.TMRPS = 0b00;
    CCP1CON1bits.TRIGEN = 0;
    CCP1CON1bits.SYNC = 0b00000;
    
    // Configure MCCP output for PWM signal
    CCP1CON2bits.OCAEN = 1;
    CCP1CON3bits.OUTM = 0b000;
    CCP1CON3bits.POLACE = 0;
    CCP1TMRbits.TMRL = 0x0000;
    
    // SN: 2017-00076
    // Freq: 20kHz
    // Duty: 76%
    CCP1PRbits.PRL = 400;               // 8MHz / 20kHz
    CCP1RA = 0;                 // edge-aligned PWM
    CCP1RB = 304;               // 400 * 0.76
        
    CCP1CON1bits.ON = 1;
}

//void setup_oc() {
//    // Set SCCP operating mode
//    CCP2CON1bits.CCSEL = 0;     // Set SCCP operating mode (OC mode)
//    CCP2CON1bits.MOD = 0b0101;  // Set mode (Buffered Dual-Compare/PWM mode)
//    
//    // Configure SCCP Timebase
//    CCP2CON1bits.T32 = 0;
//    CCP2CON1bits.TMRSYNC = 0;
//    CCP2CON1bits.CLKSEL = 0b000;
//    CCP2CON1bits.TMRPS = 0b00;
//    CCP2CON1bits.TRIGEN = 0;
//    CCP2CON1bits.SYNC = 0b00000;
//    
//    // Configure MCCP output for PWM signal
//    CCP2CON2bits.OCAEN = 1;
//    CCP2CON3bits.POLACE = 0;
//    CCP2TMRbits.TMRL = 0x0000;
//    
//    // SN: 2017-00076
//    // Freq: 20kHz
//    // Duty: 76%
//    CCP2PR = 400;               // 8MHz / 20kHz
//    CCP2RA = 0;                 // edge-aligned PWM
//    CCP2RB = 304;               // 400 * 0.76
//        
//    CCP2CON1bits.ON = 1;
//}

