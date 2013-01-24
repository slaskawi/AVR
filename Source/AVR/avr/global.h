/**
 * Global config file. Defines ports, speeds etc.
 * @author Sebastian Laskawiec and Michal Wachowicz
 */
 
 /*
  * SDA, SCL bus
  */
#define I2C_PORT	PORTC
#define I2C_SDA		1
#define I2C_SCL 	0
 
/**
 * A0 address for PCF
 */
#ifndef PCF8583_A0
#define PCF8583_A0	1
#endif
 
 
#ifndef GLOBAL_H
#define GLOBAL_H

/**
 * Clock speed
 */
#ifndef F_CPU
#define F_CPU		16000000
#endif


/**
 * UART speed
 */
#ifndef UART_BAUD
#define UART_BAUD	19200
#endif


/**
 * UART boudrate calculations
 */
#ifndef UART_CONST
#define UART_CONST	(F_CPU/(UART_BAUD*16L)-1)
#endif

/**
 * UART buffer size
 */
#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 32
#endif

/**
 * Watchdog settings
 */
#ifdef WDT_ENABLE
#define WDR()	wdt_reset()
#else 
#define WDR()
#endif

/**
 * Assembler nop definition.
 */
#define NOP() __asm__ __volatile__ ("nop")


/**
 * Number of cycles per sec
 */
#ifndef CYCLES_PER_US
#define CYCLES_PER_US ((F_CPU+500000)/1000000)
#endif

/**
 * GCC version
 */
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif


/**
 * Deprecated sbi, cbi settings
 */
#ifndef cbi
#define cbi(PORT, BIT) (_SFR_BYTE(PORT) &= ~_BV(BIT))
#endif

#ifndef sbi
#define sbi(PORT, BIT) (_SFR_BYTE(PORT) |= _BV(BIT))
#endif

/**
 * Toggle bit
 */
#ifndef tbi
#define tbi(PORT, BIT)	(_SFR_BYTE(PORT) ^= _BV(BIT))
#endif

/**
 * Compatibile properties
 */
#ifndef inp
#define inp(PORT)	_SFR_BYTE(PORT)
#endif

#ifndef outp
#define outp(VAL, PORT)	(_SFR_BYTE(PORT) = (VAL))
#endif

#ifndef outw
#define outw(VAL, PORT)	(_SFR_WORD(PORT) = (VAL))
#endif

#ifndef BV
#define BV _BV
#endif

#ifndef PRG_RDB
#define PRG_RDB	pgm_read_byte
#endif

#ifndef eeprom_rb
#define eeprom_rb	eeprom_read_byte
#endif

#ifndef eeprom_wb
#define eeprom_wb	eeprom_write_byte
#endif


/**
 * Address direction register (input)
 */
#define DDR(x) (_SFR_IO8(_SFR_IO_ADDR(x)-1))

/**
 * Address direction register (output)
 */
#define PIN(x) (_SFR_IO8(_SFR_IO_ADDR(x)-2))


/**
 * EEPROM defines
 */
#define EEPROM __attribute__((section(".eeprom")))

#define ESTR(s) ({static char __c[] EEPROM = (s); __c;})

/**
 * Logical true, false
 */
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

/**
 * Register names changes
 */
//_UBRR_
#ifdef	UBRR
#define _UBRR_	UBRR
#endif


#ifdef	UBRRL
#define _UBRR_	UBRRL
#endif


#ifdef	UBRR0
#define _UBRR_	UBRR0
#endif


#ifdef	UBRR0L
#define _UBRR_	UBRR0L
#endif

// _UBRRH_
#ifdef	UBRRH
#define _UBRRH_	UBRRH
#endif

#ifdef	UBRR0H
#define _UBRRH_	UBRR0H
#endif

#ifdef	UBRRHI
#define _UBRRH_	UBRRHI
#endif

// _UDR_
#ifdef	UDR
#define _UDR_	UDR
#endif


#ifdef	UDR0
#define _UDR_	UDR0
#endif

