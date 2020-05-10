#include "Board.cpp"
#include <fstream>
#include <time.h>

//Class to solve the given Sudoku puzzle 
//and generate a new one 
class Solver {
public:
    //
    Solver(Board &b) {
        board = new Board(b);
    }
    //destructor to delete dynamically allocated board;
    ~Solver() {
        delete board;
    }
    //prints the board
    void print() {
        board->print_board();
    }
    //prints the transpose of the board
    //used while generating a new board
    void print_transpose() {
        board->print_board_transpose();
    }

    //recursively solves the Sudoku puzzle
    bool solve_grid() {
        int row = -1;
        int col = -1;
        bool found_unassigned_loc = find_unassinged_loc(row, col);
        //if there are no unassigned locations then the board has been solved
        if (!found_unassigned_loc) {
            return true;
        }
        //for each number in an unassinged location,
        //we check if that assignment leads to a solved board 
        for (int num = 1; num <= 9; num++) {
            //make a tentative assignment
            board->grid[row][col] = num;
            //if the assignment is safe then we move on to the next unassigned element
            if (is_safe(row, col)) {
                //call solve_grid() to see if this assignment leads to a solved board 
                if (solve_grid()) {
                    return true;
                }
                //if assignment doesn't lead to a solved board then we revert the element
                //to unassigned and try the next number
                board->grid[row][col] = UNASSIGNED;
            }
            //if the assignment isn't safe then we make it unassigned again
            //and try the next number
            else {
                board->grid[row][col] = UNASSIGNED;
            }
        }
        //all numbers have been tried and none led to a solved board 
        return false;
    }

    //creates a puzzle based on difficulty selected by the user
    //by removing a certain number of elements from the solved board
    void make_puzzle(string difficulty) {
        int num_to_erase = 45;
        if (difficulty == "medium") {
            num_to_erase = 60;
        }
        if (difficulty == "hard") {
            num_to_erase = 80;
        }
        for (int i = 0; i < num_to_erase; i++) {
            //generates two random numbers between 0 and 9
            //which will be the coordinates of the number that will be removed
            int row = rand() % 9;
            int col = rand() % 9;
            board->grid[row][col] = UNASSIGNED;
        }
    }

private:
    Board* board;

    //returns the largest multiple of three less than x
    int floor(int x) {
        int rem = x % 3;
        return x - rem;
    }

    //checks if the board is safe 
    //ensures there are no duplicates in the given row, column, or 3x3 grid
    //that the element at row, col is in 
    bool is_safe(int row, int col) {
        bool rows = board->check_row_dup(row);
        bool cols = board->check_col_dup(col);
        bool grid = board->check_three_grid_dup(floor(row), floor(col));
        return rows && cols && grid;
    }

    //finds the next unassigned location in the board
    bool find_unassinged_loc(int& row, int& col) {
        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {
                if (board->grid[i][j] == UNASSIGNED) {
                    row = i;
                    col = j;
                    return true;
                }
            }
        }
        return false;
    }
};

//reads input from a .txt file
void read_input(int input_puzzle[9][9]) {
    ifstream input("input.txt");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            input >> input_puzzle[i][j];
        }
    }
    input.close();
}

//asks user whether they want to solve a puzzle, generate a puzzle, or exit
string user_intention() {
    string intention = "";
    cout << purple << bg_white << "Welcome to the Sudoku Solver!" << bg_def << endl;
    cout << bg_white << "We can solve a puzzle or generate one for you!" << bg_def << endl;
    cout << bg_white << "Would you like to [solve], [generate], or [exit]?" << def << bg_def << endl;

    while (cin >> intention) {
        if (intention == "solve" || intention == "generate" || intention == "exit") {
            return intention;
        }
        cout << bg_white << red << "Something went wrong :(" << def << bg_def << endl;
        cout << bg_white << purple << "Please type [solve],[generate], or [exit]" << def << bg_def << endl;
    }
    return "exit";
}

//solves the puzzle after printing relevant messages
void intent_solve(Solver& solve) {
    cout << bg_white << purple << "This is the puzzle:    " << bg_def << def << endl;
    solve.print();
    if (solve.solve_grid()) {
        cout << endl << bg_white << purple << "Here's the solution!   " << bg_def << def << endl;
        solve.print();
    }
    else {
        cout << bg_white << purple << "Hmm there doesn't seem to be a solution to this." << bg_def << def << endl;
    }
}

//generates the puzzle after printing relevant messages
//first solves the given input and then removes a number of elements depending on difficulty selected by user
//Then prints the transpose of that board, giving the user a new puzzle
void intent_generate(Solver& solve) {
    cout << bg_white << purple << "Please select a difficulty level" << bg_def << endl;
    cout << bg_white << "Easy  " << bg_def << endl;
    cout << bg_white << "Medium" << bg_def << endl;
    cout << bg_white << "Hard  " << bg_def << def <<endl;
    string difficulty;
    
    while (cin >> difficulty) {
        for (int i = 0; i < (int)difficulty.length(); i++) {
            difficulty[i] = tolower(difficulty[i]);
        }
        if (difficulty == "easy" || difficulty == "medium" || difficulty == "hard") {
            break;
        }
        cout << bg_white << red << "Invalid input" << def << bg_def << endl;
        cout << bg_white << purple << "[1] Easy  " << bg_def << endl;
        cout << bg_white << "[2] Medium" << bg_def << endl;
        cout << bg_white << "[3] Hard  " << bg_def << endl;
        cout << bg_white << "Please select 1, 2, or 3" << bg_def << def << endl;
    }
    solve.solve_grid();
    solve.make_puzzle(difficulty);
    cout << bg_white << blue << "Try solving this!      " << bg_def << def << endl;
    solve.print_transpose();
}

int main() {
    int input_puzzle[9][9];
    read_input(input_puzzle);

    Board b1(input_puzzle);
    Solver solve(b1);

    string intention = user_intention();

    if (intention == "exit") {
        return 0;
    }
    
    cout << endl;
    //ensures a different puzzle is generated in consecutive runs of the program
    srand(time(NULL));
    if (intention == "solve") {
        intent_solve(solve);
        return 0;
    }
    
    intent_generate(solve);
    return 0;
}