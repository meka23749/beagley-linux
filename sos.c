/*
 * sos.c — Blink "SOS" in Morse code on the BeagleY-AI LED.
 * beagley-linux — Embedded Linux From Scratch
 *
 * Cross-compile (static, so it runs on the bare BusyBox rootfs):
 *     aarch64-linux-gnu-gcc -static -o sos sos.c
 *
 * Uses the fundamental embedded syscalls: open / write / close / usleep.
 */

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <fcntl.h>      // open
#include <unistd.h>     // write, close, usleep
#include <string.h>     // strlen

#define LED_BRIGHTNESS "/sys/class/leds/led-0/brightness"
#define LED_TRIGGER    "/sys/class/leds/led-0/trigger"

/* Morse timings (microseconds) */
#define DOT        200000    /* dot  : 0.2s          */
#define DASH       600000    /* dash : 0.6s (3x dot) */
#define GAP        200000    /* gap between symbols  */
#define LETTER_GAP 600000    /* gap between letters  */
#define WORD_GAP  1400000    /* gap between repeats  */

/* Write a string into a sysfs file */
void write_sysfs(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) { perror(path); exit(1); }
    write(fd, value, strlen(value));
    close(fd);
}

/* Turn the LED on for "duration", then off */
void blink(int duration) {
    write_sysfs(LED_BRIGHTNESS, "1");   /* on  */
    usleep(duration);
    write_sysfs(LED_BRIGHTNESS, "0");   /* off */
    usleep(GAP);
}

void dot(void)  { blink(DOT);  }
void dash(void) { blink(DASH); }

int main(void) {
    /* Take manual control of the LED */
    write_sysfs(LED_TRIGGER, "none");

    printf("Transmitting SOS in Morse... (Ctrl+C to stop)\n");

    while (1) {
        /* S = dot dot dot */
        dot(); dot(); dot();
        usleep(LETTER_GAP);

        /* O = dash dash dash */
        dash(); dash(); dash();
        usleep(LETTER_GAP);

        /* S = dot dot dot */
        dot(); dot(); dot();

        usleep(WORD_GAP);
    }

    return 0;
}
