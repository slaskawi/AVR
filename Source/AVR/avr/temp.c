
void temp_show() {
    LCD_clear();
    LCD_puts("    SET TEMP    ");
    LCD_gotoxy(1, 2);

    LCD_puts("<");
    if (m_dayTemp != 0) {
        if (m_dayTemp >= 10) {
            LCD_putint(m_dayTemp, 10);
        } else {
            LCD_puts("0");
            LCD_putint(m_dayTemp, 10);
        }
    } else {
        LCD_puts("00");
    }

    LCD_puts(" / ");

    if (m_nightTemp != 0) {
        if (m_nightTemp >= 10) {
            LCD_putint(m_nightTemp, 10);
        } else {
            LCD_puts("0");
            LCD_putint(m_nightTemp, 10);
        }
    } else {
        LCD_puts("00");
    }

    LCD_puts(" +- ");

    if (m_offsetTemp != 0) {
        LCD_putint(m_offsetTemp, 10);
    } else {
        LCD_puts("0");
    }

    LCD_puts(" C>");
//	LCD_puts("<00 / 00 +- 0 C>");
}

void set_temp_init(void) {
    u16 value;
    u08 step = 0, empty, tmp[2];
    u08 day[3], month[3], year[5];
    u08 temp1[3], temp2[3];
    u08 skipLoop = 0;

    LCD_write_command(0x0f);
    waitms(20);
    do {
        empty = 0;
        LCD_gotoxy((2 + step), 2);
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
                if (step == 5) {
                    step -= 4;
                } else if (step == 11) {
                    step -= 5;
                } else {
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
                    strcpy(temp1, tmp);
                    break;
                case 1:
                    strcat(temp1, tmp);
                    break;

                case 5:
                    strcpy(temp2, tmp);
                    break;
                case 6:
                    strcat(temp2, tmp);
                    break;
                default:
                    break;
            }
        }
        step++;
        if (step == 2)
            step += 3;
        if (step == 7)
            step += 4;
    } while (step <= 11);
    LCD_write_command(0x0C);

    if (m_dayTemp >= 0 && m_nightTemp >= 0) {
        m_requiresSettings |= SETTING_TEMP;
    }

    m_dayTemp = (atoi(temp1) % 90);
    m_nightTemp = (atoi(temp2) % 90);
    m_offsetTemp = (atoi(tmp) % 90);
}