// _UCR_
#ifdef	UCR
#define _UCR_	UCR
#endif


#ifdef	UCSRB
#define _UCR_	UCSRB
#endif


#ifdef	UCSR0B
#define _UCR_	UCSR0B
#endif

// _USR_
#ifdef	USR
#define _USR_	USR
#endif


#ifdef	UCSRA
#define _USR_	UCSRA
#endif


#ifdef	UCSR0A
#define _USR_	UCSR0A
#endif

/**
 * UART settings
 */
#ifdef SIG_UART0_RECV
#define SIG_UART_RECV          SIG_UART0_RECV
#endif


#ifdef SIG_UART0_DATA
#define SIG_UART_DATA          SIG_UART0_DATA
#endif


#ifdef SIG_UART0_TRANS
#define SIG_UART_TRANS         SIG_UART0_TRANS
#endif


#ifndef __ASSEMBLER__

/**
 * Definition of different types
 * u08 - unsigned 8 bits
 * s08 - signed 8 bits
 * ...
 */
typedef unsigned 	char      	u08;
typedef          	char      	s08;
typedef unsigned 	short   	u16;
typedef          	short     	s16;
typedef unsigned 	long      	u32;
typedef          	long      	s32;
typedef unsigned 	long long 	u64;
typedef          	long long 	s64;

/**
 * Definition of types
 * UCHAR - unsigned 8 bits
 * WORD - unsigned 16 bits
 * DWORD - unsigned 32 bits
 * LPCTSTR - String pointer
 */
typedef unsigned char      	UCHAR;
typedef unsigned short     	WORD;
typedef unsigned long      	DWORD;
typedef char*		     	LPCTSTR;		

/**
 * Time handling type
 */
typedef struct	
{
	u08 second;
	u08 minute;
	u08 hour;
	u08 day;
	u08 month;
	u16 year;
} struct_time;      

/**
 * 16 bits word storing type
 */
struct u16bytes
{
	u08 low, high;
};

/**
 * 16 bits word Conversion union
 */
union u16convert
{
	u16 value;
	struct u16bytes bytes;
};


#endif

/**
 * Remote controller key defines
 */
#define KEY_POWER	16383
#define KEY_LEFT	16367
#define KEY_RIGHT	16351
#define KEY_ENTER	16183
#define KEY_RESET	16373
#define KEY_TEMPERATURE	16173
#define KEY_TIMER	16333
#define KEY_OPTIONS	16205
#define KEY_1	16223
#define KEY_2	16287
#define KEY_3	16159
#define KEY_4	16239
#define KEY_5	16303
#define KEY_6	16175
#define KEY_7	16207
#define KEY_8	16271
#define KEY_9	16143
#define KEY_0	16311

/**
 * PC Commander indexes
 */
#define PC_CONNECT			1 
#define PC_DISCONNECT		2 
#define PC_POWER			3 
#define PC_TIMER			4
#define	PC_DATE				5 
#define PC_TEMPERATURE		6 
#define PC_UART				7
#define PC_EXIT				8
#define PC_TIME_SET			9
#define PC_DATE_SET			10
#define PC_ALARM_ON			11
#define PC_ALARM_OFF		12
#define PC_ALARM_SET		13
#define PC_CONSOLE			14
#define PC_DATE_TIME 		15
#define PC_SETTINGS 		16
#define PC_TEMP_SET_UP 		17
#define PC_TEMP_TIME_SET_UP 		18
#define PC_WORK_ON   19
#define PC_WORK_OFF  20

/**
 * Global variables used in program
 */
u08 m_nightHour = 0;
u08 m_nightLast = 0;
u08 m_dayTemp = 0;
u08 m_nightTemp = 0;
u08 m_offsetTemp = 0;

/**
 * Settings
 */
u08 m_requiresSettings = 0;
u08 SETTING_TEMP = 1;
u08 SETTING_TIME = 2;
u08 SETTING_DATE = 4;
u08 SETTING_ALL = 7;

u08 m_work = 0;

#endif

