#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 10/6/2022
// Parker Lowney and Natalie Norris

// Number progression:
// 0-9 (< 10)
// a-z (< 37)
// A-Z (< 63)
// [number] (<= 89)


// Pow rewritten for integers (not particularly necessary)
int ipow(int base, int power) {
    int num = 1;

    for (int i = 0; i < power; i++) {
        num *= base;
    }

    return num;
}

int largestpower(int basey, int numby) {
    // start at 1 and go up until you reach a 'can't do this', then return powers
    int power = 0;
    while (numby >= ipow(basey, power)) {
        power++;
    }

    // Inherently overshoots, so -1 needed
    return power - 1;
}

// Prints out numbers as a single digit, corresponding to above chart
void printdigit(int numb) {

    char str[20];
    sprintf(str, "%d", numb);

    // Basic table for common values (single char digits)
    if (numb < 10) {                    // Print numbers through 9
        printf("%c", ('0' + numb));
        
    } else if (numb < 37) {             // Print lowercase letters through 37
        printf("%c", ('W' + numb));     // W is 87, so ex 10+87 = 97 which is ascii for a, 10=a

    } else if (numb < 63) {             // Print capital letters through 63
        printf("%c", (char) (numb + 28));

    } else {                            // If it's above 89, print number in brackets
        printf("[%s]", str);
    }
}

int main(int argc, char* argv[]) {
    int base;
    int num;

    // Argument guard clause
    if (argc != 3) {
        printf("Wrong number of args!\n");
        return 1;
    }

    num = atoi(argv[1]);
    base = atoi(argv[2]);

    // Find the largest base^x that you can go to
    int largest_power = largestpower(base, num);
    
    // Loop through and calculate the number
    for (int i = largest_power; i >= 0; i--) {
        // Subtract current power as many times as possible
        int subCount = 0;
        while (num >= ipow(base, i)) {
            num -= ipow(base, i);
            subCount++;
        }

        // Print out formatted number
        //printf("%d", subCount);
        printdigit(subCount);

        // Repeat for next power
    }

    printf("\n");

    return 0;
}