/* 
 * minesweeper.cpp 
 *
 * A text-based minesweeper game that runs in the terminal.
 *
 * COMP11 Fall 2018
 * Modified by: Sam Liu 
 * Modified on: 10/29/18
 *
 *
 * compile with clang++ -Wall -Wextra minesweeper.cpp -o minesweeper
 *
 *
 */

#include <iostream>
#include <time.h> 
#include <cstdlib>

using namespace std;

/* Set random seed*/
#ifdef TESTING
#define RANDOMSEED 1
#else
#define RANDOMSEED time(NULL)
#endif


const int side = 9; //Side dimension of the square board
const int num_cells = 81; //Number of cells on a 9x9 board

void initialize_board(char my_board[side][side], char user_board[side][side]);
int get_input(string prompt);
void print_board(char board[side][side]);
int plant_mines(char my_board[side][side]);
bool is_a_cell(int row, int col);
bool is_a_mine(int row, int col, char my_board[side][side]);
bool mine_in_vicinity(int row, int col, char my_board[side][side]);
int mine_count(int row, int col, char my_board[side][side]);
bool play_game(char my_board[side][side], char user_board[side][side]);

int main() {
    srand(RANDOMSEED); 

    char user_board[side][side];
    char my_board[side][side];

    initialize_board(my_board, user_board);
    
    int num_mines;
    num_mines = plant_mines(my_board); //Will plant and count num mines
    
    int num_possible_plays = num_cells - num_mines; //Num of possible plays

    bool x;    
    int play_count = 0;

    //Will play until false
    do {
            if (play_count < num_possible_plays) {
                    x = play_game(my_board, user_board);
                    play_count++;
            } else if (play_count == num_possible_plays) {
                    x = false;
                    cout << "GAME OVER. YOU WON!" << endl;
            }
    } while (x == true);

    return 0;
}

//This function will initialize both boards with '-'
//It takes in two 2D arrays, the user and solution board
void initialize_board(char my_board[side][side], char user_board[side][side])
{
        for (int i = 0; i < side; i++) {
                for (int j = 0; j < side; j++) {
                        user_board[i][j] = '-';
                        my_board[i][j] = '-';
                }
        }
}

//Will print any board when called. Used to print user board
//after every play
void print_board(char board[side][side])
{
        
        cout << "  " << "0 1 2 3 4 5 6 7 8"; 
        for (int i = 0; i < side; i++) {
                cout << endl;
                cout << i;
                for (int j = 0; j < side; j++) {
                        cout << " ";
                        cout << board[i][j];
                }
        }
        cout << endl;
}

//Randomly assigns cells a mine with a .3 probability
//to the solution board
int plant_mines(char my_board[side][side])
{
        srand(time(NULL));
        int num_mines = 0;

        for (int i = 0; i < side; i++) {
                for (int j = 0; j < side; j++) {
                        float mine_prob = rand () / (RAND_MAX + 1.0);
                        if (mine_prob <= 0.3) {
                                if (my_board[i][j] != 'x') {
                                        my_board [i][j] = 'x';
                                        num_mines++; //Counts num mines
                                }
                        }
                }
        }
        return num_mines;
}

//Checks to see if the user input of row and col is a real cell on the board
bool is_a_cell(int row, int col) 
{
        if (row >= 0 and row < side and col >= 0 and col < 9) {
                return true;
        } else {
                return false;
        }
        return false;
}

//Checks to see if the user input of row and col produces a cell that is a mine
bool is_a_mine(int row, int col, char my_board[side][side])
{
        if (my_board[row][col] == 'x') {
                return true;
        } else {
                return false;
        }
}

//Counts how many mines are int the vicinity. Takes the position
//of the surrounding cells in relation to the given cell.
//Returns the int number of cells in the vicinity.
int mine_count(int row, int col, char my_board[side][side])
{
        int mine_count = 0;
     
        if (mine_in_vicinity(row - 1, col, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row + 1, col, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row, col -1, my_board)  == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row, col + 1, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row - 1, col - 1, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row -1, col + 1, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row + 1, col -1, my_board) == true) {
                mine_count++;
        }
        if (mine_in_vicinity(row + 1, col + 1, my_board) == true) {
                mine_count++;
        }
        return mine_count++;
}
    
//Will check if there is a mine nearby by seeing if 1.) its a real cell
//2.) if that cell is a mine. This a helper function to mine_count to
//to make it more modular.
bool mine_in_vicinity(int row, int col, char my_board[side][side])
{
        if (is_a_cell(row, col) == true) { 
                if (is_a_mine(row, col, my_board) == true) {
                        return true;
                }
        } else { 
                return false;
        }
        return false;
}                                                

//Stitches the functions together. Asks for user input, and compares user input.
//Replaces the cell of a given row and col with a number.
//Called by main inside a while loop to keep game running until user hits a mine
//or clears the board.
bool play_game(char my_board[side][side], char user_board[side][side])
{
     
        int row, col;

        cout << "Enter the row and column of the square to uncover: ";
        cin >> row >> col;
        cout << endl;

        if (is_a_cell(row, col) == false) {
                cout << "Bad input. Row and col must be >= 0 and < 9." << endl;
                return true;
        } else if (my_board[row][col] != 'x' and my_board[row][col] != '-') {
                //Square is already uncovered
                cout << "Bad input. Select another square." << endl;
                return true;
        } else if (my_board[row][col] != 'x') {
                int num_mines = mine_count(row, col, my_board);
                user_board[row][col] = num_mines + '0'; //Turns int into char
                my_board[row][col] = num_mines + '0';//Updates solution board
                print_board(user_board); //used for checks
                return true;
        } else if (my_board[row][col] == 'x') { //Hits a mine
                        user_board[row][col] = 'x';
                        print_board(my_board);
                        cout << "GAME OVER. YOU LOST!" << endl;
                        return false;
        }
        return false;
}
