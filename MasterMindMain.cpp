/*
    Programmer: Cody King
    Program: C++ console-based Master Mind
    Version: 1.3.2
    Permissions: Have fun!(?)
    BUGS: entering a floating point number anywhere causes the program to seizure
    Changes: Swapped a lot of labels out with loops
*/
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
    /*
	Define necessary variables
	When defining the array size, it must be as large as the maximum possible array encountered in the game so that the arrays are adequately spaced in memory
	If this is not done, overlaps will occur between the arrays and mess everything up!
	In the field 2D array, the 1st row is the code (field[1][i]), the 2nd row is the guess (field[2][i]), and the last row is the determiner(field[3][i])
	Combining the 3 arrays into a single 2D array has had no immediate impact on program size
	*/
    int array_size = 10, max_num = 10, twos = 0, counter = 0;
    int field[3][array_size];
    bool hardMode = false;
    char response = '\0';

    // Intro!
    cout << "Master Mind for the Windows Console written in C++\nWritten by Cody King\nVersion: 1.3.1\n";

    // Establish the number of different code pieces
    AGAIN2:
    cout << "\nHow many different code pieces? Must be between 2 and 10 inclusive: ";
    cin >> max_num;
    while (max_num < 2 || max_num > 10){
        cout << "\nThat is an invalid number of code pieces!\nIt must be between 2 and 10 inclusive: ";
    }

    // Establish the number of code pieces per code.
    cout << "\nHow many pieces per code? Must be between 2 and 10 inclusive: ";
    cin >> array_size;
    while (array_size < 2 || array_size > 10){
        cout << "\nThat is an invalid number of pieces per code!\nIt must be between 2 and 10 inclusive: ";
    }

    // Prompt the user if the want to play on HARDMODE!
    cout << "\nPlay on HARDMODE? (Y/N | After every " << array_size << " guesses, a part of the code changes!): ";
    do {
        cin >> response;
        switch (response){
            case 'y':
            case 'Y':
                hardMode = true;
                cout << "\nFantastic! HARDMODE ENGAGED.\n";
                break;
            case 'n':
            case 'N':
                cout << "\nVery well. SOFTMODE ENGAGED.\n";
                break;
            default:
                cout << "\nIt's a Y or N question, its not that hard: ";
        }
    } while(response != 'y' && response != 'Y' && response != 'n' && response != 'N');


    // Generate the code to be cracked!
    AGAIN:
    int seed = time(0); // Must be initialized here for usage with playing again
    srand(seed); // Seed the RNG with the time

    for (int i = 0; i < array_size; i++){
        //cout << i << " <= "; // debugging line **********
        field[1][i] = rand() % max_num;
        //cout << field[1][i] << endl; // debugging line **********
    }

    // Guessing the code
    seed = 0; // reset for use again if playing again
    counter = 0; // reset for use again if playing again
    for (int i = 0; i < array_size; i++) field[3][i] = 0; // reset for use again if playing again
    cout << "\nOkay! Try to guess the code! Press enter after each piece:\n";
    while (seed != array_size){
        counter++;
        cout << "\nGuess number " << counter << ":" << endl;
        seed = 0; // reusing var seed for checking if all the guesses were right
        twos = 0;

        // This for loop inputs the user's guesses into guess and prompts the user if the guess they entered is invalid
        for (int i = 0; i < array_size; i++){
            int test = 0;
            cout << endl;
            cin >> test;
            while (test > (max_num - 1) || test < 0){
                cout << "Invalid piece! Must be 0 to " << (max_num - 1) << " inclusive.\n";
                cin >> test;
            }
            field[2][i] = test;
        }

        // This for loop checks each element in field against its corresponding element in guess
        for (int i = 0; i < array_size; i++){
            //cout << field[1][i] << " " << field[2][i] << endl; // debugging line **********
            if (field[1][i] == field[2][i]) field[3][i] = 1; // If the elements match, it puts a 1 in the same spot in determiner
            else field[3][i] = 0; // If they don't, it puts a 0 there instead
        }

        // These loops check each element in field against every element in guess
        for (int i = 0; i < array_size; i++){
            next:
            if (field[3][i] == 1); // If the element in field has a 1 in its corresponding spot in determiner, this causes that element to be skipped
            else{
                // This loop checks the current element in field to every element in guess
                for (int j = 0; j < array_size; j++){
                    // If the element in guess does not have a 0 in its corresponding spot in determiner, this causes that element to be skipped
                    if (field[3][j] != 0);
                    /*
					If the element in guess is equal to the element in field, this sets a 2 into the corresponding spot of guess in determiner
                    It also then immediately forces the next element of field to be checked; this prevents one element in field being linked to more than
                    one element in guess
					*/
                    else if (field[1][i] == field[2][j]){
                        field[3][j] = 2;
                        i++;
                        goto next;
                    }
                }
            }
        }

        //for (int i = 0; i < array_size; i++) cout << field[3][i] << " "; // debugging line **********

        // This loop counts the number of 1s (indicating correct pieces in the correct spot) in determiner
        for (int i = 0; i < array_size; i++){
            if (field[3][i] == 1) seed++;
        }

        // This loop counts the number of 2s (indicating correct pieces in the wrong spot) in determiner
        for (int i = 0; i < array_size; i++){
            if (field[3][i] == 2) twos++;
        }

        cout << endl << seed << " correct piece(s) in the correct spot.\n" << twos << " correct piece(s) in the wrong spot.\n";

        // These conditionals randomly changes one of the pieces of code after every fifth guess has been tested provided HARDMODE is ENGAGED!
        if (hardMode){
            if (seed != array_size){
                if (counter % array_size == 0){
                    int randomNum = 0;
                    randomNum = rand() % array_size;
                    field[1][randomNum] = rand() % max_num;
                    cout << "\nHeads up! A piece might have changed!\n";
                    //cout << (randomNum + 1) << " <= " << field[1][randomNum]; // debugging line **********
                }
            }
        }
    }
    // Congratulate the player for succeeding
    cout << "\nCongratulations! [ ";
    for (int i = 0; i < array_size; i++) cout << field[2][i] << " ";
    cout << "] is correct!\nYou succeeded in " << counter << " guess(es)!\n";
    //for (int i = 0; i < array_size; i++) cout << field[1][i] << " "; // debugging line **********

    // Prompt the user if they want to play again
    char ans;
    cout << "\n\nPlay again? (Y/N): ";
    do {
        cin >> ans;
        switch (ans){
            case 'y':
            case 'Y':
                do {
                    cout << "\nWith the same settings or different ones? (S/D): ";
                    cin >> ans;
                    switch (ans){
                        case 's':
                        case 'S':
                            goto AGAIN;
                            break;
                        case 'd':
                        case 'D':
                            goto AGAIN2;
                            break;
                        default:
                            cout << "\nThat is not a valid response.\n";
                    }
                } while(ans != 's' && ans != 'S' && ans != 'd' && ans != 'D');
                break;
            case 'n':
            case 'N':
                cout << "\nPress Enter to Exit";
                cin.ignore();
                cin.get();
                exit(0);
            default:
                cout << "\nThat is not a valid response.\n";
            }

    } while (ans != 'n' && ans != 'N' && ans != 'y' && ans != 'Y');
    return 0;
}
