#include <ncurses.h>
#include <stdio.h>

#define LENGTH 80
#define HEIGHT 25

void init_matrix(int data[HEIGHT][LENGTH]);
void output_matrix(int buff[HEIGHT][LENGTH]);
void display(int speed);

void copy_matrix(int buff[HEIGHT][LENGTH], int data[HEIGHT][LENGTH]);
int count_neighbours(int x, int y, int buff[HEIGHT][LENGTH]);
void update_matrix(int buff[HEIGHT][LENGTH], int data[HEIGHT][LENGTH]);
int change_speed(int speed, char speed_number, int *stop);

int main() {
    int stop = 0;
    int speed = 300;
    int buff[HEIGHT][LENGTH];
    int data[HEIGHT][LENGTH];

    init_matrix(data);
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        stop = 1;
    }
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);

    while (stop != 1) {
        copy_matrix(buff, data);
        output_matrix(buff);

        refresh();
        update_matrix(buff, data);

        char speed_number = getch();
        speed = change_speed(speed, speed_number, &stop);

        napms(speed);
        display(speed);
    }
    endwin();
    return 0;
}

void init_matrix(int data[HEIGHT][LENGTH]) {
    char c;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (scanf(" %c", &c) == 1) {
                data[i][j] = (c == '1') ? 1 : 0;
            }
        }
    }
}

void copy_matrix(int buff[HEIGHT][LENGTH], int data[HEIGHT][LENGTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            buff[i][j] = data[i][j];
        }
    }
}

int count_neighbours(int i, int j, int buff[HEIGHT][LENGTH]) {
    int count = 0;
    count += buff[(i + HEIGHT - 1) % HEIGHT][(j + LENGTH - 1) % LENGTH];
    count += buff[(i + HEIGHT) % HEIGHT][(j + LENGTH - 1) % LENGTH];
    count += buff[(i + HEIGHT + 1) % HEIGHT][(j + LENGTH - 1) % LENGTH];
    count += buff[(i + HEIGHT - 1) % HEIGHT][(j + LENGTH) % LENGTH];
    count += buff[(i + HEIGHT + 1) % HEIGHT][(j + LENGTH) % LENGTH];
    count += buff[(i + HEIGHT - 1) % HEIGHT][(j + LENGTH + 1) % LENGTH];
    count += buff[(i + HEIGHT) % HEIGHT][(j + LENGTH + 1) % LENGTH];
    count += buff[(i + HEIGHT + 1) % HEIGHT][(j + LENGTH + 1) % LENGTH];
    return count;
}

void update_matrix(int buff[HEIGHT][LENGTH], int data[HEIGHT][LENGTH]) {
    int count;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            count = count_neighbours(i, j, buff);
            if (count == 3) {
                data[i][j] = 1;
            } else if (count == 2 && buff[i][j] == 1) {
                continue;
            } else {
                data[i][j] = 0;
            }
        }
    }
}

void output_matrix(int buff[HEIGHT][LENGTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (buff[i][j] == 1) {
                mvprintw(i, j, "*");
            } else {
                mvprintw(i, j, " ");
            }
        }
    }
}

int change_speed(int speed, char speed_number, int *stop) {
    switch (speed_number) {
        case '1':
            speed = 1000;
            break;
        case '2':
            speed = 500;
            break;
        case '3':
            speed = 100;
            break;
        case 'q':
            *stop = 1;
        default:
            break;
    }
    return speed;
}

void display(int speed) {
    printw("\npress q to quit\ndelay = %d\n", speed);
    printw("press 1, 2, 3 to change speed");
}