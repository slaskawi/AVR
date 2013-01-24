u08 _msec, msec1, _sec, sec1, _min, min1, _hour, hour1, _day, _month; // aktualny czas
u16 _year;


u08 indexTimer = 0;
u08 indexTemperature = 0;

void power_on(void) {
    LCD_gotoxy(1, 1);
    LCD_puts("      Ready     ");
    if (m_requiresSettings != SETTING_ALL) {
        LCD_gotoxy(1, 2);
        LCD_puts("Change Settings!");
    }
}

void updateTime() {
    PCF8583_get_time(&_hour, &_min, &_sec, &_msec);
}

void updateTemperature() {
    if (((++indexTemperature) & 0xFF) % 0x0A == 0) {
        get_temperatureSilent();
    }
}

void dayNightDecision() {
    u08 startTime = m_nightHour;
    u08 stopTime = (startTime + m_nightLast) % 24;
    LCD_gotoxy(1, 2);
    LCD_putint(_hour, 10);
    LCD_puts(":");
    LCD_putint(_min, 10);
    LCD_puts("/");
    LCD_putint(startTime, 10);
    LCD_puts("/");
    LCD_putint(stopTime, 10);
    LCD_puts("/");
    if (_hour >= startTime && _hour <= stopTime) {
        LCD_puts("N");
        //fusing condition
        if (m_currentTemperature + m_offsetTemp < m_nightTemp) {
            if (m_work == 0) {
                PORTD |= _BV(4);
            } else {
                PORTD &= ~(_BV(4));
                UART_puts("night - fusing");
            }
        } else {
            PORTD |= _BV(4);
        }
        //cooling condition
        if (m_currentTemperature > (m_nightTemp + m_offsetTemp)) {
            if (m_work == 0) {
                PORTD |= _BV(7);
            } else {
                PORTD &= ~(_BV(7));
                UART_puts("night - cooling");
            }
        } else {
            PORTD |= _BV(7);
        }
    } else {
        LCD_puts("D");
        //day fusing
        if ((m_currentTemperature + m_offsetTemp) < m_dayTemp) {
            if (m_work == 0) {
                PORTD |= _BV(4);
            } else {
                PORTD &= ~(_BV(4));
                UART_puts("day - fusing");
            }
        } else {
            PORTD |= _BV(4);
        }
        //day cooling condition
        if (m_currentTemperature > (m_dayTemp + m_offsetTemp)) {
            if (m_work == 0) {
                PORTD |= _BV(7);
            } else {
                PORTD &= ~(_BV(7));
                UART_puts("day - fusing");
            }
        } else {
            PORTD |= _BV(7);
        }
    }
    LCD_puts("/");
    LCD_putint(m_currentTemperature, 10);
}

