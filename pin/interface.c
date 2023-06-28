#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "defn.h"

// Colors

// Globals
WINDOW* frame_border;
WINDOW* typebox;
int TLINES = TYPEBOX_HEIGHT + 2;
int TCOLS;

// Functions
void event_loop();

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
    frame_border = create_window(LINES - TLINES, COLS, 0, 0);

    // Draw typebar border
    TCOLS = COLS;
    typebox = create_window(TLINES, TCOLS, 0, LINES - TLINES);

    // Switch input to typebox
    keypad(typebox, TRUE);

    // Move cursor to typebar
    wmove(typebox, 10, 10);

    // Refresh everything
    wrefresh(typebox);

    // Start event loop
    event_loop(border, typebox);

    endwin();
}

// Main event loop for keys
void event_loop() {
    // A couple quick constants (not const bc screen resizing)
    int XMAX = TCOLS - 2;
    int YMAX = TLINES - 2;

    int ch;    // int for expanded char set
    int x = 1;
    int y = 1;

    wmove(typebox, y, x);
    wrefresh(typebox);

    while ((ch = wgetch(typebox)) != KEY_F(1)) {
        if (isprint(ch)) {  // If regular key, just write
            if (x < XMAX) {
                waddch(typebox, ch);
                x++;

            } else {
                // At edge, new line
                if (y < YMAX) {
                    waddch(typebox, ch);
                    y++;
                    x = 1;
                }
            }

        } else {
            // Specific special case keys

            switch(ch) {
                case KEY_UP:  // Change later to scroll messages
                    // >1 to account for border at top
                    y -= (y > 1);   // Equals a bool to decide whether move 1 or 0
                    break;
                case KEY_DOWN:  // Change later to scroll messages
                    y += (y < TLINES - 1);
                    break;
                case '\n':
                    break;   // Send message
                case KEY_BACKSPACE:
                    if (!(x == 1 && y == 1)) {   // If not first char
                        if (x == 1 && y != 1) {  // If end of line
                            mvwaddch(typebox, --y, (x = XMAX), ' ');

                        } else {   // Normal space
                            mvwaddch(typebox, y, --x, ' ');
                        }
                    }
            }
        }


        wmove(typebox, y, x);
        wrefresh(typebox);
    }
}

// Temporary testing main
int main(void) {
    setup_interface();
    return 0;
}
