#include <stdio.h>
//Declaring global variables
int board[9][9];
int locations[9][3][9];
//board[rows][columns]

//Function prototypes
void printBoard();
void setLocationsInitial();
void setBoard();
void bruteForce();
void findAllNumbers();
int checkSolve();
void lessBruteForce();

//Main function
int main()
{

    //Setting sudoku board to solve
    setBoard();

    //Setting locations
    setLocationsInitial();
    findAllNumbers();

    //Printing sudoku board to solve
    printBoard();

    printf("\n\nEnter any key to solve the board:\n");
    getchar();

    printf("\n");

    //Solving sudoku board
    int previousCheck = 0;
    for (;;)
    {
        if ((!checkSolve()) || (checkSolve() == previousCheck))
        {
            break;
        }
        previousCheck = checkSolve();
        bruteForce();
        lessBruteForce();
    }

    //printing solved sudoku board
    printBoard();

    fflush(stdin);
    printf("\n\nEnter any key to quit:\n");
    getchar();
    return 0;
}

//Function declarations

int checkSolve()
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!board[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

void setLocationsInitial()
{
    for (int k = 0; k < 9; k++)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                //Check rows for all numbers
                locations[k][0][i] = 0;

                //Check columns for all numbers
                locations[k][1][j] = 0;

                //Check square for all numbers
                locations[k][2][(i / 3) * 3 + (j / 3)] = 0;
            }
        }
    }
}

void setBoard()
{
    FILE *ptr;
    ptr = fopen("board.txt", "r");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(ptr, "%d", &board[i][j]);
        }
    }
}

void printBoard()
{
    for (int i = 0; i < 9; i++)
    {
        printf("\n");
        if (i == 3 || i == 6)
        {
            for (int k = 0; k < 11; k++)
            {
                printf(".. ");
            }
            printf("\n");
        }
        for (int j = 0; j < 9; j++)
        {
            if (j == 3 || j == 6)
            {
                printf(":  ");
            }
            printf("%d  ", board[i][j]);
        }
    }
}

//Make a 2-D array with 3 1-D arrays for each number
//The 3 1-D arrays store 0 or 1 for whether the row, column or square contain the number
//Now take the intersection of these 3 arrays, if it is found that a number is in none, then fill it

void findAllNumbers()
{
    for (int k = 1; k <= 9; k++)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                //Check rows for all numbers
                locations[k - 1][0][i] += (board[i][j] == k);

                //Check columns for all numbers
                locations[k - 1][1][j] += (board[i][j] == k);

                //Check square for all numbers
                locations[k - 1][2][(i / 3) * 3 + (j / 3)] += (board[i][j] == k);
            }
        }
    }
}

void bruteForce()
{
    //Now we have an array locations[9][3][9]
    //If locations[k][0][i] == locations[k][1][j] == locations[k][2][l] == 0
    //Means that the number k is not in location (i,j) coordinate and l'th square

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!board[i][j])
            {
                int canFillNums[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
                for (int k = 1; k <= 9; k++)
                {
                    if (locations[k - 1][0][i] == locations[k - 1][1][j] &&
                        locations[k - 1][2][(i / 3) * 3 + (j / 3)] == locations[k - 1][1][j] &&
                        locations[k - 1][0][i] == 0)
                    {
                        canFillNums[k - 1] = 1;
                    }
                }
                int count = 0, m = 0;
                for (int l = 0; l < 9; l++)
                {
                    if (canFillNums[l] == 1)
                    {
                        count++;
                        m = l;
                    }
                }
                if (count == 1)
                {
                    board[i][j] = m + 1;
                    locations[m][0][i] = 1;
                    locations[m][1][j] = 1;
                    locations[m][2][(i / 3) * 3 + (j / 3)] = 1;
                }
            }
        }
    }
}

void lessBruteForce()
{
    for (int k = 1; k <= 9; k++)
    {
        for (int l = 0; l < 9; l++)
        {
            if (locations[k - 1][2][l] == 0)
            {
                // printf("Bello, l = %d, k = %d\n", l, k);
                // printf("Hello\n");

                int rowFactor = (l / 3) * 3;
                int colFactor = (l % 3) * 3;
                int row[3] = {rowFactor, rowFactor + 1, rowFactor + 2};
                int col[3] = {colFactor, colFactor + 1, colFactor + 2};

                for (int x = 0; x < 3; x++)
                {
                    //Row square check
                    if (locations[k - 1][2][(l / 3) * 3 + x])
                    {
                        // printf("Hello, l = %d, new l = %d, k = %d\n", l, ((l/3)*3 + x), k);
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                if (board[rowFactor + i][j + x * 3] == k)
                                {
                                    row[i] = -1;
                                }
                            }
                        }
                    }

                    //Col square check
                    if (locations[k - 1][2][l % 3 + x * 3])
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                if (board[i + x * 3][colFactor + j] == k)
                                {
                                    col[j] = -1;
                                }
                            }
                        }
                    }
                }

                int count = 0;
                int coordinate[2];
                for (int i = 0; i < 3; i++)
                {
                    if (row[i] != -1)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (col[j] != -1)
                            {
                                if (!board[row[i]][col[j]])
                                {
                                    count++;
                                    coordinate[0] = row[i];
                                    coordinate[1] = col[j];
                                }
                            }
                        }
                    }
                }
                // printf("Hello, l = %d, count = %d, k = %d\n", l, count, k);
                if (count == 1)
                {
                    board[coordinate[0]][coordinate[1]] = k;
                    locations[k - 1][0][coordinate[0]] = 1;
                    locations[k - 1][1][coordinate[1]] = 1;
                    locations[k - 1][2][l] = 1;
                }
            }
        }
    }
}