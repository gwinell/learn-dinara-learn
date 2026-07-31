#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define STATUS_LINE FIELD_HEIGHT
#define MIN_DELAY 10000
#define MAX_DELAY 500000
#define INITIAL_DELAY 100000
#define DELAY_STEP 10000

struct Field {
    int cells[FIELD_HEIGHT][FIELD_WIDTH];
};

static int count_neighbors(const struct Field* field, int row, int col);
static void update_field(const struct Field* field, struct Field* new_field);
static void copy_field(const struct Field* src, struct Field* dest);
static void read_initial_state(struct Field* field);
static void print_field(const struct Field* field);
static int init_ncurses();
static void cleanup_ncurses();
static int handle_input(int delay, int* running);
static void game_loop(struct Field* field, struct Field* next_field);

int main() {
    struct Field field = {0};
    struct Field next_field = {0};
    int error = 0;
    read_initial_state(&field);
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        error = 1;
    }
    if (error == 0) {
        error = init_ncurses();
    }
    if (error == 0) {
        game_loop(&field, &next_field);
        cleanup_ncurses();
    }
    return error;
}

static int count_neighbors(const struct Field* field, int row, int col) {
    int count = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr != 0 || dc != 0) {
                int r = (row + dr + FIELD_HEIGHT) % FIELD_HEIGHT;
                int c = (col + dc + FIELD_WIDTH) % FIELD_WIDTH;
                count += field->cells[r][c];
            }
        }
    }
    return count;
}

static void update_field(const struct Field* field, struct Field* new_field) {
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            int neighbors = count_neighbors(field, row, col);
            int alive = field->cells[row][col];
            int next = 0;
            if (alive && (neighbors == 2 || neighbors == 3)) {
                next = 1;
            } else if (!alive && neighbors == 3) {
                next = 1;
            }
            new_field->cells[row][col] = next;
        }
    }
}

static void copy_field(const struct Field* src, struct Field* dest) {
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            dest->cells[row][col] = src->cells[row][col];
        }
    }
}

static void read_initial_state(struct Field* field) {
    char line[FIELD_WIDTH + 2];
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            field->cells[row][col] = 0;
        }
    }
    for (int row = 0; row < FIELD_HEIGHT && fgets(line, sizeof(line), stdin); row++) {
        for (int col = 0; col < FIELD_WIDTH && line[col] != '\0' && line[col] != '\n'; col++) {
            if (line[col] == '1' || line[col] == '*') {
                field->cells[row][col] = 1;
            }
        }
    }
}

static void print_field(const struct Field* field) {
    clear();
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            char ch = field->cells[row][col] ? '*' : ' ';
            mvaddch(row, col, ch);
        }
    }
    mvaddstr(STATUS_LINE, 0, "Controls: A/Z speed, Q/Space quit");
    refresh();
}

static int init_ncurses() {
    int result = 0;
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);
    if (LINES < FIELD_HEIGHT + 1 || COLS < FIELD_WIDTH) {
        endwin();
        result = 1;
    }
    return result;
}

static void cleanup_ncurses() { endwin(); }

static int handle_input(int delay, int* running) {
    int result = delay;
    int ch = getch();
    if (ch == ' ' || ch == 'q' || ch == 'Q') {
        *running = 0;
    } else if ((ch == 'a' || ch == 'A') && result > MIN_DELAY) {
        result -= DELAY_STEP;
    } else if ((ch == 'z' || ch == 'Z') && result < MAX_DELAY) {
        result += DELAY_STEP;
    }
    return result;
}

static void game_loop(struct Field* field, struct Field* next_field) {
    int delay = INITIAL_DELAY;
    int running = 1;
    while (running) {
        print_field(field);
        update_field(field, next_field);
        copy_field(next_field, field);
        usleep(delay);
        delay = handle_input(delay, &running);
    }
}                next = 1;
            } else if (!alive && neighbors == 3) {
                next = 1;
            }
            new_field->cells[row][col] = next;
        }
    }
}

static void copy_field(const struct Field* src, struct Field* dest) {
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            dest->cells[row][col] = src->cells[row][col];
        }
    }
}

static void read_initial_state(struct Field* field) {
    char line[FIELD_WIDTH + 2];
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            field->cells[row][col] = 0;
        }
    }
    for (int row = 0; row < FIELD_HEIGHT && fgets(line, sizeof(line), stdin); row++) {
        for (int col = 0; col < FIELD_WIDTH && line[col] != '\0' && line[col] != '\n'; col++) {
            if (line[col] == '1' || line[col] == '*') {
                field->cells[row][col] = 1;
            }
        }
    }
}

static void print_field(const struct Field* field) {
    clear();
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            char ch = field->cells[row][col] ? '*' : ' ';
            mvaddch(row, col, ch);
        }
    }
    refresh();
}

static int init_ncurses() {
    int result = 0;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);
    if (LINES < FIELD_HEIGHT || COLS < FIELD_WIDTH) {
        endwin();
        result = 1;
    }
    return result;
}

static void cleanup_ncurses() { endwin(); }

static int handle_input(int delay, int* running) {
    int result = delay;
    int ch = getch();
    if (ch == ' ') {
        *running = 0;
    } else if ((ch == 'a' || ch == 'A') && result > MIN_DELAY) {
        result -= DELAY_STEP;
    } else if ((ch == 'z' || ch == 'Z') && result < MAX_DELAY) {
        result += DELAY_STEP;
    }
    return result;
}

static void game_loop(struct Field* field, struct Field* next_field) {
    int delay = INITIAL_DELAY;
    int running = 1;
    while (running) {
        print_field(field);
        update_field(field, next_field);
        copy_field(next_field, field);
        usleep(delay);
        delay = handle_input(delay, &running);
    }
}
