/* sd2iec - SD/MMC to Commodore serial bus interface/controller
   Copyright (C) 2007,2008  Ingo Korb <ingo@akana.de>

   Inspiration and low-level SD/MMC access based on code from MMC2IEC
     by Lars Pontoppidan et al., see sdcard.c|h and config.h.

   FAT filesystem access based on code from ChaN, see tff.c|h.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License only.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


   config.h: User-configurable options to simplify hardware changes and/or
             reduce the code/ram requirements of the code.


   Based on MMC2IEC, original copyright header follows:

//
// Title        : MMC2IEC - Configuration
// Author       : Lars Pontoppidan
// Date         : Jan. 2007
// Version      : 0.7
// Target MCU   : AtMega32(L) at 8 MHz
//
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt
//

*/

#ifndef CONFIG_H
#define CONFIG_H

#include "autoconf.h"

#if CONFIG_HARDWARE_VARIANT==1
/* Configure for your own hardware                     */
/* Example values are for the "Shadowolf 1.x" variant. */

/*** SD card signals ***/
/* CARD_DETECT must return non-zero when card is inserted */
/* This must be a pin capable of generating interrupts.   */
#  define SDCARD_DETECT         (!(PIND & _BV(PD2)))
#  define SDCARD_DETECT_SETUP() do { DDRD &= ~_BV(PD2); PORTD |= _BV(PD2); } while(0)

/* Set up the card detect pin to generate an interrupt on every change */
#  if defined __AVR_ATmega32__
#    define SD_CHANGE_SETUP()  do { MCUCR |= _BV(ISC00); GICR |= _BV(INT0); } while(0)
#  elif defined __AVR_ATmega644__
#    define SD_CHANGE_SETUP()  do { EICRA |= _BV(ISC00); EIMSK |= _BV(INT0); } while(0)
#  else
#    error Unknown chip!
#  endif

/* Name of the interrupt of the card detect pin */
#  define SD_CHANGE_ISR INT0_vect

/* CARD Write Protect must return non-zero when card is write protected */
#  define SDCARD_WP         (PIND & _BV(PD6))
#  define SDCARD_WP_SETUP() do { DDRD &= ~ _BV(PD6); PORTD |= _BV(PD6); } while(0)

/* SD Card supply voltage - choose the one appropiate to your board */
/* #  define SD_SUPPLY_VOLTAGE (1L<<15)  / * 2.7V - 2.8V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<16)  / * 2.8V - 2.9V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<17)  / * 2.9V - 3.0V */
#  define SD_SUPPLY_VOLTAGE (1L<<18)  /* 3.0V - 3.1V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<19)  / * 3.1V - 3.2V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<20)  / * 3.2V - 3.3V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<21)  / * 3.3V - 3.4V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<22)  / * 3.4V - 3.5V */
/* #  define SD_SUPPLY_VOLTAGE (1L<<23)  / * 3.5V - 3.6V */


/*** Device address selection ***/
/* DEVICE_SELECT should return the selected device number,   */
/* DEVICE_SELECT_SETUP() is called once to set up the ports. */
#  define DEVICE_SELECT       (8+!(PIND & _BV(PD7))+2*!(PIND & _BV(PD5)))
#  define DEVICE_SELECT_SETUP() do { \
      DDRC  &= ~(_BV(PD7)|_BV(PD5)); \
      PORTD |=   _BV(PD7)|_BV(PD5);  \
   } while (0)


/*** LEDs ***/
/* BUSY led, recommended color: green */
/* R.Riedel - using PORTC instead of the original PORTA here plus inverse polarity */
#  define BUSY_LED_SETDDR() DDRC  |= _BV(PC0)
#  define BUSY_LED_ON()     PORTC |= _BV(PC0)
#  define BUSY_LED_OFF()    PORTC &= ~_BV(PC0)

/* DIRTY led, recommended color: red */
/* R.Riedel - using PORTC instead of the original PORTA here plus inverse polarity */
#  define DIRTY_LED_SETDDR() DDRC  |= _BV(PC1)
#  define DIRTY_LED_ON()     PORTC |= _BV(PC1)
#  define DIRTY_LED_OFF()    PORTC &= ~_BV(PC1)
#  define DIRTY_LED_PORT     PORTC
#  define DIRTY_LED_BIT()    _BV(PC1)

/* Auxiliary LED for debugging */
#  define AUX_LED_SETDDR()   DDRC  |= _BV(PC2)
#  define AUX_LED_ON()       PORTC |= _BV(PC2)
#  define AUX_LED_OFF()      PORTC &= ~_BV(PC2)


/*** IEC signals ***/
/* R.Riedel - using PORTA instead of the original PORTC for the IEC */

