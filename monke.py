import random as random
from random import randint
import sys

monkey_names = [
    "Dart Monkey",
    "Boomerang Monkey",
    "Bomb Shooter",
    "Tack Shooter",
    "Ice Monkey",
    "Glue Gunner",
    "Sniper Monkey",
    "Monkey Sub",
    "Monkey Buccaneer",
    "Monkey Ace",
    "Heli Pilot",
    "Mortar Monkey",
    "Dartling Gunner",
    "Wizard Monkey",
    "Super Monkey",
    "Ninja Monkey",
    "Alchemist",
    "Druid",
    "Monkey Village",
    "Spike Factory",
    "Engineer",
    "Beast Handler",
    "Mermonkey",
    "Banana Farm"
]

# Paths that are money making and thus invalid in chimps
invalid = [8, 16, 18]

paths = [
    "Top Path",
    "Middle Path",
    "Bottom Path",
]

def main():
    # Command line args
    chimps = False
    if ("-c" in sys.argv) or ("-C" in sys.argv):
        chimps = True

    minimum = 1
    if ("-m" in sys.argv) or ("-min" in sys.argv) or ("--min" in sys.argv):
        minimum = 3

    print("Welcome to monke picker, press any key for monke. To run in chimps mode or with a tier 3 minimum run with [-c/-m] respectively")
    print(f"Chimps mode: {chimps}\nMinimum: {minimum}")

    # Main loop
    while True:
        inputt = input()

        if inputt not in ["stop", "exit", "quit", "q"]:
            if not chimps:
                monkey = randint(0, len(monkey_names) - 1)
                upgrade = randint(0, 2)
                tier = randint(minimum, 5)
            else:
                monkey = randint(0, len(monkey_names) - 2)
                upgrade = randint(0, 2)
                tier = randint(minimum, 5)

                # Restrict paths for invalid towers
                if monkey in invalid:
                    upgrade %= 2


            print("--------------------")
            print(monkey_names[monkey])
            print(paths[upgrade])
            print(f"Tier: {tier}")
            print("--------------------\n")
        else:
            print("Exiting...")
            break;



if __name__ == "__main__":
    main()

#Rules
#You must get the specified monkey at least before getting a new monkey
#You can choose whatever crosspath
#You can't go past the specified tier
#If an upgrade costs over 30k, you don't have to get it
