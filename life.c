#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int terminal_size(int* width, int* height) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0) {
        return -1;
    }
    // subtract one from column count to help reduce tearing
    *width = w.ws_col-1;
    *height = w.ws_row;
    return 0;
}

int print(bool* board, int width, int height) {
    printf("\033[1;1H");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board[i*j]) {
                printf("*");
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}

int turn (bool* board, bool* next, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = 0;
            if (x > 0 && y > 0)
                neighbors += board[((y-1)*width) + (x-1)];
            if (y > 0)
                neighbors += board[((y-1)*width) + x];
            if (y > 0 && x < (width-1))
                neighbors += board[((y-1)*width) + (x+1)];
            if (x > 0)
                neighbors += board[(y*width) + (x-1)];
            if (x < (width-1))
                neighbors += board[(y*width) + (x+1)];
            if (y < (height-1) && x > 0) 
                neighbors += board[((y+1)*width) + (x-1)];
            if (y < (height-1))
                neighbors += board[((y+1)*width) + x];
            if (y < (height-1) && x < (width-1))
                neighbors += board[((y+1)*width) + (x+1)];

            // if the current cell is alive
            if (board[y*width + x]) {
                // and has two or three neighbors
                if (neighbors == 2 || neighbors == 3)
                    // continue to live
                    next[y*width + x] = true;
                else
                    // die
                    next[y*width + x] = false;
            } // else if current cell is dead and has three neighbors
            else if (neighbors == 3) {
                // come to life
                next[y*width + x] = true;
            } else {
                // remain dead
                next[y*width + x] = false;
            }
        }
    }
    return 0;
}

int main (int argc, char **argv) {
    int width, height;
    if (terminal_size(&width, &height) != 0) {
        puts("error getting terminal size");
        return 1;
    }

    bool *step = calloc(width * height, sizeof(bool));
    bool *next = calloc(width * height, sizeof(bool));
    if (step == NULL || next == NULL) {
        puts("error callocing board");
        return 1;
    }

    srand(0);
    for (int i = 0; i < width*height; i++) {
        step[i] = rand() % 2;
    }

    while (1) {
        print(step, width, height);
        turn(step, next, width, height);
        step = next;
        sleep(1);
    }
    return 0;
}