#  define IEC_PIN  PINA
#  define IEC_DDR  DDRA
#  define IEC_PORT PORTA

/* Please note that you'll have to change the assembler modules if these */
/* pins aren't defined as Px0-Px3 in this order.                         */
#  define IEC_PIN_ATN   PA0
#  define IEC_PIN_DATA  PA1
#  define IEC_PIN_CLOCK PA2
#  define IEC_PIN_SRQ   PA3


/*** User interface ***/
/* Disk image change key */
#  define DISKCHANGE_PIN  PINC
#  define DISKCHANGE_DDR  DDRC
#  define DISKCHANGE_PORT PORTC
#  define DISKCHANGE_BIT  _BV(PC4)
/* Target value of the debounce counter */
#  define DISKCHANGE_MAX  128



/* Pre-configurated hardware variants */

#elif CONFIG_HARDWARE_VARIANT==2
/* Hardware configuration: Shadowolf 1 */
#  define SDCARD_DETECT         (!(PIND & _BV(PD2)))
#  define SDCARD_DETECT_SETUP() do { DDRD &= ~_BV(PD2); PORTD |= _BV(PD2); } while(0)
#  if defined __AVR_ATmega32__
#    define SD_CHANGE_SETUP()   do { MCUCR |= _BV(ISC00); GICR |= _BV(INT0); } while(0)
#  elif defined __AVR_ATmega644__
#    define SD_CHANGE_SETUP()   do { EICRA |= _BV(ISC00); EIMSK |= _BV(INT0); } while(0)
#  else
#    error Unknown chip!
#  endif
#  define SD_CHANGE_ISR         INT0_vect
#  define SDCARD_WP             (PIND & _BV(PD6))
#  define SDCARD_WP_SETUP()     do { DDRD &= ~ _BV(PD6); PORTD |= _BV(PD6); } while(0)
#  define SD_SUPPLY_VOLTAGE     (1L<<18)
#  define DEVICE_SELECT         (8+!(PIND & _BV(PD7))+2*!(PIND & _BV(PD5)))
#  define DEVICE_SELECT_SETUP() do {        \
             DDRC  &= ~(_BV(PD7)|_BV(PD5)); \
             PORTD |=   _BV(PD7)|_BV(PD5);  \
          } while (0)
#  define BUSY_LED_SETDDR()     DDRC  |= _BV(PC0)
#  define BUSY_LED_ON()         PORTC |= _BV(PC0)
#  define BUSY_LED_OFF()        PORTC &= ~_BV(PC0)
#  define DIRTY_LED_SETDDR()    DDRC  |= _BV(PC1)
#  define DIRTY_LED_ON()        PORTC |= _BV(PC1)
#  define DIRTY_LED_OFF()       PORTC &= ~_BV(PC1)
#  define DIRTY_LED_PORT        PORTC
#  define DIRTY_LED_BIT()       _BV(PC1)
#  define AUX_LED_SETDDR()      DDRC  |= _BV(PC2)
#  define AUX_LED_ON()          PORTC |= _BV(PC2)
#  define AUX_LED_OFF()         PORTC &= ~_BV(PC2)
#  define IEC_PIN               PINA
#  define IEC_DDR               DDRA
#  define IEC_PORT              PORTA
#  define IEC_PIN_ATN           PA0
#  define IEC_PIN_DATA          PA1
#  define IEC_PIN_CLOCK         PA2
#  define IEC_PIN_SRQ           PA3
#  define DISKCHANGE_PIN        PINC
#  define DISKCHANGE_DDR        DDRC
#  define DISKCHANGE_PORT       PORTC
#  define DISKCHANGE_BIT        _BV(PC4)
#  define DISKCHANGE_MAX        128

#elif CONFIG_HARDWARE_VARIANT == 3
/* Hardware configuration: LarsP */
#  define SDCARD_DETECT         (!(PIND & _BV(PD2)))
#  define SDCARD_DETECT_SETUP() do { DDRD &= ~_BV(PD2); PORTD |= _BV(PD2); } while(0)
#  define SD_CHANGE_SETUP()     do { MCUCR |= _BV(ISC00); GICR |= _BV(INT0); } while(0)
#  define SD_CHANGE_ISR         INT0_vect
#  define SDCARD_WP             (PIND & _BV(PD6))
#  define SDCARD_WP_SETUP()     do { DDRD &= ~ _BV(PD6); PORTD |= _BV(PD6); } while(0)
#  define SD_CHANGE_ICR         MCUCR
#  define SD_SUPPLY_VOLTAGE     (1L<<21)
#  define DEVICE_SELECT         (8+!(PIND & _BV(PA2))+2*!(PIND & _BV(PA3)))
#  define DEVICE_SELECT_SETUP() do {        \
             DDRC  &= ~(_BV(PA2)|_BV(PA3)); \
             PORTD |=   _BV(PA2)|_BV(PA3);  \
          } while (0)
