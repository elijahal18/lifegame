#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    Elijah Ager Luckett
    Unix Systems Programming
    Homework 1: Game of life
*/

// function prototypes
int** init(FILE* gen0, int rows, int collums);
int display(int** world, int rows, int collums);
int numneighbors(int** world, int x, int y);
int update(int** world, int rows, int collums);
void freeworld(int** world, int rows);

// main function (used to parse arguments and use functions to run the game
int main(int argc, char** argv){
    // default values
    int rows = 10;
    int collums = 10;
    int generations = 10;
    char* file = "life.txt";
    
    // depending on the number of arguments change the default values
    if (argc>=3){
        rows = strtol(argv[1], NULL, 10);
        collums = strtol(argv[2], NULL, 10);
    }
    if (argc >= 4){
        file = argv[3];
    }
    if (argc >= 5){
        generations = strtol(argv[4], NULL, 10);
    }
    // print error message if arguments are incorrect
    if (argc > 5){
        fprintf(stderr, "incorrect input size");
        return 1;
    }
    
    FILE* gen0 = fopen(file, "r");
    
    // check that the rows are formatted correctly and that there is a readable file
    if (gen0==NULL){
        fprintf(stderr,"No file named %s\n", file);
        return 1;
    }
    if (rows <= 0 || collums <= 0){
        fprintf(stderr,"incorrect sizing");
        return 1;
    }
    
    // add 2 more rows and collumns for game buffer
    rows+=2;
    collums+=2;
    
    int** world; //represents the world we will print
    
    world = init(gen0,rows,collums); // use function init to read the generation 0 file and fill the world with 1s (alive) and 0s (dead)
    
    if (world == NULL){ //should return the correct world if not print error
        fprintf(stderr,"first gen failed\n");
        return 1;
    }
    // display first generation
    printf("Generation 0 \n");
    int genres=display(world,rows,collums);
    if (genres==1){
        return 1;
    }
    
    // then augment and display each generation using the functions update and display
    for (int i = 1; i <= generations; i++){
        printf("Generation %d \n", i);
        genres=update(world, rows, collums);
        if (genres==1){
            return 1;
        }
        genres=display(world, rows, collums);
        if (genres==1){
            return 1;
        }
    }
    freeworld(world,rows); // free the allocated space used to store the world
    return 0;
}

// init file is used to allocate space for the world and fill in the correct alive (1) or dead (0) values to the int **world

int** init(FILE* gen0, int rows, int collums){
    int **world;
    world = (int**) calloc(rows, sizeof(int**));// allocate space for the world
    for (int i = 0; i < rows; i++) {
        world[i] = (int*) calloc(rows, sizeof(int*)); //allocate space for each row
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < collums; j++){
            world[i][j] = 0; //Make the default for each coordinate dead
        }
    }
    
    // variables for reading the file
    char ch;
    int x = 1, y=1; //set up coord
    if (gen0==NULL){ //
        fprintf(stderr,"incorrect sizing");
        return NULL;
    }
    else{
        while ((ch = fgetc(gen0))!=EOF){//read file using fgetc
            if(ch == ' '){ // empty spaces are for dead coordinates
                world[y][x] = 0; //has to be y x because first indexing is the row
                x++;
            }
            else if(ch == '*'){ // * indicate life
                world[y][x] = 1;
                x++;
            }
            else if (ch == '\n') { // switch to new line
                y++; // go next row
                x = 1; // reset starting to left
            }
            else{
                fprintf(stderr,"incorrectly formatted file");
                return NULL;
            }
        }
    }
    return world;
}

// Used to read the int** world variable and print it out using dead (-) or alive (*) to the terminal
int display(int** world, int rows, int collums){
    char dead = '-';
    char alive = '*';
    for(int row = 1; row < rows-1; row++){
        for(int collum = 1; collum < collums-1; collum++){// go through each coordinate that isn't the buffer
            //printf("%d",world[row][collum]);
            if (world[row][collum]==1){
                printf("%c", alive);
            }
            else if (world[row][collum]==0){
                printf("%c", dead);
                //printf(" ");
            }
            /*
            else {
                fprintf(stderr,"incorrect formating of world");
                return 1;
            }
             */
        }
        printf("\n");
    }
    return 0;
}

// used by update() to determine the number of neighbors for each coordinate to update the game
int numneighbors(int** world, int x, int y){
    // could do with loop but instead just find each coordinate individually
    // space will be freed and not too time consuming
    int n = world[x][y+1];
    int s = world[x][y-1];
    int w = world[x-1][y];
    int e = world[x+1][y];
    int ne = world[x+1][y+1];
    int se = world[x+1][y-1];
    int nw = world[x-1][y+1];
    int sw = world[x-1][y-1];
    int numneighbors = n+s+w+e+ne+se+nw+sw;
    return numneighbors;
}

// used to update the game
int update(int** world, int rows, int collums){
    //allocate temporary world
    int temp[rows][collums];
    for(int row = 1; row < rows - 1; row++){
        for(int collum = 1; collum < collums - 1; collum++){ // go through each coordinate (that isn't the buffer
            int aliveneigh = numneighbors(world, row, collum); // find the number of neighbors that they have
            
            if(world[row][collum] == 1){ // if a cell is alive
                if(aliveneigh == 3 || aliveneigh == 2){ // if the cell has 2 or 3 neighbors then it lives on
                    temp[row][collum] = 1;
                }
                else{
                    temp[row][collum] = 0; // else it will die
                }
            }
            else if (world[row][collum] == 0 && aliveneigh == 3){ //if it is dead than the only way it lives on is it has 3 neighbors
                temp[row][collum] = 1;
            }
            else{ // in any other case the cell will be dead
                temp[row][collum] = 0;
            }
            
        }
    }

    // Apply the next gen by filling the int** world with the temporary values
    for(int row = 1; row < rows -1; row++){
        for(int collum = 1; collum < collums -1; collum++)
        {
            world[row][collum] = temp[row][collum];
        }
    }
    return 0;
}

// free world is just used to free the allocated memory int **world
void freeworld(int** world, int rows){
    for(int i = 0; i < rows; i++){//free each row
        free(world[i]);
    }
    free(world);//then free the int** world
}
