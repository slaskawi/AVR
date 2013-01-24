/**
 * PC communication handles
 * @author Michal Wachowicz
 */

void PC_DateTime_Show() {
    LCD_clear();
    LCD_puts("Sending dateTime");

    PC_timer_send();
    PC_date_send();

}

/**
 * Sends current times to PC
 */
void PC_timer_send(void) {
    PCF8583_get_time(&hour, &min, &sec, &msec);

    LCD_gotoxy(5, 2);
    if (hour < 10) {
        LCD_putc('0');
        LCD_putint(hour, 10);
        UART_putint(0, 10);
        UART_putint(hour, 10);
    } else {
        LCD_putint(hour, 10);
        UART_putint(hour, 10);
    }

    LCD_putc(':');
    UART_putc(':');

    if (min < 10) {
        LCD_putc('0');
        LCD_putint(min, 10);
        UART_putint(0, 10);
        UART_putint(min, 10);

    } else {
        LCD_putint(min, 10);
        UART_putint(min, 10);
    }

    LCD_putc(':');
    UART_putc(':');

    if (sec < 10) {
        LCD_putc('0');
        LCD_putint(sec, 10);
        UART_putint(0, 10);
        UART_putint(sec, 10);

    } else {
        LCD_putint(sec, 10);
        UART_putint(sec, 10);
    }
}

/**
 * Sets time
 */
void PC_time_set(void) {
    u08 hour[3], min[3], sec[3];
    u08 temp[10];

    UART_gets(command);

    strcpy(temp, command);

    strlcpy(hour, temp, 3);

    char *wsk = &temp;
    wsk = wsk + 3;

    strlcpy(min, wsk, 3);

    wsk = wsk + 3;
    strlcpy(sec, wsk, 3);

    PCF8583_set_time(atoi(hour), atoi(min), atoi(sec), 0);
    m_requiresSettings |= SETTING_TIME;
}

void PC_temp_set(void) {
    u08 hour[3], min[3], sec[3];
    u08 temp[10];

    UART_gets(command);

    strcpy(temp, command);

    strlcpy(hour, temp, 3);

    char *wsk = &temp;
    wsk = wsk + 3;

    strlcpy(min, wsk, 3);

    wsk = wsk + 3;
    strlcpy(sec, wsk, 3);

    m_dayTemp = atoi(hour);
    m_nightTemp = atoi(min);
    m_offsetTemp = atoi(sec);

    m_requiresSettings |= SETTING_TEMP;
}

void PC_temp_time_set(void) {
    u08 hour[3], min[3];
    u08 temp[10];

    UART_gets(command);

    strcpy(temp, command);

    strlcpy(hour, temp, 3);

    char *wsk = &temp;
    wsk = wsk + 3;

    strlcpy(min, wsk, 3);
    m_nightHour = atoi(hour);
    m_nightLast = atoi(min);

}

/**
 * Sends date to PC
 */
void PC_date_send(void) {
    PCF8583_get_date(&day, &month, &year);

    LCD_gotoxy(4, 2);
    if (day < 10) {
        LCD_putc('0');
        LCD_putint(day, 10);
        UART_putint(0, 10);
        UART_putint(day, 10);
    } else {
        LCD_putint(day, 10);
        UART_putint(day, 10);
    }

    LCD_putc('/');
    UART_putc('/');

    if (month < 10) {
        LCD_putc('0');
        LCD_putint(month, 10);
        UART_putint(0, 10);
        UART_putint(month, 10);
    } else {
        LCD_putint(month, 10);
        UART_putint(month, 10);
    }

    LCD_putc('/');
    UART_putc('/');

    LCD_putint(year, 10);
    UART_putint(year, 10);
}

/**
 * sets date
 */
void PC_date_set(void) {
    u08 day[3], month[3], year[5];
    u08 temp[15];

    UART_gets(command);

    strcpy(temp, command);

    strlcpy(day, temp, 3);

    char *wsk = &temp;
    wsk = wsk + 3;

    strlcpy(month, wsk, 3);

    wsk = wsk + 3;
    strlcpy(year, wsk, 5);

    PCF8583_set_date(atoi(day), atoi(month), atoi(year));
    m_requiresSettings |= SETTING_DATE;
}

/**
 * Shows temperature
 */
void PC_temperature_show(void) {
    LCD_clear();
    LCD_puts("  Temperature:  ");
    get_temperature();
}

/**
 * Initializes PC console
 */
void PC_console_init(void) {
    u08 x;
    LCD_clear();
    LCD_write_command(0x0f);
    while (1) {
        while (UART_unread_data()) {
            x = UART_getc();
            if (x == '!')
                break;
            else
                LCD_putc(x);
        }
        if (x == '!')
            break;
    }
    LCD_write_command(0x0C);
}
