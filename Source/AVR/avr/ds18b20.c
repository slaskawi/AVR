/**
 * This file is responsible for DALLAS
 * @author Michal Wachowicz
 */
#define DQ 0 

#define SET_DQ DDRA &= ~_BV(DQ) 
#define CLR_DQ DDRA |= _BV(DQ) 

#define IN_DQ PINA & _BV(DQ) 

short int x;

u08 m_currentTemperature = 0;
/**
 * Delay (counters)
 * @param x
 * 		Time to wait
 */
int dl(unsigned char x) {
    TCCR0 = 0x02;
    OCR0 = x;
    while (1) {
        if (TCNT0 >= OCR0)
            break;
    }
    TCCR0 = 0;
    TCNT0 = 0;
}

/**
 * Delaying function
 * @param x
 * 		Time to wait in ms
 */
void dlms(unsigned char x) {

    TCCR0 = 0x05;
    OCR0 = 8;
    for (; x > 0; x--) {
        TCNT0 = 0;
        while (1) {
            if (TCNT0 >= OCR0)
                break;
        }
    }
    TCCR0 = 0;
    TCNT0 = 0;

}

/**
 * One wire reset
 * @return
 * 		1 - if sensor is not detected or an error occured
 * 		0 - if everything is ok
 */
unsigned char ow_reset__(void) {
    unsigned char presence = 1;
    //pull DQ line DQ = 0
    CLR_DQ;

    // 450 us
    dl(250);
    dl(250);

    //DQ = 1; - return to output mode
    SET_DQ;

    //wait for Dallas pulse (line stabilisation)
    dl(55);

    //what Dallas has returned
    if (IN_DQ)
        presence = 1;
    else
        presence = 0;

    dl(200);
    dl(200);
    if (IN_DQ && (presence == 0)) {
        //OK
        presence = 0;
    } else {
        //Dallas not present
        presence = 1;
    }

    return (presence);
}

/**
 * reads a bit from the one-wire bus. The delay
 * required for a read is 15us, so the DELAY routine won't work.
 * We put our own delay function in this routine in the form of a
 * for() loop.
 * @return
 * 		bit
 */
unsigned char read_bit(void) {
    unsigned char presence = 0;
    //DQ = 0; start the time slot
    CLR_DQ;
    for (x = 0; x < 5; x++)
        ;
    asm ("nop");

    //DQ = 1; return high
    SET_DQ;
    //delay to start the timeslot
    dl(15);
    ;

    if (IN_DQ)
        presence = 1;
    else
        presence = 0;
    return (presence);
}

/**
 * writes a bit to the one-wire bus, passed in bitval.
 * @param bitval
 * 		value od the bit 
 */
void write_bit(char bitval) {
    //DQ = 0; start the timeslot
    CLR_DQ;
    for (x = 0; x < 10; x++)
        ;
    asm ("nop");
    // return DQ high if write 1
    if (bitval == 1)
        SET_DQ;
    //hold value for remainder of timeslot - delay 104us
    dl(100);
    //DQ = 1;
    SET_DQ;
}

/**
 * reads a byte from the one-wire bus 
 * @return
 * 		read byte
 */
unsigned char read_byte(void) {
    unsigned char i;
    unsigned char value = 0;
    for (i = 0; i < 8; i++) {
        //reads byte in, one byte at a time and then
        //shifts it left wait for rest of timeslot 120us
        if (read_bit())
            value |= 0x01 << i;
        dl(70);
    }
    return (value);
}

/**
 * writes a byte to the one-wire bus.
 * @param val
 * 		char value to be written
 */
void write_byte(char val) {
    unsigned char i;
    unsigned char temp;
    //writes byte, one bit at a time
    for (i = 0; i < 8; i++) {
        //shifts val right 'i' spaces
        temp = val >> i;
        //copy that bit to temp
        temp &= 0x01;
        //write bit in temp into
        write_bit(temp);
    }
    dl(50);
}

/**
 * Gets the temperature, writes it into UART and LCD
 */
void get_temperature(void) {
    char tmp[6];
    tmp[5] = 0;

    char temp1 = 0, temp2;
    short int t, u;

    ow_reset__();

    //Skip ROM
    write_byte(0xCC);
    //Start Conversion
    write_byte(0x44);

    dlms(200);
    dlms(200);
    dlms(200);
    dlms(200);
    dlms(200);

    ow_reset__();
    //Skip ROM
    write_byte(0xCC);
    //Read Scratch Pad
    write_byte(0xBE);

    temp1 = read_byte();
    temp2 = read_byte();

    t = (temp2 << 4) | (temp1 >> 4);
    u = temp1 & 0x0F;
    if (t < 0) {
        t = ~t;
        if (u == 0)
            t++;
        else {
            short int k = 0;
            while ((u & _BV(k)) == 0) {
                k++;
            }

            char q = 0xFF;
            u = ~u;
            u &= (q << k);
            u |= _BV(k);
            u &= 0x0F;
        }
    }

    tmp[0] = t / 100 + 48;
    tmp[1] = ((t / 10) - (10 * (t / 100))) + 48;
    tmp[2] = t % 10 + 48;
    tmp[3] = '.';
    tmp[4] = (u * 10 / 16) + 48;

    if (temp2 < 0) {
        tmp[0] = '-';
        if (tmp[1] == 48) {
            for (x = 1; x < 5; x++)
                tmp[x] = tmp[x + 1];
        }
    } else {
        if (tmp[0] == 48) {
            short int d = 0;
            if (tmp[1] == 48)
                d = 1;
            for (x = 0; x < (6 - d); x++)
                tmp[x] = tmp[x + 1 + d];
        }
    }

    //write it to LCD and UART
    char *wsk = &tmp;
    LCD_gotoxy(6, 2);
    LCD_puts(wsk);
    LCD_putc(176);
    LCD_putc('C');
    UART_puts(wsk);
}

void get_temperatureSilent() {
//    char tmp[6];
//    tmp[5] = 0;

    char temp1 = 0, temp2;
    short int t, u;

    ow_reset__();
    ow_reset__();

    //Skip ROM
    write_byte(0xCC);
    //Start Conversion
    write_byte(0x44);

    ow_reset__();
    //Skip ROM
    write_byte(0xCC);
    //Read Scratch Pad
    write_byte(0xBE);

    temp1 = read_byte();
    temp2 = read_byte();

    t = (temp2 << 4) | (temp1 >> 4);
    u = temp1 & 0x0F;
    if (t < 0) {
        t = ~t;
        if (u == 0)
            t++;
        else {
            short int k = 0;
            while ((u & _BV(k)) == 0) {
                k++;
            }

            char q = 0xFF;
            u = ~u;
            u &= (q << k);
            u |= _BV(k);
            u &= 0x0F;
        }
    }

    m_currentTemperature = t;
}
