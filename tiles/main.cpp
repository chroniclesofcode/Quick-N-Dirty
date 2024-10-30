#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <unistd.h>

/*
    extensions: quick drop, drop-faster as times goes on, abilities??? (think about)
    every tick, block drops down 1. If block hits something, do calculations for
    row destruction

    inside Block::variants
    [0][1]
    [2][3]
*/



class Tiles {

    enum Color {
        EMPTY, RED, GREEN, BLUE, BLOCK
    };

    class Block {
    public:
        static std::vector<std::vector<bool>> variants;
        Block() {}
        Block(unsigned int t, unsigned int c) : type{t}, col{static_cast<Color>(c)},  grid{variants[t]} {}
        unsigned int type;
        Color col;
        std::vector<bool> grid;
    };

public:
    Tiles(int sn, int sm) : n{sn}, m{sm}, bx{0}, by{0} {
        board = std::vector<std::vector<Color>>(n, std::vector<Color>(m, Color::EMPTY));

        // Init random blocks
        rng = std::mt19937(dev());
        dist = std::uniform_int_distribution<std::mt19937::result_type>(0, Block::variants.size() - 1);
        dist_col = std::uniform_int_distribution<std::mt19937::result_type>(1, 3);
        currb = generate_block();
    }

    void start() {
        int SLEEP_MS = 1000;
        draw_board();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        while (!game_end) {
            get_input();
            drop();
            draw_board();
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        }
    }
private:

    int kbhit(void) {
        // timeout structure passed into select
        struct timeval tv;
        // fd_set passed into select
        fd_set fds;
        // Set up the timeout.  here we can wait for 1 second
        tv.tv_sec = 0;
        tv.tv_usec = 5;

        // Zero out the fd_set - make sure it's pristine
        FD_ZERO(&fds);
        // Set the FD that we want to read
        FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
        // select takes the last file descriptor value + 1 in the fdset to check,
        // the fdset for reads, writes, and errors.  We are only passing in reads.
        // the last parameter is the timeout.  select will return if an FD is ready or 
        // the timeout has occurred
        select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
        // return 0 if STDIN is not ready to be read.
        return FD_ISSET(STDIN_FILENO, &fds);
    }

    bool validate_move(int x, int y, const std::vector<bool> &orient) {
        if (!(x >= 0 && x < n && y >= 0 && y < m-1)) 
            return false;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (orient[2*i+j] && board[x+i][y+j] != Color::EMPTY) {
                    return false;
                }
            }
        }

        bx = x;
        by = y;
        return true;
    }

    void rotate(bool is_left) {
        std::vector<bool> turn = currb.grid;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < i; j++) {
                swap(turn[i*2+j], turn[j*2+i]);
            }
        }
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 1; j++) {
                if (is_left) {
                    swap(turn[j*2+i], turn[(2-j-1)*2+i]);
                } else {
                    swap(turn[i*2+j], turn[i*2+2-j-1]);
                }
            }
        }
        if (validate_move(bx, by, turn))
            swap(turn, currb.grid);
    }

    void shift_left() {
        validate_move(bx, by-1, currb.grid);
    }

    void shift_right() {
        validate_move(bx, by+1, currb.grid);
    }

    void get_input() {
        if (kbhit()) {
            std::string s;
            std::cin >> s;
            if (s == "w") {
                rotate(true);
            } else if (s == "a") {
                shift_left();
            } else if (s == "s") {
                rotate(false);
            } else if (s == "d") {
                shift_right();
            } else if (s == "f") {
                std::cout << "space bar" << std::endl;
            } else {
                std::cout << "invalid command!" << std::endl;
            }
        }
    }

    void draw_board() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (currb.grid[2*i+j]) {
                    if (board[bx+i][by+j] != Color::EMPTY) {
                        std::cout << '\n' << "YOU LOSE!" << std::endl;
                        game_end = true;
                        return;
                    }
                    board[bx+i][by+j] = Color::BLOCK;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (board[i][j] == Color::BLOCK) {
                    std::cout << static_cast<int>(currb.col) << ' ';
                    board[i][j] = Color::EMPTY;
                } else {
                    std::cout << static_cast<int>(board[i][j]) << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    // Fix the current block onto the board, and generate a new block
    void draw_curr() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (currb.grid[2*i+j]) {
                    board[bx+i][by+j] = currb.col;
                }
            }
        }
        currb = generate_block();
        bx = 0;
        by = 0;
    }

    void drop() {
        int lowest_left = bx + (currb.grid[2] == true);
        int lowest_right = bx + (currb.grid[3] == true);
        if (lowest_left == n-1 || lowest_right == n-1) {
            draw_curr();
        } else if (board[lowest_left+1][by] != Color::EMPTY || board[lowest_right+1][by+1] != Color::EMPTY) {
            draw_curr();
        } else {
            bx++;
        }
    }

    Block generate_block() {
        return Block(dist(rng), dist_col(rng));
    }

private:
    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;
    std::uniform_int_distribution<std::mt19937::result_type> dist_col;

    bool game_end = false;
    int n, m;
    std::vector<std::vector<Color>> board;
    int bx, by;
    Block currb;
};

std::vector<std::vector<bool>> Tiles::Block::variants = std::vector<std::vector<bool>>({ {1,1,0,0}, {1,1,0,1}, {1,1,1,1} });

int main(void) {
    Tiles t(10, 5);
    t.start();
    return 0;
}
