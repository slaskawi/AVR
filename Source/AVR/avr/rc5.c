#define REMOTE_PORT	PORTA	///< Port, do którego jest pod³¹czone wyjœcie odbiornika IR
#define REMOTE_BIT	PD1	///< Bit portu REMOTE_PORT, do którego jest pod³¹czone wyjœcie odbiornika IR
#define REMOTE_STD	2

/* Standard kodowania
 1 : Format REC-80 u¿ywany przez firmê Panasonic
 2 : Format NEC
 3 : Format SONY (12 bitowy)
 4 : Format SONY (15 bitowy)
 5 : Format RC-5 u¿ywany przez firmê Philips
 */

int REMOTE_active(void) {
    register u08 i = 50;
    while (i--)
        if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
            return 1;
    return 0;
}

u16 REMOTE_check(u08 std) {
    u08 i, T2, T4, time, tmp = 0;
    union u16convert code;

    code.value = 0;
    TCCR0 = 4;        //update every 32us
    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal

    if (std < 2) {
        TCNT0 = 1;

        while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT)) {
            T2 = TCNT0;
            if (T2 >= 140)        // max wait time
                return 0;
        }

        // measure time T

        TCNT0 = 1;
        loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);

        T2 = TCNT0;        // T is normally around 0E-10 hex = 15 -> 480 uS
        T2 = T2 * 2;
        // max time is 4T
        T4 = T2 * 2;

        for (i = 0; i < ((std == 0) ? 48 : 32); i++) {
            TCNT0 = 1;
            while (1) {
                time = TCNT0;
                if (time > T4)
                    return 0;

                // measure time on the lo flank
                if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT)) {
                    tmp <<= 1;
                    if (time >= T2)
                        tmp++;
                    break;
                }
            }

            // save command data as we go
            if (std == 0) {
                if (i == 39)
                    code.bytes.low = tmp;
                if (i == 47)
                    code.bytes.high = tmp;
            } else {
                if (i == 15)
                    code.bytes.high = tmp;
                if (i == 31)
                    code.bytes.low = tmp;
            }

            // syncronize - wait for next hi flank
            loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
        }
        return (code.value);
    } else if (std < 4)          // Format used by SONY remote controllers
            {
        if (TCNT0 <= 60)
            return 0;        // is it the leading Signal ?

        // now looking for the Data-bits
        for (i = 0; i < ((std == 3) ? 11 : 14); i++) {
            tmp = 0x01;
            while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT)) {
                T2 = TCNT0;
                if (T2 >= 140)      // max wait time
                    return 0;
            }
            TCNT0 = 1;
            while (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT)) {
                T2 = TCNT0;
                if (T2 >= 140)      // max wait time
                    return 0;
            }
            if (TCNT0 >= 25)      // pulse longer than 1 ms? then logic "1"
                code.value += ((u16) tmp << i);  // the device code
        }
        return (code.value);
    }

    else  // std=4 -> RC-5;

    {

        for (i = 0; i < 13; i++) {
            if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
                T2 = 0;
            else
                T2 = 1;

            TCNT0 = 1;
            while (1) {
                time = TCNT0;
                if (time > 0x21)
                    return 0;

                if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT) && (T2 == 1)) {
                    tmp <<= 1;
                    tmp++;
                    break;
                } else if (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT)
                        && (T2 == 0)) {
                    tmp <<= 1;
                    break;
                }
            }

            //save address data
            if (i == 6) {
                code.bytes.high = (tmp & 0x5f); // save address and cut troggle bit
                tmp = 0;
            }

            //delay
            TCNT0 = 1;
            while (1) {
                time = TCNT0;
                if (time > 0x21)
                    break;
            }
        }
        code.bytes.low = tmp;
        return (code.value);
    }
}

#if (REMOTE_STD == 1)
/*
 The REC-80 format used by Panasonic is a space coded 48 bit code consisting
 of a 32 bit group id, followed by a 16 bit commmand word.
 Leading this is a header consisting of a 10T signal and a 4T pause.
 All bits start with a pulse of length T. The length of the pause following
 indicates the bit value. A T pause for a 0-bit and a 3T pause for a 1-bit.
 */

u16 REMOTE_get(void)
{
    u08 i, tmp = 0;
    u08 time;
    u08 T2,T4;
    union u16convert code;

    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal

    TCCR0=4;//update every 32us
    TCNT0 = 1;

    while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
    {
        T2 = TCNT0;
        if (T2 >= 100)    // max wait time
        return 0;
    }

    // measure time T

    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);

    T2 = TCNT0;// T is normally around 0E-10 hex = 15 -> 480 uS
    T2 = T2 * 2;
    // max time is 4T
    T4 = T2 * 2;

    for (i = 0; i < 48; i++)
    {
        TCNT0 = 1;
        while(1)
        {
            time = TCNT0;
            if (time > T4)
            return 0;

            // measure time on the lo flank
            if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
            {
                tmp <<= 1;
                if (time >= T2)
                tmp++;
                break;
            }
        }

        // save command data as we go
        if( i == 39)
        code.bytes.low = tmp;

        if( i == 47)
        code.bytes.high = tmp;

        // syncronize - wait for next hi flank
//    loop_until_bit_is_set(REMOTE_PORT - 2, REMOTE_BIT);
    }
    return code.value;
}
#endif // (REMOTE_STD == 1)
//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 2)
/*
 The NEC format is a space coded 32 bit code consisting
 of a 16 bit group id, followed by a 16 bit commmand word.
 Leading this is a header consisting of a 16T signal and a 8T pause.
 All bits start with a pulse of length T. The length of the pause following
 indicates the bit value. A T pause for a 0-bit and a 3T pause for a 1-bit.

 Modified REC80 format by Will Jenkins, wdj@cus.org.uk
 */

