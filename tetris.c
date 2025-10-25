/*
 * Simple Tetris in C using ncurses
 * Compile: gcc -o tetris tetris.c -lncurses
 * Controls:
 *  Left/Right arrows: move
 *  Up arrow / X / Z: rotate
 *  Down arrow: soft drop
 *  Space: hard drop
 *  Q: quit
 *
 * Note: requires ncurses (libncurses-dev)
 */

#include <ncurses\ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BOARD_W 10
#define BOARD_H 20
#define TET_SIZE 4

enum { EMPTY=0, FILLED=1 };

typedef struct {
    int x, y; // position of top-left of tetromino 4x4
    int shape; // index 0..6
    int rot;   // rotation 0..3
} Piece;

// 7 tetrominoes, each as 4 rotations of 4x4 matrix (16 chars)
static const char tetromino[7][4][TET_SIZE*TET_SIZE] = {
    // I
    {
        {0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0},
        {0,0,1,0, 0,0,1,0, 0,0,1,0, 0,0,1,0},
        {0,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0},
        {0,1,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0}
    },
    // J
    {
        {1,0,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,1,0, 0,1,0,0, 0,1,0,0, 0,0,0,0},
        {0,0,0,0, 1,1,1,0, 0,0,1,0, 0,0,0,0},
        {0,1,0,0, 0,1,0,0, 1,1,0,0, 0,0,0,0}
    },
    // L
    {
        {0,0,1,0, 1,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,0,0, 0,1,0,0, 0,1,1,0, 0,0,0,0},
        {0,0,0,0, 1,1,1,0, 1,0,0,0, 0,0,0,0},
        {1,1,0,0, 0,1,0,0, 0,1,0,0, 0,0,0,0}
    },
    // O
    {
        {0,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0}
    },
    // S
    {
        {0,1,1,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},
        {0,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,0,0},
        {0,0,0,0, 0,1,1,0, 1,1,0,0, 0,0,0,0},
        {1,0,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}
    },
    // T
    {
        {0,1,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,1,0,0, 0,1,1,0, 0,1,0,0, 0,0,0,0},
        {0,0,0,0, 1,1,1,0, 0,1,0,0, 0,0,0,0},
        {0,1,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}
    },
    // Z
    {
        {1,1,0,0, 0,1,1,0, 0,0,0,0, 0,0,0,0},
        {0,0,1,0, 0,1,1,0, 0,1,0,0, 0,0,0,0},
        {0,0,0,0, 1,1,0,0, 0,1,1,0, 0,0,0,0},
        {0,1,0,0, 1,1,0,0, 1,0,0,0, 0,0,0,0}
    }
};

int board[BOARD_H][BOARD_W];
Piece current;
int nextPiece;
int score = 0;
int level = 1;
int lines_cleared = 0;

// check if piece fits on board
int fits(Piece *p) {
    for (int ry=0; ry<TET_SIZE; ++ry) {
        for (int rx=0; rx<TET_SIZE; ++rx) {
            int cell = tetromino[p->shape][p->rot][ry*TET_SIZE + rx];
            if (!cell) continue;
            int bx = p->x + rx;
            int by = p->y + ry;
            if (bx < 0 || bx >= BOARD_W || by < 0 || by >= BOARD_H) return 0;
            if (board[by][bx]) return 0;
        }
    }
    return 1;
}

void lock_piece(Piece *p) {
    for (int ry=0; ry<TET_SIZE; ++ry) {
        for (int rx=0; rx<TET_SIZE; ++rx) {
            int cell = tetromino[p->shape][p->rot][ry*TET_SIZE + rx];
            if (!cell) continue;
            int bx = p->x + rx;
            int by = p->y + ry;
            if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W)
                board[by][bx] = FILLED;
        }
    }
}

void clear_lines() {
    int cleared = 0;
    for (int y=BOARD_H-1; y>=0; --y) {
        int full = 1;
        for (int x=0; x<BOARD_W; ++x) if (!board[y][x]) { full = 0; break; }
        if (full) {
            // move everything down
            for (int yy=y; yy>0; --yy) for (int x=0; x<BOARD_W; ++x) board[yy][x] = board[yy-1][x];
            for (int x=0; x<BOARD_W; ++x) board[0][x] = EMPTY;
            ++cleared;
            ++y; // recheck same row
        }
    }
    if (cleared) {
        lines_cleared += cleared;
        // scoring: classic tetris scoring (single/double/triple/tetris)
        int points;
        switch (cleared) {
            case 1: points = 40 * level; break;
            case 2: points = 100 * level; break;
            case 3: points = 300 * level; break;
            default: points = 1200 * level; break;
        }
        score += points;
        // level up every 10 lines
        level = 1 + lines_cleared / 10;
    }
}

