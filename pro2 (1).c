/*
* Alexa Padberg
* Operating systems
* 
* This code takes in a 9x9 sudoku text file, checks for valid format,
* then checks if the columns, rows, and squares are valid to ensure the 
* solution is correct.
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 27

int sudoku[9][9];
bool is_board_valid = true;

/* structure for passing data to threads */
typedef struct {
    int row;
    int col;
} params_t;

void* check_rows(void* args);
void* check_cols(void* args);
void* check_box(void* args);

bool is_valid(int grid[9]);


int main(int argc, char* argv[]) {

    // Read Sudoku solution from file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    int i = 0;
    int j = 0;
    //read solution in form of sudoku grid
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (fscanf(file, "%d", &sudoku[i][j]) != 1) {
                printf("Error: Invalid input format in file '%s'\n", argv[1]);
                fclose(file);
                return EXIT_FAILURE;
            }
        }


    }
    fclose(file);
    //print sudoku grid
    printf("Sudoku grid:\n");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    pthread_t tid[NUM_THREADS];  /* the thread identifier array */
    pthread_attr_t attr; /* set of thread attributes */

    /* set the default attributes of the thread */
    pthread_attr_init(&attr);

    // Create threads for checking rows
    for (i = 0; i < 9; i++) {
        params_t* data = (params_t*)malloc(sizeof(params_t));
        data->row = i;
        data->col = 0;
        pthread_create(&tid[i], &attr, check_rows, (void*)data);
    }


    // Create threads for checking cols
    for (j = 0; j < 9; j++) {
        params_t* data = (params_t*)malloc(sizeof(params_t));
        data->row = 0;
        data->col = j;
        int tid_acc = 9 + j;
        pthread_create(&tid[tid_acc], &attr, check_cols, (void*)data);
    }

    //create threads for checking sqaures
    for (int box = 0; box < 9; box++) {
        params_t* data = (params_t*)malloc(sizeof(params_t));
        data->row = (box / 3) * 3; // Starting row of the box. integer division
        data->col = (box % 3) * 3; // Starting column of the box
        pthread_create(&tid[18 + box], &attr, check_box, (void*)data);
    }



    //join all the threads together
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    //print out result
    if (is_board_valid)
        printf("The Sudoku board is valid!\n");
    else
        printf("The Sudoku board is not valid.\n");


    return EXIT_SUCCESS;
}

//function to check each row of the board
void* check_rows(void* args) {
    params_t* data = (params_t*)args;
    int row = data->row;
    int col = data->col;
    int curr[9];
    for (int j = 0; j < 9; j++) {
        curr[j] = sudoku[row][j];

    }
    if (!is_valid(curr)) {
        //  printf("Row %d is not valid\n", row + 1);
        is_board_valid = false;
    }
    if (is_valid(curr)) {
        //   printf("Row %d is  valid!\n", row + 1);
    }
    free(data);
    pthread_exit(NULL);
}

//function to check each column of the board
void* check_cols(void* args) {
    params_t* data = (params_t*)args;
    int row = data->row;
    int col = data->col;
    int curr[9];
    for (int i = 0; i < 9; i++) {
        curr[i] = sudoku[i][col];
    }

    if (!is_valid(curr)) {
        // printf("column %d is not valid\n", col + 1);
        is_board_valid = false;
    }

    free(data);
    pthread_exit(NULL);
}

//function to check each 3x3 square of the board
void* check_box(void* args) {
    params_t* data = (params_t*)args;
    int row = data->row;
    int col = data->col;
    int curr[9];
    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    int i = 0;
    int j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            curr[i * 3 + j] = sudoku[start_row + i][start_col + j];
        }
    }


    if (!is_valid(curr)) {
        ///  printf("Box at row %d, col %d is not valid\n", start_row + 1, start_col + 1);
        is_board_valid = false;
    }
    else {
        //  printf("Box at row %d, col %d is valid!\n", start_row + 1, start_col + 1);
    }
    free(data);
    pthread_exit(NULL);
}


//function take in a row/column/box then stores in a new column based off
//their values 
bool is_valid(int grid[9]) {
    bool used_nums[10] = { false };
    int i = 0;
    for (i = 0; i < 9; i++) {
        int num = grid[i];
        used_nums[num] = true;
    }
    for (i = 1; i <= 9; i++) {
        if (!used_nums[i]) {
            is_board_valid = false;
            return false;
        }
    }
    return true;
}
