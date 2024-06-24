#pragma once

char int_to_char(int value) {
    if (value < 1 || value > 9) {
        return ' ';
    }
    return value + '0';
}

int char_to_int(char value) {
    if (value < '1' || value > '9') {
        return 0;
    }
    return value - '0';
}