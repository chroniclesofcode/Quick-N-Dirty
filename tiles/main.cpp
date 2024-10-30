#include <iostream>
#include <vector>
#include <random>

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
        std::cout << currb.type << ' ' << currb.col << '\n';
    }
private:

    // Fix the current block onto the board, and generate a new block
    void draw_curr() {
        board[bx][by] = currb.col;
    }

    void drop() {
        int lowest_left = bx + (Block::variants[currb.type][2] == true);
        int lowest_right = bx + (Block::variants[currb.type][3] == true);
        if (lowest_left == n-1 || lowest_right == n-1) {
            draw_curr();
        } else if (board[bx][lowest_left+1] != Color::EMPTY || board[bx+1][lowest_right+1] != Color::EMPTY) {
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

    int n, m;
    std::vector<std::vector<Color>> board;
    int bx, by;
    Block currb;
};

std::vector<std::vector<bool>> Tiles::Block::variants = std::vector<std::vector<bool>>({ {1,1,0,0}, {1,1,0,1}, {1,1,1,1} });

int main(void) {
    Tiles t(20, 10);
    t.start();
    return 0;
}
