#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 10/6/2022
// Parker Lowney and Natalie Norris

// Number progression:
// 1-9 (< 10)
// a-z (< 37)
// A-Z (< 63)
// |aa| - |zz| (< 89)
// |AA| - |ZZ| (< 115)
// |aaa| - |zzz|
// |AAA| - |ZZZ|
// ...


// Dumbass compiler thinks pow doesn't exist
int ipow(int base, int power) {
    for (int i = 0; i < power; i++) {
        base *= base;
    }

    return base;
}

int largestpower(int basey, int numby) {
    // start at 1 and go up until you reach a 'can't do this', then return powers
    int power = 1;
    while (numby >= ipow(basey, power)) {
        power++;
    }
    return power;
}

char* digittostring(int numb) {
    char temp[50];
    int ticky = 0;  // ticky keeps track of how many characters we are putting into the array

    if (numb < 10) {
        temp[0] = (numb + '0');
        
    } else if (numb < 37) {
        temp[0] = (numb + 'W');  // W is 87, so ex 10+87 = 97 which is ascii for a, 10=a

    } else if (numb < 63) {
        temp[0] = (char) (numb + 28);
    }

    char* final = malloc(ticky + 1); // Create a pointer to a fitted array
    final[ticky] = '\0';
    strcpy(final, temp);

    return final; // REMEMBER TO FREE MEMORY
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


    // Array of pointers to strings for each char, so that multi char digits don't mess up total
    // size count
    char* output[largest_power];
    
    // Loop through and calculate the number
    for (int i = largest_power; i > 0; i--) {
        // Subtract current power as many times as possible
        int subCount = 0;
        while (num >= ipow(base, i)) {
            num -= ipow(base, i);
            subCount++;
        }

        // Put that number into output string
        output[largest_power - i] = digittostring(subCount);

        // Repeat for next power
    }

    // Print out number
    for (int i = 0; i < largest_power; i++) {
        printf("%s", output[i]);
    }

    printf("\n");
    

    // Loop to free memory
    for (int i = 0; i < largest_power; i++) {
        free(output[i]);
    }

    return 0;
}