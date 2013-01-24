u08 msec, sec, min, hour, day, month;
u16 year;

void watch(void) {
    PCF8583_get_time(&hour, &min, &sec, &msec);

    LCD_gotoxy(5, 2);
    if (hour < 10)
        LCD_puts("0");

    LCD_putint(hour, 10);

    if (min < 10)
        LCD_puts(":0");
    else
        LCD_putc(':');
    LCD_putint(min, 10);
    if (sec < 10)
        LCD_puts(":0");
    else
        LCD_putc(':');
    LCD_putint(sec, 10);

    LCD_gotoxy(16, 2);
    LCD_putc(62);
}

void date(void) {
    PCF8583_get_date(&day, &month, &year);

    LCD_gotoxy(1, 2);
    LCD_putc(60);

    LCD_gotoxy(4, 2);
    if (day < 10)
        LCD_puts("0");
    LCD_putint(day, 10);
    LCD_putc('/');
    if (month < 10)
        LCD_puts("0");
    LCD_putint(month, 10);
    LCD_putc('/');
    LCD_putint(year, 10);

    LCD_gotoxy(16, 2);
    LCD_putc(62);
}

void set_time_show(void) {
    LCD_clear();
    LCD_puts("    SET TIME    ");
    LCD_gotoxy(1, 2);
    LCD_putc(60);
    LCD_puts("   00:00:00   ");

    LCD_gotoxy(16, 2);
    LCD_putc(62);
}

void set_time_init(void) {
    u16 value;
    u08 step = 0, empty, tmp[2];
    u08 hour[3], min[3], sec[3];
    u08 skipLoop = 0;

    LCD_write_command(0x0f);
    waitms(20);
    do {
        empty = 0;
        LCD_gotoxy((5 + step), 2);
        value = decode_rc5();
        switch (value) {
            case KEY_0: {
                LCD_puts("0");
                strcpy(tmp, "0");
                break;
            }

            case KEY_1: {
                LCD_puts("1");
                strcpy(tmp, "1");
                break;
            }

            case KEY_2: {
                LCD_puts("2");
                strcpy(tmp, "2");
                break;
            }

            case KEY_3: {
                LCD_puts("3");
                strcpy(tmp, "3");
                break;
            }
            case KEY_4: {
                LCD_puts("4");
                strcpy(tmp, "4");
                break;
            }
            case KEY_5: {
                LCD_puts("5");
                strcpy(tmp, "5");
                break;
            }

            case KEY_6: {
                LCD_puts("6");
                strcpy(tmp, "6");
                break;
            }

            case KEY_7: {
                LCD_puts("7");
                strcpy(tmp, "7");
                break;
            }

            case KEY_8: {
                LCD_puts("8");
                strcpy(tmp, "8");
                break;
            }

            case KEY_9: {
                LCD_puts("9");
                strcpy(tmp, "9");
                break;
            }

            case KEY_LEFT: {
                step -= 1;
                if (step == 2 || step == 5) {
                    step--;
                }
                skipLoop = 1;
                break;
            }

            case KEY_RIGHT: {
                empty = 1;
                break;
            }

            default: {
                step--;
                empty = 1;
                break;
            }
        }

        if(skipLoop) {
            skipLoop = 0;
            continue;
        }

        if (!empty) {
            switch (step) {
                case 0:
                    strcpy(hour, tmp);
                    break;
                case 1:
                    strcat(hour, tmp);
                    break;
                case 3:
                    strcpy(min, tmp);
                    break;
                case 4:
                    strcat(min, tmp);
                    break;
                case 6:
                    strcpy(sec, tmp);
                    break;
                case 7:
                    strcat(sec, tmp);
                    break;
                default:
                    break;
            }
        }
        step++;
        if (step == 2)
            step++;
        if (step == 5)
            step++;
    } while (step <= 7);
    LCD_write_command(0x0C);
    m_requiresSettings |= SETTING_TIME;
    PCF8583_set_time(atoi(hour) % 24, atoi(min) % 60, atoi(sec) % 60, 0);
}

void set_date_show(void) {
    LCD_clear();
    LCD_puts("    SET DATE    ");
    LCD_gotoxy(1, 2);
    LCD_putc(60);

    LCD_gotoxy(4, 2);
    LCD_puts("00/00/0000");

    LCD_gotoxy(16, 2);
    LCD_putc(62);
}

