#include "Board.cpp"
#include <fstream>
#include <time.h>

class Solver {
public:
    Solver(Board &b) {
        board = new Board(b);
    }

    ~Solver() {
        delete board;
    }

    void print() {
        board->print_board();
    }

    void print_transpose() {
        board->print_board_transpose();
    }

    bool solve_grid() {
        int row = -1;
        int col = -1;
        bool found_unassigned_loc = find_unassinged_loc(row, col);
        if (!found_unassigned_loc) {
            return true;
        }
        for (int num = 1; num <= 9; num++) {
            board->grid[row][col] = num;
            if (is_safe(row, col)) {
                if (solve_grid()) {
                    return true;
                }
                board->grid[row][col] = UNASSIGNED;
            }
            else {
                board->grid[row][col] = UNASSIGNED;
            }
        }
        return false;
    }

    void make_puzzle(string difficulty) {
        int num_to_erase = 45;
        if (difficulty == "medium") {
            num_to_erase = 60;
        }
        if (difficulty == "hard") {
            num_to_erase = 80;
        }
        for (int i = 0; i < num_to_erase; i++) {
            int row = rand() % 9;
            int col = rand() % 9;
            board->grid[row][col] = UNASSIGNED;
        }
    }

private:
    Board* board;

    int floor(int x) {
        int rem = x % 3;
        return x - rem;
    }

    bool is_safe(int row, int col) {
        bool rows = board->check_row_dup(row);
        bool cols = board->check_col_dup(col);
        bool grid = board->check_three_grid_dup(floor(row), floor(col));
        return rows && cols && grid;
    }

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

void read_input(int input_puzzle[9][9]) {
    ifstream input("input.txt");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            input >> input_puzzle[i][j];
        }
    }
    input.close();
}

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
    srand(time(NULL));
    if (intention == "solve") {
        intent_solve(solve);
        return 0;
    }
    
    intent_generate(solve);
    return 0;
}