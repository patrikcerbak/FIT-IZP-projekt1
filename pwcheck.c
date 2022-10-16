/***************************************************
*
*   Name:       Project 1 - Password Check 
*   Author:     Patrik Cerbak - xcerba00
*   Date:       10/2021
*
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXLENGTH 100 // maximum length of a password
#define ASCIICHARS 128 // there are 128 characters in the ascii table


// structure for saving program arguments
typedef struct {
    int minLevel; // minLevel is for the LEVEL argument, I chose this name since it is the minimal security level to show
    int param;
    int showStats;
    int error;
} Arguments;


// function for saving input to an "Arguments" structure
Arguments getArguments(int argc, char arg1[], char arg2[], char arg3[]) {
    Arguments args;
    args.error = 0; // default value

    if(argc < 3 || argc > 4) { // check how many arguments are there
        fprintf(stderr, "Error: Spatny pocet argumentu.\n"); // since the "stats" section needs to be in Czech, I chose to do the error messages in Czech as well
        args.error = 1;
    }

    // check if every character of the first argument is a number
    int i = 0;
    while(args.error == 0 && arg1[i] != '\0') {
        if(arg1[i] >= '0' && arg1[i] <= '9') {
            i++;
        } else {
            fprintf(stderr, "Error: Argument LEVEL obsahuje neciselny znak.\n");
            args.error = 1;
            break;
        }
    }

    // check if every character of the second argument is a number
    int j = 0;
    while(args.error == 0 && arg2[j] != '\0') {
        if(arg2[j] >= '0' && arg2[j] <= '9') {
            j++;
        } else {
            fprintf(stderr, "Error: Argument PARAM obsahuje neciselny znak.\n");
            args.error = 1;
            break;
        }
    }

    // save the arguments into their variables
    if(args.error == 0) {
        args.minLevel = atoi(arg1);
        args.param = atoi(arg2);
    } else {
        args.minLevel = 0;
        args.param = 0;
    }

    if(args.error == 0) {
        // check if the first two arguments are within their range
        if(args.minLevel < 1 || args.minLevel > 4) {
            fprintf(stderr, "Error: Argument LEVEL neni v intervalu [1,4].\n");
            args.error = 1;
        }

        if(args.param < 1 || args.param > INT_MAX) {
            fprintf(stderr, "Error: Argument PARAM je mimo platny rozsah.\n");
            args.error = 1;
        }
    }

    // check if the --stats argument is valid
    if(argc != 4 || args.error == 1) {
        args.showStats = 0; // there is no third argument, return 0
    } else {
        char stats[] = "--stats"; // declare string to compare the third argument with
        int k = 0;
        while(arg3[k] != '\0') {
            if(arg3[k] == stats[k] && arg3[k + 1] == stats[k + 1]) { // k + 1 is there, because the \0 character needs to be on same spot in both strings
                args.showStats = 1;
            } else {
                args.showStats = 0;
                args.error = 1;
                fprintf(stderr, "Error: Treti argument je ve spatnem tvaru.\n");
                break;
            }

            k++;
        }
    }

    return args;
}


// get password length
int getLength(char password[]) {
    int i = 0;
    while(password[i] != '\n' && password[i] != '\0') { // check for both \n and \0, because if fgets gets longer line than specified, it does not save the \n into the variable, just \0
        i++;
    }

    return i; // i is the length of the password
}


// check passwords for level 1
int levelOne(char password[]) {
    // declare variables for capital and lowercase
    int hasCapital = 0;
    int hasLowercase = 0;

    for(int i = 0; i < MAXLENGTH; i++) { // check for every character in a password
        if(password[i] == '\n') { // if it sees newline, stop checking - end of password
            break;
        } else if(password[i] >= 'A' && password[i] <= 'Z') {
            hasCapital = 1;
        } else if(password[i] >= 'a' && password[i] <= 'z') {
            hasLowercase = 1;
        }
    }

    if(hasCapital == 1 && hasLowercase == 1) {
        return 1; // if it has both capital and lowercase letter - return 1
    } else {
        return 0; // else return 0
    }
}


// check passwords for level 2
int levelTwo(char password[], int param) {
    // declare variables for each category
    int hasCapital = 0;
    int hasLowercase = 0;
    int hasNumber = 0;
    int hasSpecial = 0;

    if(param > 4) { // if param is larger than 4, check for every category (param = 4)
        param = 4;
    }

    for(int i = 0; i < MAXLENGTH; i++) { // checks similary as in levelOne, but for more caregories
        if(password[i] == '\n') {
            break;
        } else if(password[i] >= 'A' && password[i] <= 'Z') {
            hasCapital = 1;
        } else if(password[i] >= 'a' && password[i] <= 'z') {
            hasLowercase = 1;
        } else if(password[i] >= '0' && password[i] <= '9') {
            hasNumber = 1;
        } if((password[i] >= ' ' && password[i] <= '/') || (password[i] >= ':' && password[i] <= '@') || (password[i] >= '[' && password[i] <= '`') || (password[i] >= '{' && password[i] <= '~')) { // checks for every special character that was specified in assignment
            hasSpecial = 1;
        }
    }

    if((hasCapital + hasLowercase + hasNumber + hasSpecial) >= param) {
        return 1; // if the password has characters from at least X categories, return 1
    } else {
        return 0; // else return 0
    }
}


// check passwords for level 3
int levelThree(char password[], int param) {
    int sameChars; // variable for saving the number of same characters in a row

    for(int i = 0; i < MAXLENGTH; i++) { // first loop is for determining the position of a "starting character" for checking
        if(password[i] == '\n') { // check if we are still within the password and not outside
            break;
        } else {
            sameChars = 0;

            for(int j = 0; j < param; j++) { // this loop is for the checking itself
                if(password [i + j] == '\n') {
                    break;
                } else if(password[i] == password[i + j]) { // see if the "starting character" is the same as the character on "starting character's" position + "j" positions
                    sameChars++; // if so, increase this variable
                } else {
                    sameChars = 0; // if not, reset this variable
                    break; // and break the second loop
                }
            }

            if(sameChars == param) { // if true, there are at least X characters in a row, the password does not pass level 3
                break;
            }
        }
    }

    if(sameChars == param) { // check if the password passes level 3
        return 0;
    } else {
        return 1;
    }
}


// check passwords for level 4
int levelFour(char password[], int param, int length) {
    int sameSubChars; // this variable is for counting how many characters are same in a substring and another part of the password
    int sameSubStrings; // this counts how many same substrings are there in the password
    char subString[length];

    for(int i = param; i < length; i++) { // determine the length of the substring
        for(int j = 0; j < length; j++) { // determine the "starting character" of the substring
            if(password[j + i - 1] == '\n') { // check if we can even take substring from password[]
                break;
            } else {
                for(int k = 0; k < i; k++) { // this loop is for saving the substring into subString variable
                    subString[k] = password[j + k];
                    subString[k + 1] = '\0'; // we need to end our substring with \0
                }

                sameSubStrings = 0; // reset varaible

                for(int l = 0; l < length; l++) { // determines the "starting character" of another substring to compare
                    sameSubChars = 0; // reset variable

                    for(int m = 0; m < i; m++) { // this loop is for comparing subString with another substring within the password
                        if(password[l + m] == subString[m]) {
                            sameSubChars++; // if the characters match, add 1

                            if(sameSubChars == i) { // if there are enough same chars, then add to sameSubChars
                                sameSubStrings++;
                            }
                        } else {
                            sameSubChars = 0; // if the characters dont match, reset
                        }

                        // check if there are same substrings in the password
                        if(sameSubStrings == 2) { // there is 2, because the original subString that we compare is there always
                            l = length; //
                            j = length; // we found a match, break every loop
                            i = length; //
                            break;
                        }
                    }
                }
            }
        }
    }

    if(sameSubStrings == 2) { // same check as above
        return 0; // it does not pass level 4
    } else {
        return 1; // it passes level 4
    }

}


// function that counts ones in an ascii table array
int countDifferentChars(int asciiTable[]) {
    int count = 0;
    for(int i = 0; i < ASCIICHARS; i++) {
        if(asciiTable[i] == 1) {
            count++;
        }
    }
    return count;
}


// counts the average from sum and number of passwords
double getAverage(int sum, int pwdNumber) {
    return (double)sum / (double)pwdNumber;
}


int main(int argc, char *argv[]) {
    char password[MAXLENGTH + 2]; // MAXLENGTH + \n + \0
    int length; // variable for the length of a password
    int level; // a variable to store the security level of the current password

    Arguments args = getArguments(argc, argv[1], argv[2], argv[3]); // get and store arguments
    int err = args.error; // a variable for storing whether or not there is an error, it has the value from "args.error" for now

    // variables for stats
    int asciiTable[ASCIICHARS + 1] = {0}; // creates a table with same amount of characters as ASCII table (+ \0) and fills it with zeros
    int minLength = MAXLENGTH; // needs to start with the biggest possible number
    int sum = 0;
    int pwdNumber = 0;

    while((fgets(password, MAXLENGTH + 2, stdin) != NULL) && err == 0) { // MAXLENGTH + 2, since the fgets function also saves the \n character
        length = getLength(password);

        if(length > MAXLENGTH) { // checks if the password is too long
            err = 1; // update error variable
            fprintf(stderr, "Error: Program narazil na heslo delsi nez %d znaku.\n", MAXLENGTH);
            break;
        }

        // checks for every security level
        level = 0;
        if(levelOne(password)) {
            level++;
        }
        if(level == 1 && levelTwo(password, args.param)) {
            level++;
        }
        if(level == 2 && levelThree(password, args.param)) {
            level++;
        }
        if(level == 3 && levelFour(password, args.param, length)) {
            level++;
        }

        // and prints the passwords, that are satisfying are conditions
        if(level >= args.minLevel) {
            printf("%s", password);
        }

        // the aim of this loop is to save a 1 into the asciiTable[] for each unique character in a password
        for(int k = 0; k < MAXLENGTH; k++) {
            if(password[k] == '\n') {
                break;
            } else {
                asciiTable[(int)password[k]] = 1; // save 1 into asciiTable[int value of char]
            }
        }

        // check if the password is shorter than minLength and update the variable
        if(length < minLength) {
            minLength = length;
        }

        // update variables for the average length calculation
        sum = sum + length;
        pwdNumber++;
    }

    // a section for showing stats
    if(args.showStats == 1 && err == 0) {
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", countDifferentChars(asciiTable));
        printf("Minimalni delka: %d\n", minLength);
        printf("Prumerna delka: %.1lf\n", getAverage(sum, pwdNumber));
    }

    // returns 0 if there are no errors, otherwise returns 1
    if(err == 0) {
        return 0;
    } else {
        return 1;
    }
}