void spawn_piece() {
    current.shape = nextPiece;
    nextPiece = rand() % 7;
    current.rot = 0;
    current.x = (BOARD_W/2) - (TET_SIZE/2);
    current.y = -1; // start slightly above
    if (!fits(&current)) {
        // game over
        clear();
        mvprintw(10, 5, "GAME OVER");
        mvprintw(12, 5, "Score: %d", score);
        mvprintw(14, 5, "Press Q to quit");
        refresh();
        nodelay(stdscr, FALSE);
        int ch;
        while ((ch = getch()) != 'q' && ch != 'Q') {}
        endwin();
        exit(0);
    }
}

void init_game() {
    memset(board, 0, sizeof(board));
    srand(time(NULL));
    nextPiece = rand() % 7;
    spawn_piece();
    score = 0; level = 1; lines_cleared = 0;
}

void draw_board() {
    int ox = 2, oy = 1;
    // draw border
    for (int y=0; y<=BOARD_H+1; ++y) {
        mvaddch(oy+y, ox-1, '|');
        mvaddch(oy+y, ox+BOARD_W*2, '|');
    }
    for (int x=0; x<BOARD_W*2; ++x) mvaddch(oy+BOARD_H, ox+x, '-');

    // draw occupied
    for (int y=0; y<BOARD_H; ++y) {
        for (int x=0; x<BOARD_W; ++x) {
            if (board[y][x]) mvprintw(oy+y, ox + x*2, "[]");
            else mvprintw(oy+y, ox + x*2, "  ");
        }
    }

    // draw current piece
    for (int ry=0; ry<TET_SIZE; ++ry) {
        for (int rx=0; rx<TET_SIZE; ++rx) {
            int cell = tetromino[current.shape][current.rot][ry*TET_SIZE + rx];
            if (!cell) continue;
            int bx = current.x + rx;
            int by = current.y + ry;
            if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W)
                mvprintw(oy+by, ox + bx*2, "[]");
        }
    }

    // draw next piece box
    int nx = ox + BOARD_W*2 + 4, ny = oy;
    mvprintw(ny, nx, "Next:");
    for (int ry=0; ry<TET_SIZE; ++ry) {
        for (int rx=0; rx<TET_SIZE; ++rx) {
            int cell = tetromino[nextPiece][0][ry*TET_SIZE + rx];
            if (cell) mvprintw(ny+1+ry, nx+rx*2, "[]"); else mvprintw(ny+1+ry, nx+rx*2, "  ");
        }
    }

    // stats
    mvprintw(ny+8, nx, "Score: %d", score);
    mvprintw(ny+9, nx, "Level: %d", level);
    mvprintw(ny+10, nx, "Lines: %d", lines_cleared);

    mvprintw(oy+BOARD_H+2, ox, "Controls: ← → = move | ↑ = rotate | ↓ = soft drop | Space = hard drop | Q = quit");
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    init_game();

    int ch;
    struct timespec last, now;
    clock_gettime(CLOCK_MONOTONIC, &last);
    double fall_interval = 0.8; // seconds (will decrease with level)

    while (1) {
        // timing
        clock_gettime(CLOCK_MONOTONIC, &now);
        double elapsed = (now.tv_sec - last.tv_sec) + (now.tv_nsec - last.tv_nsec)/1e9;
        double adjusted_interval = fall_interval * (1.0 / (1.0 + (level-1)*0.1));

        // input
        ch = getch();
        if (ch != ERR) {
            if (ch == 'q' || ch == 'Q') break;
            else if (ch == KEY_LEFT) {
                Piece tmp = current; tmp.x -= 1; if (fits(&tmp)) current.x -= 1;
            }
            else if (ch == KEY_RIGHT) {
                Piece tmp = current; tmp.x += 1; if (fits(&tmp)) current.x += 1;
            }
            else if (ch == KEY_UP || ch == 'x' || ch == 'X' || ch == 'z' || ch == 'Z') {
                Piece tmp = current; tmp.rot = (tmp.rot+1)%4; if (fits(&tmp)) current.rot = tmp.rot;
            }
            else if (ch == KEY_DOWN) {
                Piece tmp = current; tmp.y += 1; if (fits(&tmp)) current.y += 1; else { lock_piece(&current); clear_lines(); spawn_piece(); }
            }
            else if (ch == ' ') { // hard drop
                while (1) {
                    Piece tmp = current; tmp.y += 1; if (fits(&tmp)) current.y += 1; else { lock_piece(&current); clear_lines(); spawn_piece(); break; }
                }
            }
        }

        if (elapsed >= adjusted_interval) {
            last = now;
            // fall
            Piece tmp = current; tmp.y += 1;
            if (fits(&tmp)) {
                current.y += 1;
            } else {
                lock_piece(&current);
                clear_lines();
                spawn_piece();
            }
        }

        clear();
        draw_board();
        refresh();

        // small sleep to reduce CPU
        usleep(20000);
    }

    endwin();
    return 0;
}
