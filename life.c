#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int terminal_size(int* width, int* height) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0) {
        return -1;
    }
    *width = w.ws_col;
    *height = w.ws_row;
    return 0;
}

int main (int argc, char **argv) {
    int width, height;

    while (1) {
        if (terminal_size(&width, &height) != 0) {
            puts("error getting terminal size");
            return -1;
        }
        printf("%dx%d\n", width, height);
        sleep(1);
    }
    return 0;
}