remote_switch(u16 decode_value) {
    u16 value;

    switch (decode_value) {
        // power on/off
        case KEY_POWER: {
            power_on();
            break;
        }

            // temperature
        case KEY_TEMPERATURE: {
            LCD_clear();
            LCD_puts("  Temperature:  ");

            u08 indexer = 0;
            for (value = 0; value == 0;
                    value = decode_rc5(), indexer = (++indexer & 0xff)) {
                if ((indexer % 0x0A) == 0) {
                    LCD_clear();
                    LCD_puts("  Temperature:  ");
                    get_temperature();
                }
            }
            LCD_clear();
            power_on();

            break;
        }

            // zegarek
        case KEY_TIMER: {
            LCD_clear();
            do {
                LCD_puts("     Clock     ");
                watch();
                value = decode_rc5();
                if (value == KEY_RIGHT) {
                    LCD_clear();
                    LCD_puts("      Date      ");
                    do {
                        date();
                        value = decode_rc5();
                        if (value == KEY_RIGHT) {

                            //ustawienia temp
                            LCD_clear();
                            temp_show();
                            do {
                                value = decode_rc5();
                                if (value == KEY_ENTER) {
                                    set_temp_init();
                                    value = decode_rc5();
                                } else if (value == KEY_RESET) {
                                    hour1 = 0, min1 = 0, sec1 = 0, msec1 = 0;

                                } else if (value == KEY_RIGHT) {
                                    set_time_show();
                                    do {
                                        value = decode_rc5();
                                        if (value == KEY_ENTER)
                                            set_time_init();
                                        else if (value == KEY_RIGHT) {
                                            set_date_show();
                                            do {
                                                value = decode_rc5();
                                                if (value == KEY_ENTER)
                                                    set_date_init();
                                                else if (value == KEY_RIGHT) {
                                                    set_nightTime_show();
                                                    do {
                                                        value = decode_rc5();
                                                        if (value
                                                                == KEY_ENTER) {
                                                            set_nightTime_init();
                                                        }

                                                        else if (value
                                                                == KEY_POWER) {
                                                            LCD_clear();
                                                            power_on();
                                                            return;
                                                        }
                                                    } while (!((value
                                                            == KEY_LEFT)
                                                            | (value
                                                                    == KEY_TIMER)
                                                            | (value
                                                                    == KEY_POWER)));
                                                    value = 1;
                                                    set_date_show();
                                                } else if (value == KEY_POWER) {
                                                    LCD_clear();
                                                    power_on();
                                                    return;
                                                }
                                            } while (!((value == KEY_LEFT)
                                                    | (value == KEY_TIMER)
                                                    | (value == KEY_POWER)));
                                            value = 1;
                                            set_time_show();
                                        } else if (value == KEY_POWER) {
                                            LCD_clear();
                                            power_on();
                                            return;
                                        }
                                    } while (!((value == KEY_LEFT)
                                            | (value == KEY_TIMER)
                                            | (value == KEY_POWER)));
                                    value = 1;
                                    LCD_clear();
                                    temp_show();
                                    //stoper_show();
                                    hour1 = 0;
                                    min1 = 0;
                                    sec1 = 0;
                                    msec1 = 0;
                                }
                            } while (!((value == KEY_LEFT)
                                    | (value == KEY_TIMER)
                                    | (value == KEY_POWER)));
                            if (value == KEY_LEFT) {
                                value = 1;
                                LCD_clear();
                                LCD_puts("      Date      ");
                            }
                        }
                    } while (!((value == KEY_LEFT) | (value == KEY_TIMER)
                            | (value == KEY_POWER)));
                    LCD_clear();
                }
            } while (!((value == KEY_TIMER) | (value == KEY_POWER)));
            power_on();
            break;
        }

        case KEY_OPTIONS: {
            options_show();
            do {
                value = decode_rc5();
                if (value == KEY_RIGHT) {
                    work_settings_show();
                    do {
                        value = decode_rc5();
                        if ((value == KEY_ENTER) & (bit_is_clear(PIND, 5)))
                            work_off();
                        else if ((value == KEY_ENTER)
                                & (!(bit_is_clear(PIND, 5))))
                            work_on();

                        if (value == KEY_RIGHT) {
                            PC_connection_show();

                            do {
                                value = decode_rc5();
                                if ((value == KEY_ENTER)
                                        & (bit_is_clear(PIND, 6)))
                                    PC_connection_disable();
                                else if ((value == KEY_ENTER)
                                        & (!(bit_is_clear(PIND, 6))))
                                    PC_connection_enable();
                                else if (value == KEY_POWER) {
                                    LCD_clear();
                                    power_on();
                                    return;
                                }
                            } while (!((value == KEY_LEFT)
                                    | (value == KEY_OPTIONS)
                                    | (value == KEY_POWER)));
                            value = 1;
                            work_settings_show();
                        } else if (value == KEY_POWER) {
                            LCD_clear();
                            power_on();
                            return;
                        }
                    } while (!((value == KEY_LEFT) | (value == KEY_OPTIONS)
                            | (value == KEY_POWER)));
                    value = 1;
                    options_show();
                }
            } while (!((value == KEY_OPTIONS) | (value == KEY_POWER)));
            power_on();
            break;
        }

        case 16269: {
            LCD_clear();
            LCD_puts("   UART DEBUG   ");
            do {
                while (UART_unread_data())
                    UART_putc(UART_getc());
                value = decode_rc5();
            } while (!((value == 16269) | (value == KEY_POWER)));
            power_on();
            break;
        }

        default: {
            break;
        }
    }
}

