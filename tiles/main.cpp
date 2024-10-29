#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

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
        Block(unsigned int t, unsigned int c) : type{t}, col{static_cast<Color>(c)} {}
        unsigned int type;
        Color col;
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
        int SLEEP_MS;
        draw_board();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        while (!game_end) {

            drop();
            draw_board();
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        }
    }
private:

    void draw_board() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (Block::variants[currb.type][2*i+j]) {
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
                if (Block::variants[currb.type][2*i+j]) {
                    board[bx+i][by+j] = currb.col;
                }
            }
        }
        currb = generate_block();
        bx = 0;
        by = 0;
    }

    void drop() {
        int lowest_left = bx + (Block::variants[currb.type][2] == true);
        int lowest_right = bx + (Block::variants[currb.type][3] == true);
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
    Tiles t(10, 10);
    t.start();
    return 0;
}
