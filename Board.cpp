#define underline "\033[4m"
#define nounderline "\033[0m"
#include <array>
#include <map>
#include <iostream>

using namespace std;

const int UNASSIGNED = 0;
const int NUM_ROWS = 9;
const int NUM_COLS = 9;


// for color
namespace Color {
    enum Code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_PURPLE = 35,
        FG_BLACK = 30,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49,
        BG_WHITE = 47,
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
            operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

Color::Modifier red(Color::FG_RED);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier black(Color::FG_BLACK);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier yellow(Color::FG_YELLOW);
Color::Modifier bg_white(Color::BG_WHITE);
Color::Modifier bg_def(Color::BG_DEFAULT);
Color::Modifier purple(Color::FG_PURPLE);



class Board {
    friend class Solver;
public:
    Board();
    Board(int input[9][9]);
    void print_board();
    void print_board_transpose();
private:
    int grid[9][9];
    void print_helper_underline(int i, int j, bool trans);
    void print_helper_nounderline(int i, int j, bool trans);
    bool check_three_grid_dup(int row_from, int col_from);
    bool check_row_dup(int row);
    bool check_col_dup(int col);  
};

Board::Board() {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            grid[i][j] = UNASSIGNED;
        }
    }
}

Board::Board(int input[9][9]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            grid[i][j] = input[i][j];
        }
    }
}

void Board::print_helper_nounderline(int i, int j, bool trans) {
    int num = j;
    if (trans) {
        num = i;
    }
    if (grid[i][j] == UNASSIGNED) {
        cout <<bg_white<< red << "-" << def << bg_def;
    }
    else {
        cout <<black << bg_white << grid[i][j] << def << bg_def;
    }
    if (num % 3 == 2) {
        cout <<black << bg_white<< "|" <<def << bg_def;
        return;
    }
    cout <<bg_white <<black<< " " << bg_def <<def;
}

void Board::print_helper_underline(int i, int j, bool trans) {
    int num = j;
    if (trans) {
        num = i;
    }
    if (grid[i][j] == UNASSIGNED) {
        cout << bg_white << red << underline << "-" << nounderline << def << bg_def;
    }
    else {
        cout <<black << bg_white<< underline << grid[i][j] << nounderline << def << bg_def;
    }
    if (num % 3 == 2) {
        cout <<black<<bg_white<< underline << "|" << nounderline << def << bg_def;
        return;
    }
    cout <<bg_white<<black<< underline << " " << nounderline <<def << bg_def;
}

void Board::print_board() {
    bool trans = false;
    cout << bg_white << "  " << bg_def;
    cout << bg_white<< black << underline << "                   " << nounderline << def << bg_def;
    cout << bg_white << "  " << bg_def << endl;
    for (int i = 0; i < NUM_ROWS; i++) {
        cout << bg_white << "  " << bg_def;
        bool under = (i % 3 == 2);
        if (under) {
            cout <<black << bg_white << underline << "|" << nounderline << def << bg_def;
        }
        else {
            cout <<black << bg_white<< "|" << def << bg_def;
        }
        for (int j = 0; j < NUM_COLS; j++) {
            if (under) {
                print_helper_underline(i, j, trans);
            }
            else {
                print_helper_nounderline(i, j, trans);
            }
        }
        cout << bg_white << "  " << bg_def;
        cout << endl;
    }
    cout << bg_white << "  " << bg_def;
    cout << bg_white << "                   " << bg_def;
    cout << bg_white << "  " << bg_def << endl;
}

void Board::print_board_transpose() {
    bool trans = true;
    cout << bg_white << "  " << bg_def;
    cout << bg_white << black << underline << "                   " << nounderline << def << bg_def;
    cout << bg_white << "  " << bg_def << endl;
    for (int i = 0; i < NUM_ROWS; i++) {
        cout << bg_white << "  " << bg_def;
        bool under = (i % 3 == 2);
        if (under) {
            cout <<bg_white <<black << underline << "|" << nounderline << def << bg_def;
        }
        else {
            cout <<black << bg_white<< "|" << def << bg_def;
        }
        for (int j = 0; j < NUM_COLS; j++) {
            if (under) {
                print_helper_underline(j, i, trans);
            }
            else {
                print_helper_nounderline(j, i, trans);
            }
        }
        cout << bg_white << "  " << bg_def;
        cout << endl;
    }
    cout << bg_white << "  " << bg_def;
    cout << bg_white << black << underline << "                   " << nounderline << def << bg_def;
    cout << bg_white << "  " << bg_def << endl;
}

bool Board::check_three_grid_dup(int row_from, int col_from) {
    map<int, int> nums_count;
    for (int i = row_from; i < row_from + 3; i++) {
        for (int j = col_from; j < col_from + 3; j++) {
            if (grid[i][j] == UNASSIGNED) {
                continue;
            }
            nums_count[grid[i][j]]++;
        }
    }

    for (auto item : nums_count) {
        if (item.second > 1) {
            return false;
        }
    }
    return true;
}

bool Board::check_row_dup(int row) {
    map<int, int> nums_count;
    for (int j = 0; j < NUM_COLS; j++) {
        if (grid[row][j] == UNASSIGNED) {
            continue;
        }
        nums_count[grid[row][j]]++;
    }
    for (auto item : nums_count) {
        if (item.second > 1) {
            return false;
        }
    }
    return true;
}

bool Board::check_col_dup(int col) {
    map<int, int> nums_count;
    for (int i = 0; i < NUM_ROWS; i++) {
        if (grid[i][col] == UNASSIGNED) {
            continue;
        }
        nums_count[grid[i][col]]++;
    }
    for (auto item : nums_count) {
        if (item.second > 1) {
            return false;
        }
    }
    return true;
}