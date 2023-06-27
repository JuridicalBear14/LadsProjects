#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

// Colors

// Write message array to screen
void write_messages() {

}

// Clears a window
void clear_window() {

}

// Redraw window
void redraw_window() {

}

// Create and draw a window
WINDOW* create_window(int height, int width, int x, int y) {
    WINDOW* temp;

    temp = newwin(height, width, y, x);
    wborder(temp, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(temp);

    return temp;
}

// Initial setup
void setup_interface() {
    // Set up gui stuff
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Set up color pairs

    // Draw main border
    WINDOW* border = create_window(LINES, COLS, 0, 0);

    // Draw typebar border
    WINDOW* typebox = create_window(3, COLS, 0, LINES - 3);

    sleep(5);

    endwin();
}

// Main event loop
void event_loop() {
    
}

// Temporary testing main
int main(void) {
    setup_interface();
    return 0;
}