#  define BUSY_LED_SETDDR()     DDRA  |= _BV(PA0)
#  define BUSY_LED_ON()         PORTA &= ~_BV(PA0)
#  define BUSY_LED_OFF()        PORTA |= _BV(PA0)
#  define DIRTY_LED_SETDDR()    DDRA  |= _BV(PA1)
#  define DIRTY_LED_ON()        PORTA &= ~_BV(PA1)
#  define DIRTY_LED_OFF()       PORTA |= _BV(PA1)
#  define DIRTY_LED_PORT        PORTA
#  define DIRTY_LED_BIT()       _BV(PA1)
#  define AUX_LED_SETDDR()      do {} while (0)
#  define AUX_LED_ON()          do {} while (0)
#  define AUX_LED_OFF()         do {} while (0)
#  define IEC_PIN               PINC
#  define IEC_DDR               DDRC
#  define IEC_PORT              PORTC
#  define IEC_PIN_ATN           PC0
#  define IEC_PIN_DATA          PC1
#  define IEC_PIN_CLOCK         PC2
#  define IEC_PIN_SRQ           PC3
#  define DISKCHANGE_PIN        PINA
#  define DISKCHANGE_DDR        DDRA
#  define DISKCHANGE_PORT       PORTA
#  define DISKCHANGE_BIT        _BV(PA4)
#  define DISKCHANGE_MAX        128

#elif CONFIG_HARDWARE_VARIANT == 4
/* Hardware configuration: uIEC (incomplete) */
/* Faked SD definition so the code can at lease be compile-tested */
#  define SDCARD_DETECT         1
#  define SDCARD_DETECT_SETUP() do {} while (0)
#  define SD_CHANGE_SETUP()     do {} while (0)
#  define SD_CHANGE_ISR         INT0_vect
#  define SDCARD_WP             0
#  define SDCARD_WP_SETUP()     do {} while (0)
#  define SD_SUPPLY_VOLTAGE     (1L<<21)
/* No device jumpers on uIEC */
#  define DEVICE_SELECT         10
#  define DEVICE_SELECT_SETUP() do {} while (0)
#  define BUSY_LED_SETDDR()     DDRG  |= _BV(PG4)
#  define BUSY_LED_ON()         PORTG |= _BV(PG4)
#  define BUSY_LED_OFF()        PORTG &= ~_BV(PG4)
#  define DIRTY_LED_SETDDR()    DDRG  |= _BV(PG3)
#  define DIRTY_LED_ON()        PORTG |= _BV(PG3)
#  define DIRTY_LED_OFF()       PORTG &= ~_BV(PG3)
#  define DIRTY_LED_PORT        PORTG
#  define DIRTY_LED_BIT()       _BV(PG3)
#  define AUX_LED_SETDDR()      DDRG  |= _BV(PG2)
#  define AUX_LED_ON()          PORTG |= _BV(PG2)
#  define AUX_LED_OFF()         PORTG &= ~_BV(PG2)
#  define IEC_PIN               PINE
#  define IEC_DDR               DDRE
#  define IEC_PORT              PORTE
/* JLB - I'm debating whether to just punt and rearrange the wires or rewrite the code to compensate */
#  define IEC_PIN_ATN           PE6
#  define IEC_PIN_DATA          PE4
#  define IEC_PIN_CLOCK         PE5
#  define IEC_PIN_SRQ           PE2
/* This should really be on a INT pin, but I need to find one.  Use G1 for now. */
#  define DISKCHANGE_PIN        PING
#  define DISKCHANGE_DDR        DDRG
#  define DISKCHANGE_PORT       PORTG
#  define DISKCHANGE_BIT        _BV(PG1)
#  define DISKCHANGE_MAX        128

#else
#  error "CONFIG_HARDWARE_VARIANT is unset or set to an unknown value."
#endif

#define IEC_BIT_ATN   _BV(IEC_PIN_ATN)
#define IEC_BIT_DATA  _BV(IEC_PIN_DATA)
#define IEC_BIT_CLOCK _BV(IEC_PIN_CLOCK)
#define IEC_BIT_SRQ   _BV(IEC_PIN_SRQ)


/* ---------------- End of user-configurable options ---------------- */

/* Disable COMMAND_CHANNEL_DUMP if UART_DEBUG is disabled */
#ifndef CONFIG_UART_DEBUG
#  undef CONFIG_COMMAND_CHANNEL_DUMP
#endif

#endif
