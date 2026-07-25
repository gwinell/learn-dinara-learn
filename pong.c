#include <stdio.h>

void clear_screen(void);
void move_cursor(int row, int col);
int count_digits(int n);
void print_score_line(int score_left, int score_right);
void draw(int left_paddle, int right_paddle, int ball_x, int ball_y,
          int score_left, int score_right, int winner);
int read_command(void);
void apply_command(int command, int *left_paddle, int *right_paddle);
int move_paddle(int paddle_y, int direction);
void update_seed(unsigned int *seed);
void serve_ball(int *ball_x, int *ball_y, int *dir_x, int *dir_y,
                unsigned int *seed);
void update_ball(int *ball_x, int *ball_y, int *dir_x, int *dir_y,
                 int left_paddle, int right_paddle,
                 int *score_left, int *score_right, unsigned int *seed);

int main(void) {
    int left_paddle = 13;
    int right_paddle = 13;
    int ball_x;
    int ball_y;
    int dir_x;
    int dir_y;
    int score_left = 0;
    int score_right = 0;
    int winner = 0;
    int win_score = 21;
    int command;
    unsigned int seed = 123456789;

    serve_ball(&ball_x, &ball_y, &dir_x, &dir_y, &seed);

    while (winner == 0) {
        draw(left_paddle, right_paddle, ball_x, ball_y,
             score_left, score_right, winner);
        command = read_command();
        apply_command(command, &left_paddle, &right_paddle);
        update_ball(&ball_x, &ball_y, &dir_x, &dir_y,
                    left_paddle, right_paddle,
                    &score_left, &score_right, &seed);
        if (score_left >= win_score) {
            winner = 1;
        } else if (score_right >= win_score) {
            winner = 2;
        }
    }

    draw(left_paddle, right_paddle, ball_x, ball_y,
         score_left, score_right, winner);
    printf("\033[?25h\n");
    return 0;
}

void clear_screen(void) {
    printf("\033[2J\033[H\033[?25l");
}

void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

int count_digits(int n) {
    int count = 1;
    while (n >= 10) {
        n /= 10;
        count++;
    }
    return count;
}

void print_score_line(int score_left, int score_right) {
    int right_col = 80 - 8 - count_digits(score_right) + 1;
    move_cursor(1, 1);
    printf("\033[1;36mPlayer: %d", score_left);
    move_cursor(1, right_col);
    printf("Player: %d\033[0m", score_right);
}

void draw(int left_paddle, int right_paddle, int ball_x, int ball_y,
          int score_left, int score_right, int winner) {
    int x;
    int y;
    clear_screen();
    print_score_line(score_left, score_right);
    for (y = 0; y < 25; y++) {
        for (x = 0; x < 80; x++) {
            move_cursor(y + 2, x + 1);
            if (x == ball_x && y == ball_y) {
                printf("\033[1;31m*\033[0m");
            } else if (x == 1 && y >= left_paddle - 1 && y <= left_paddle + 1) {
                printf("\033[1;32m#\033[0m");
            } else if (x == 78 && y >= right_paddle - 1 && y <= right_paddle + 1) {
                printf("\033[1;32m#\033[0m");
            } else if (y == 0 || y == 24) {
                printf("\033[37m-\033[0m");
            } else if (x == 0 || x == 79) {
                printf("\033[37m|\033[0m");
            } else if (x == 40) {
                printf("\033[34m:\033[0m");
            } else {
                printf(" ");
            }
        }
    }
    move_cursor(27, 1);
    if (winner == 1) {
        printf("\033[1;35mPlayer 1 (left) wins!\033[0m");
    } else if (winner == 2) {
        printf("\033[1;35mPlayer 2 (right) wins!\033[0m");
    } else {
        printf("\033[33mA/Z: left up/down | K/M: right up/down | Enter: skip turn\033[0m");
    }
    printf("\n");
}

int read_command(void) {
    int c = getchar();
    int d;
    if (c == EOF || c == '\n') {
        return 0;
    }
    d = c;
    while (d != '\n' && d != EOF) {
        d = getchar();
    }
    if (c == 'a' || c == 'A') {
        return 1;
    }
    if (c == 'z' || c == 'Z') {
        return 2;
    }
    if (c == 'k' || c == 'K') {
        return 3;
    }
    if (c == 'm' || c == 'M') {
        return 4;
    }
    return 0;
}

void apply_command(int command, int *left_paddle, int *right_paddle) {
    if (command == 1) {
        *left_paddle = move_paddle(*left_paddle, -1);
    } else if (command == 2) {
        *left_paddle = move_paddle(*left_paddle, 1);
    } else if (command == 3) {
        *right_paddle = move_paddle(*right_paddle, -1);
    } else if (command == 4) {
        *right_paddle = move_paddle(*right_paddle, 1);
    }
}

int move_paddle(int paddle_y, int direction) {
    paddle_y += direction;
    if (paddle_y < 2) {
        paddle_y = 2;
    }
    if (paddle_y > 22) {
        paddle_y = 22;
    }
    return paddle_y;
}

void update_seed(unsigned int *seed) {
    *seed = *seed * 1103515245u + 12345u;
}

void serve_ball(int *ball_x, int *ball_y, int *dir_x, int *dir_y,
                unsigned int *seed) {
    update_seed(seed);
    *ball_x = 40;
    *ball_y = 12;
    if (*seed % 2 == 0) {
        *dir_x = 1;
    } else {
        *dir_x = -1;
    }
    *dir_y = (int)((*seed / 2) % 3) - 1;
}

void update_ball(int *ball_x, int *ball_y, int *dir_x, int *dir_y,
                 int left_paddle, int right_paddle,
                 int *score_left, int *score_right, unsigned int *seed) {
    int next_x = *ball_x + *dir_x;
    int next_y = *ball_y + *dir_y;

    if (next_y < 1 || next_y > 23) {
        *dir_y = -*dir_y;
        next_y = *ball_y + *dir_y;
    }
    if (next_x == 1 && next_y >= left_paddle - 1 && next_y <= left_paddle + 1) {
        *dir_x = -*dir_x;
        next_x = *ball_x + *dir_x;
    }
    if (next_x == 78 && next_y >= right_paddle - 1 && next_y <= right_paddle + 1) {
        *dir_x = -*dir_x;
        next_x = *ball_x + *dir_x;
    }
    if (next_x <= 0) {
        *score_right += 1;
        serve_ball(ball_x, ball_y, dir_x, dir_y, seed);
        return;
    }
    if (next_x >= 79) {
        *score_left += 1;
        serve_ball(ball_x, ball_y, dir_x, dir_y, seed);
        return;
    }
    *ball_x = next_x;
    *ball_y = next_y;
}
