#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "defn.h"

// Colors

// Globals
WINDOW* message_box;
WINDOW* typebox;

// Psuedo-constant window sizes
int TLINES = TYPEBOX_HEIGHT + 2;
int TYPEBOX_WIDTH;
int MESSAGE_BOX_HEIGHT;
int MESSAGE_BOX_WIDTH;

int MSG_MAX = 1000;   // Initial max for message count, doubles when space runs out
char** messages;
int msgix = 0;

// Functions
void event_loop();
void clear_window(WINDOW* win, int height);

// Write message array to screen
void write_messages() {
    int line = MESSAGE_BOX_HEIGHT - 1;   // Line to add to, starts at bottom

    // Clear message_box
    clear_window(message_box, MESSAGE_BOX_HEIGHT);

    int line_height;
    for (int i = msgix - 1; i >= 0; i--) {
        // Get line height for wrapping pruposes
        line_height = (strlen(messages[i])) / MESSAGE_BOX_WIDTH;

        mvwaddstr(message_box, (line -= line_height), 0, messages[i]);
        line -= 1 + MSGGAP;   // Allows for configurable text gap
    }

    wmove(typebox, 0, 0);
    keypad(typebox, TRUE);
    wrefresh(message_box);
    wrefresh(typebox);
}

// Add a local message to messages list
void add_message(char* buf, int len) {
    messages[msgix] = (char*) malloc(len + namelen + 3);
    snprintf(messages[msgix], len + namelen + 3, "<%s> %s", name, buf);

    // If limit hit, realloc
    if (++msgix == MSG_MAX) {
        messages = realloc(messages, MSG_MAX);
    }
}

// Add a remote message to messages
void add_remote(char* buf, int len) {
    messages[msgix] = (char*) malloc(len + namelen + 3);
    strncpy(messages[msgix], buf, strlen(buf));

    // If limit hit, realloc
    if (++msgix == MSG_MAX) {
        messages = realloc(messages, MSG_MAX);
    }
}

// Clears a window's contents
void clear_window(WINDOW* win, int height) {
    // Create clearing string
    char blanks[MESSAGE_BOX_WIDTH];
    sprintf(blanks, "%*c", MESSAGE_BOX_WIDTH, ' ');

    // Clear each line
    for (int i = 0; i < height; i++) {
        mvwaddstr(win, i, 0, blanks);   // Clear line
    }

    wrefresh(win);
}

// Redraw window
void redraw_window() {

}

// Create and draw a window
WINDOW* create_border(int height, int width, int x, int y) {
    WINDOW* temp;

    temp = newwin(height, width, y, x);
    wborder(temp, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(temp);

    return temp;
}

// Initial setup
void* start_interface(void* argv) {
    // Set up gui stuff
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Set up message array
    messages = (char**) calloc(MSG_MAX, sizeof(char*));

    // Set up color pairs

    // Draw main border
    WINDOW* message_box_border = create_border(LINES - TLINES, COLS, 0, 0);

    // Draw typebar border
    WINDOW* typebox_border = create_border(TLINES, COLS, 0, LINES - TLINES);

    // Setup window sizes
    TYPEBOX_WIDTH = COLS - 4;   // -4 -> 2 border 2 padding

    MESSAGE_BOX_WIDTH = COLS - 4;   // -4 -> 2 border 2 padding
    MESSAGE_BOX_HEIGHT = LINES - TLINES - 2;  // -2 border

    // Create actual windows inside borders
    message_box = newwin(MESSAGE_BOX_HEIGHT, MESSAGE_BOX_WIDTH, 1, 2);
    typebox = newwin(TYPEBOX_HEIGHT, TYPEBOX_WIDTH, LINES - TLINES + 1, 2);

    // Switch input to typebox
    keypad(typebox, TRUE);

    // Move cursor to typebar
    wmove(typebox, 0, 0);

    // Refresh everything
    wrefresh(typebox);

    // Start event loop
    event_loop(border, typebox);

    endwin();
}

// Main event loop for keys
void event_loop() {
    // A couple quick constants (not const bc screen resizing)
    int XMAX = TYPEBOX_WIDTH - 1;
    int YMAX = TYPEBOX_HEIGHT - 1;  // -1 because checks at end of line
    int XSTART = 0;

    int ch;    // int for expanded char set
    int x = 0;
    int y = 0;

    // Buffer to hold current message being typed
    char buffer[MAXMSG];
    int addix = -1;   // Next index to add character

    // Move to start of box
    wmove(typebox, y, x);
    wrefresh(typebox);

    // Run until F1 quit key
    while ((ch = wgetch(typebox)) != KEY_F(1)) {
        if (isprint(ch) && (addix < MAXMSG)) {  // If regular key, just write
            if (x < XMAX) {
                waddch(typebox, ch);
                buffer[++addix] = ch;
                x++;

            } else {
                // At edge, new line
                if (y < YMAX) {
                    waddch(typebox, ch);
                    buffer[++addix] = ch;
                    y++;
                    x = 0;
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
                case KEY_BACKSPACE:
                    if (!(x == 0 && y == 0)) {   // If not first char
                        if (x == 0 && y != 0) {  // If end of line
                            mvwaddch(typebox, --y, (x = XMAX), ' ');

                        } else {   // Normal space
                            mvwaddch(typebox, y, --x, ' ');
                        }

                        // Remove from buffer
                        buffer[addix--] = 0;
                    }
                    break;
                case '\n':   // Enter
                    // If not empty, add
                    if (strlen(buffer) > 0) {
                        buffer[++addix] = 0;
                        add_message(buffer, addix + 1);
                        write_messages();
                        send_message(buffer, addix + 1);

                        clear_window(typebox, TYPEBOX_HEIGHT);
                        memset(buffer, 0, addix);
                        addix = -1;

                        y = 0; x = 0;
                    }
            }
        }


        wmove(typebox, y, x);
        wrefresh(typebox);
    }
}

/*
// Temporary testing main
int main(void) {
    name = "test";
    namelen = 4;
    start_interface(NULL);
    return 0;
}
*/
