/***************************************************************************
Name: Harsha V. Kuchampudi
Date: February 13th, 2015
Course ID: COP 3503
Program Title: Magic Square

Program Description: A program that creates an N*N magic square. The user
specifies the size of the square matrix: N. The value N must be an odd
number between 3 and 15.
***************************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;

// Checks if the user's N value abides by the constraints
int isValidEntry(int inputNumber);

// Checks if all rows, columns,and diagonals add to the same number
void matrixCheck(int sizeMatrix, int magicSquare[][100]);

int main(){

    // Base array declaration
    int masterArray[100][100] = {0};

    // Base variables declaration
    int row, column, var_x, var_y;

    // The inital N number the user desired
    int initialNumber;

    // The final N number that is chosen after constraint checks
    int chosenNumber = 0;

    cout << "Enter the size of a magic square: ";
    cin >> initialNumber;
    cout << endl;

    // Checks if user N value is valid and assigns it to 'chosenNumber'
    chosenNumber = isValidEntry(initialNumber);

    /***** Siamese Method Algorithm Start *****/

    var_x = 0;
    var_y = (chosenNumber - 1) / 2;
    masterArray[var_x][var_y] = 1;

    for (row = 2; row <= chosenNumber * chosenNumber; row++){

        var_x--;
        var_y++;

        if (var_x < 0 && var_y == chosenNumber){
            var_x = var_x + 2;
            var_y--;
        }

        if (var_x < 0){
            var_x = chosenNumber - 1;
        }

        if (var_y >= chosenNumber){
            var_y = 0;
        }

        if (masterArray[var_x][var_y] != 0){
            var_x = var_x + 2;
            var_y = var_y - 1;
        }

        masterArray[var_x][var_y] = row;

    }

    /***** Siamese Method Algorithm End *****/
    
    cout << endl;

    cout << "One Magic Square is: " << endl;
    
    // Matrix Output
    for(row = 0;row < chosenNumber;row++){
        for(column = 0; column < chosenNumber; column++){
            cout << setw(5) << masterArray[row][column];
        }
        cout << endl;
    }

    cout << endl;

    // Method call to check if all rows, columns,and diagonals add to the same number
    matrixCheck(chosenNumber, masterArray);

}

int isValidEntry(int inputNumber){

    // Input variable in case N is not valid
    int failNumber;

    // Variable containing a valid N value
    int successNumber = inputNumber;

    // Constraint checking
    if (inputNumber % 2 == 0 || inputNumber < 3 || inputNumber > 15){
        cout << "Please Enter a Valid Number: ";
        cin >> failNumber;
        successNumber = isValidEntry(failNumber);
    }

    // Returns a viable N value
    return successNumber;

}

void matrixCheck(int sizeMatrix, int magicSquare[][100]){

    int rowSum = 0; // Sum of a Row
    int colSum = 0; // Sum of a Column
    int diagonalSum1 = 0; // Sum of Forward Diagonal
    int diagonalSum2 = 0; // Sum of Backward Diagonal

    cout << "Checking the sums of every row: ";

    // Calculates and outputs the sum of every row
    for (int row = 0; row < sizeMatrix; row++){
        for (int column = 0; column < sizeMatrix; column++){
            rowSum = rowSum + magicSquare[row][column];
        }

        cout << " " << rowSum;
        rowSum = 0;
    }

    cout << endl;

    cout << "Checking the sums of every column: ";

    // Calculates and outputs the sum of every column
    for (int column = 0; column < sizeMatrix; column++){
        for (int row = 0; row < sizeMatrix; row++){
            colSum = colSum + magicSquare[row][column];
        }

        cout << " " << colSum;
        colSum = 0;
    }

    cout << endl;

    cout << "Checking the sums of every diagonal: ";

    // Calculates and outputs the sum of the forward diagonal
    for (int diag1 = 0; diag1 < sizeMatrix; diag1++){
        diagonalSum1 = diagonalSum1 + magicSquare[diag1][diag1];
    }

    cout << diagonalSum1 << " ";

    // Calculates and outputs the sum of the backward diagonal
    for (int diag2 = sizeMatrix - 1; diag2 >= 0; diag2--){
        diagonalSum2 = diagonalSum2 + magicSquare[diag2][diag2];
    }

    cout << diagonalSum2;

}