u16 REMOTE_get(void) {
    u08 i, tmp = 0;
    u08 time;
    u08 T2, T4;
    union u16convert code;

    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal

    TCCR0 = (4);  //update every 32us
    TCNT0 = 1;

    while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT)) {
        T2 = TCNT0;
        if (T2 >= 140)  // max wait time was 100
            return 0;
    }

    // measure time T
    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);

    T2 = TCNT0;    // T is normally around 0E-10 hex = 15 -> 480 uS
    T2 = T2 * 2;
    // max time is 4T
    T4 = T2 * 2;

    for (i = 0; i < 32; i++) {
        TCNT0 = 1;
        while (1) {
            time = TCNT0;

            if (time > T4)
                return 0;

            // measure time on the lo flank
            if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT)) {
                tmp <<= 1;
                if (time >= T2)
                    tmp += 1;
                break;
            }
        }

        // save command data as we go
        if (i == 15)
            code.bytes.high = tmp;

        if (i == 31)
            code.bytes.low = tmp;

        // syncronize - wait for next hi flank
        loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
    }

    return code.value;
}
#endif // (REMOTE_STD == 2)
//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 3) || (REMOTE_STD == 4)
/*
 The SONY format is a pulse coded 12 or 15 bit code consisting
 of a 6 bit device code, followed by a 6 or 9 bit commmand word.
 Leading this is a header consisting of a 4T signal pulse.
 All bits are composed of a pulse of either 1T or 2T in length followed by a
 1T pause. A 2T pulse indicates a 1 bit, and a 1T pulse is a 0 bit.
 */

u16 REMOTE_get(void)
{
    u08 d, T2;
    u16 tmp;
    union u16convert code;

    code.value = 0;
    TCCR0 = (4);  //update every 32us
    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);// leading signal
    if (TCNT0 <= 60) return 0;// is it the leading Signal ?

    // now looking for the Data-bits
#if (REMOTE_STD == 3)
    for(d=0; d < 14; d++)
#else
    for(d=0; d < 11; d++)
#endif
    {
        tmp = 1;
        while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
        {
            T2 = TCNT0;
            if (T2 >= 140)      // max wait time
            return 0;
        }
        TCNT0 = 1;
        while (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
        {
            T2 = TCNT0;
            if (T2 >= 140)      // max wait time
            return 0;
        }
        if (TCNT0 >= 35)      // pulse longer than 1 ms? then logic "1"
        code.value += (tmp << d);// the device code

    }

    return code.value;
}
#endif // (REMOTE_STD == 3) || (REMOTE_STD == 4)
//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 5)
/*
 The RC-5 format used by Philips is bi-phase coded 13 bit code consisting
 of a 7 bit device code, followed by 6 bit commmand word. Highest two bits of
 device code is startbit and troggle bit. Troggle bit must be masked out
 because  its change after any keypress.
 */

u16 REMOTE_get(void)
{
    u08 i, time, T2, tmp = 0;
    union u16convert code;

    TCCR0 = (4);        //update every 32us
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);// skip leading signal
    for(i=0; i<13; i++)
    {
        if(bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT) )
        T2 = 0;
        else
        T2 = 1;

        TCNT0 = 1;
        while(1)
        {
            time=TCNT0;
            if(time > 0x21)
            return 0;

            if(bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT) && (T2==1) )
            {
                tmp <<= 1;
                tmp++;
                break;
            }
            else if(bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT) && (T2==0) )
            {
                tmp <<= 1;
                break;
            }
        }

        //save address data
        if(i == 6)
        {
            code.bytes.high = (tmp & 0x5f);  // save address and cut troggle bit
            tmp=0;
        }

        //delay
        TCNT0 = 1;
        while(1)
        {
            time=TCNT0;
            if(time > 0x21)
            break;
        }
    }
    code.bytes.low = tmp;

    return code.value;
}

#endif // (REMOTE_STD == 5)

u16 decode_rc5(void) {
    u16 code;
    int i;

    for (i = 0; i < 80; i++) {
        if ((code = REMOTE_get())) {
            PORTD &= ~(_BV(3));
            waitms(150);
            PORTD |= _BV(3);
            waitms(150);
            PORTD &= ~(_BV(3));
            waitms(150);
            PORTD |= _BV(3);
            UART_putint(code, 10);
            UART_puts("\r");
            return code;
        }
    }
}

