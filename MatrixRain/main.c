#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

struct charInfo
{
    char character;
    char color;
}buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

struct trail
{
    int start;
    int end;
    int speed;
} trails[SCREEN_WIDTH];

void setCursorPosition(int x, int y)
{
    printf("\033[%d;%dH", x, y);
}

// Uses double buffering 
void printFrame(struct charInfo buffer[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    setCursorPosition(0, 0);

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            buffer[y][x].color == 'g' ? printf("\033[32m") : printf("\033[37m");
            putchar(buffer[y][x].character);
        }
        printf("\n\r");
    }
}

void clearBuffer(struct charInfo buffer[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            buffer[y][x].character = ' ';
        }
    }
}

void delay(int milliseconds) {
    int counter = 0;
    for (int i = 0; i < milliseconds * 1000000; i++) {
        counter++; // Just incrementing the counter to waste time
    }
}

void cls()
{
    printf("\033[H\033[J"); // Clear screen
}

int mod(int dividend, int divisor) {
    while (dividend >= divisor) {
        dividend -= divisor;
    }
    return dividend;
}

void init_trail(struct trail *trail)
{
    if (mod(rand(), 4) != 0)
    {
        trail->start = -(mod(rand(), SCREEN_HEIGHT));
        trail->end = 0;
        trail->speed = 1;// mod(rand(), 5);
    }
    else {
        trail->start = -SCREEN_HEIGHT;  // Keep it off-screen
        trail->end = -SCREEN_HEIGHT;
        trail->speed = 0;  // No movement
    }
}

void update(struct charInfo buffer[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    srand(2185412);

    // For each column
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
        // Clear row with space
		for (int y = 0; y < SCREEN_HEIGHT; y++)
            buffer[y][x].character = ' ';

        // If start is negative, set it to 0
        int start = trails[x].start < 0 ? 0 : trails[x].start;

        // If end is past the screen height set it to the bottom of the screen
        int end = trails[x].end > SCREEN_HEIGHT ? SCREEN_HEIGHT : trails[x].end;

        // Set characters in the column
        for (int y = start; y < end; y++)
        {
	        // select random char and make green
            buffer[y][x].character = mod(rand(), 256);

            if (y == end - 1)
                // Print char white
                buffer[y][x].color = 'w';
            else
                // Make green
                buffer[y][x].color = 'g';
        }

        trails[x].start += trails[x].speed;
        trails[x].end += trails[x].speed;

        if (trails[x].start >= SCREEN_HEIGHT)
            init_trail(&trails[x]);
	}
}

int main() {
    // Initialize trails
    for (int i = 0; i < SCREEN_WIDTH; i++)
        init_trail(&trails[i]);

    for (int i = 0; i < 100; i++)
    {
        update(buffer);
        printFrame(buffer);
        delay(5);
    }

    delay(5000);
    cls();

    printf("   _____                            ____   _____ \n");
    printf("  / ____|                          / __ \\ / ____|\n");
    printf(" | |  __  ___  ___  _ __ __ _  ___| |  | | (___  \n");
    printf(" | | |_ |/ _ \\/ _ \\| '__/ _` |/ _ \\ |  | |\\___ \\ \n");
    printf(" | |__| |  __/ (_) | | | (_| |  __/ |__| |____) |\n");
    printf("  \\_____|\\___|\\___/|_|  \\__, |\\___|\\____/|_____/ \n");
    printf("                         __/ |                   \n");
    printf("                        |___/                    \n");


    return 0;
}