void set_date_init(void) {
    u16 value;
    u08 step = 0, empty, tmp[2];
    u08 day[3], month[3], year[5];
    u08 skipLoop = 0;

    LCD_write_command(0x0f);
    waitms(20);
    do {
        empty = 0;
        LCD_gotoxy((4 + step), 2);
        value = decode_rc5();
        switch (value) {
            case KEY_0: {
                LCD_puts("0");
                strcpy(tmp, "0");
                break;
            }

            case KEY_1: {
                LCD_puts("1");
                strcpy(tmp, "1");
                break;
            }

            case KEY_2: {
                LCD_puts("2");
                strcpy(tmp, "2");
                break;
            }

            case KEY_3: {
                LCD_puts("3");
                strcpy(tmp, "3");
                break;
            }
            case KEY_4: {
                LCD_puts("4");
                strcpy(tmp, "4");
                break;
            }
            case KEY_5: {
                LCD_puts("5");
                strcpy(tmp, "5");
                break;
            }

            case KEY_6: {
                LCD_puts("6");
                strcpy(tmp, "6");
                break;
            }

            case KEY_7: {
                LCD_puts("7");
                strcpy(tmp, "7");
                break;
            }

            case KEY_8: {
                LCD_puts("8");
                strcpy(tmp, "8");
                break;
            }

            case KEY_9: {
                LCD_puts("9");
                strcpy(tmp, "9");
                break;
            }

            case KEY_LEFT: {
                step -= 1;
                if (step == 2 || step == 5) {
                    step--;
                }
                skipLoop = 1;
                break;
            }

            case KEY_RIGHT: {
                empty = 1;
                break;
            }

            default: {
                step--;
                empty = 1;
                break;
            }
        }

        if(skipLoop) {
            skipLoop = 0;
            continue;
        }

        if (!empty) {
            switch (step) {
                case 0:
                    strcpy(day, tmp);
                    break;
                case 1:
                    strcat(day, tmp);
                    break;
                case 3:
                    strcpy(month, tmp);
                    break;
                case 4:
                    strcat(month, tmp);
                    break;
                case 6:
                    strcpy(year, tmp);
                    break;
                case 7:
                    strcat(year, tmp);
                    break;
                case 8:
                    strcat(year, tmp);
                    break;
                case 9:
                    strcat(year, tmp);
                    break;
                default:
                    break;
            }
        }
        step++;
        if (step == 2)
            step++;
        if (step == 5)
            step++;
    } while (step <= 9);

    m_requiresSettings |= SETTING_DATE;

    LCD_write_command(0x0C);

    PCF8583_set_date(atoi(day) % 31, atoi(month) % 12, atoi(year));
}

void set_nightTime_show(void) {
    LCD_clear();
    LCD_puts(" SET NIGHT TIME ");
    LCD_gotoxy(1, 2);
    LCD_putc(60);

    LCD_gotoxy(5, 2);
    LCD_puts(" 00/00");
}

void set_nightTime_init() {
    u16 value;
    u08 step = 0, empty, tmp[2];
    u08 hour[3], min[3], sec[3];
    u08 day[3], month[3];
    u08 skipLoop = 0;

    LCD_write_command(0x0f);
    waitms(20);
    do {
        empty = 0;
        LCD_gotoxy((6 + step), 2);
        value = decode_rc5();
        switch (value) {
            case KEY_0: {
                LCD_puts("0");
                strcpy(tmp, "0");
                break;
            }

            case KEY_1: {
                LCD_puts("1");
                strcpy(tmp, "1");
                break;
            }

            case KEY_2: {
                LCD_puts("2");
                strcpy(tmp, "2");
                break;
            }

            case KEY_3: {
                LCD_puts("3");
                strcpy(tmp, "3");
                break;
            }
            case KEY_4: {
                LCD_puts("4");
                strcpy(tmp, "4");
                break;
            }
            case KEY_5: {
                LCD_puts("5");
                strcpy(tmp, "5");
                break;
            }

            case KEY_6: {
                LCD_puts("6");
                strcpy(tmp, "6");
                break;
            }

            case KEY_7: {
                LCD_puts("7");
                strcpy(tmp, "7");
                break;
            }

            case KEY_8: {
                LCD_puts("8");
                strcpy(tmp, "8");
                break;
            }

            case KEY_9: {
                LCD_puts("9");
                strcpy(tmp, "9");
                break;
            }

            case KEY_LEFT: {
                step -= 1;
                if (step == 2 || step == 5) {
                    step--;
                }
                skipLoop = 1;
                break;
            }

            case KEY_RIGHT: {
                empty = 1;
                break;
            }

            default: {
                step--;
                empty = 1;
                break;
            }
        }

        if(skipLoop) {
            skipLoop = 0;
            continue;
        }

        if (!empty) {
            switch (step) {
                case 0:
                    strcpy(hour, tmp);
                    break;
                case 1:
                    strcat(hour, tmp);
                    break;
                case 3:
                    strcpy(min, tmp);
                    break;
                case 4:
                    strcat(min, tmp);
                    break;
                case 6:
                    strcpy(sec, tmp);
                    break;
                case 7:
                    strcat(sec, tmp);
                    break;
                default:
                    break;
            }
        }
        step++;
        if (step == 2)
            step++;
        //if (step == 5) step++;
    } while (step <= 4);
    LCD_write_command(0x0C);

    m_nightHour = atoi(hour);
    m_nightLast = atoi(min);

}

