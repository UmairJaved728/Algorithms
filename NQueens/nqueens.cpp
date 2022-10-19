// Required Libraries
#include<iostream>
#include<fstream>
#include<cstring>
#include<sstream>
#include<vector>
#include<stack>

using namespace std;

// Defined constants
#define EMPTY 0     // Denotes Empty Cell
#define QUEEN 1     // Denotes Queen cell
#define OCCUPIED 2  // Denotes the Row and Column are occupied by a Queen

int** makeBoard(int** board, int size);                                     // Declares memory and assign initial value
void placeInitialQueens(int **board, vector<pair<int, int>> positions);     // Places the input Queens on the board
void printBoard(ostream& out, int** board, int size);                       // Prints the board to console and file as well
void deleteBoard(int** board, int size);                                    // Dealocates the memory, occupied by board
bool solveNQueens(int **board, int size, int queensPlaced);                 // Main function to implement the algorithm
bool baordIsValid(int** board, int size);     // IMPORTANT                  // Checks if the board is valid, for cases in which the inpuut board is alread invalid
bool checkDiagonal(int** board, int size, int r, int c);                    // Checks the Diagonal of a Cell, before placing a Queen

// Main function to start execution
int main(int argc, char **argv)
{
    if(argc != 3)   // First argument is executable file name, Other 2 will be the required file names.
    {
        //cout << argc << '\n';
        cout << "Invalid Arguments provided! Please input two file names.\nOne for input, one for output\n";
        return 0;       // If not provided, exit from thr program.
    }

    // Helper Variables
    string line;                        // To read a line from file
    fstream inputFile, outputFile;      // File pointers to open files
    stringstream ss;                    // To break a line into integers
    pair<int, int> p;                   // To store the (row, column) pair
    vector<pair<int, int>> positions;   // As we don't know the how many Queens will be placed at beginning, that's why we are using a vector
    int **board;                        // Main board grid
    int size, row, col, queensPlaced = 0, i, j; // Some Helper Variables
    bool isValid, isSolved;                       // to check whether a given board is valid and solve able

    inputFile.open(argv[1], ios::in);       // Opening Input File
    outputFile.open(argv[2], ios::out);     // Opening Output File

    if(inputFile.fail())    // If input file does not exists then exit from the program.
    {
        cout << "Input file does not exists\n";
        return 0;
    }

    // Loop Until we get lines from the input file
    while(getline(inputFile, line, '\n'))
    {
        line.push_back('\n');           // Add a line break at the end
        ss.str(line);                   // Push line to the streM   
        ss >> size;                     // Get first integer, i.e size of board from the stream
        // As the positions are stored in column major order, i.e column first.
        while( ss >> col )              // Loop until there are any integers left in the stream
        {
            ss >> row;
            p.first = row;      
            p.second = col;             // Make pair
            positions.push_back(p);     // Push to the vector (array)
        }
        ss.clear();                                 // Clear stream
        board = makeBoard(board, size);             // Initialize the memory and create board
        placeInitialQueens(board, positions);       // Place the Queens at the given board
        queensPlaced = positions.size();            // Number of Queens Placed in the board
        
        isSolved = false;
        isValid = baordIsValid(board, size);        // Check if the board with gievn Queens is valid?
        if(isValid)
            isSolved = solveNQueens(board, size, queensPlaced);  // If the board is Valid and the board is solved or not
       
        if( isSolved )                               // If Solved
        {
            printBoard(cout, board, size);                 // Print to console
            printBoard(outputFile, board, size);           // Print to file
        }
        else                                        // Either board is not valid or not solve able, No Solution exists
        {
            cout << "No solution\n";
            outputFile << "No solution\n";
        }
        positions.clear();                          // Clear the positions vector
        deleteBoard(board, size);                   // Deallocate the memory
    }

    inputFile.close();              // Close input file
    outputFile.close();             // Close output file
    return 0;
}
// End of main() function

// Declares memory and assign initial value
int** makeBoard(int** board, int size)
{
    int i, j;
    board = new int*[size+1];           // Initialize row memory
    for(i=0; i<=size; i++)
    {   
        board[i] = new int[size+1];     // Initialize column memory
        for(j=0; j<=size; j++)
            board[i][j] = EMPTY;        // Initialize Empty cells
    }
    return board;                       // Return Initialized Board
}

// Places the input Queens on the board
void placeInitialQueens(int **board, vector<pair<int, int>> positions)
{
    int i;
    for(i=0; i<positions.size(); i++)                                   // Loop until size of given pairs
    {
        board[positions[i].first][positions[i].second] = QUEEN;         // Place Queen on the grid
        board[positions[i].first][0] = OCCUPIED;                        // Mark row as occupied
        board[0][positions[i].second] = OCCUPIED;                       // Mark column as occupied
    }
}

// Prints the board to console
void printBoard(ostream& out, int** board, int size)
{
    int i, j;
    bool first = true;
    // Uncomment this code to display whole grid in the console
    
    cout << "    ";
    for(i=0; i<=size; i++)
        cout << i << "  ";
    cout << "\n------------------------------------\n";
    for(i=0; i<=size; i++)
    {
        cout << i << " | ";
        for(j=0; j<=size; j++)
            cout << board[i][j] << "  ";
        cout << '\n';
    }
    cout << '\n';
    
    for(i=1; i<=size; i++)
    {                                        // Writing to the file in column major order
        for(j=1; j<=size; j++)
        {
            if(board[j][i] == QUEEN)
            {
                if(!first)
                    out << ' ';
                out << i << ' ' << j;
                first = false;
            }
        }
    }
    out << '\n';
}

// Dealocates the memory, occupied by board
void deleteBoard(int** board, int size)
{
    int i;
    for(i=0; i<=size; i++)
        delete[] board[i];              // Delete rows
    delete[] board;                     // Delete Columns
}

// Main function to implement the algorithm
bool solveNQueens(int **board, int size, int queensPlaced)
{
    int i, j, k;                                    // Helper variables
    bool isBreak;
    stack<pair<int, int>> stack;                    // Stack of (row, column) pair
    pair<int, int> position;                        // A pair to store current position

    j = 1;
    for(i=1; i<=size;)                              // Loop until all Columns are placed
    {
        if(board[0][i] == OCCUPIED)                 // If the column is already OCCUPIED by a Queen, saves time complexity
        {
            i++;                                    // Next Column
            continue;
        }
        for(; j<=size; j++)                         // Loop until all Rows are covered of one column
        {
            if(board[j][0] == OCCUPIED)             // If the row is already OCCUPIED by a Queen, saves time complexity
                continue;                           // Next row
            /*
            Here we have already verified that no Queen is present in the current row and column
            Now the only thing to check is four of its diagonals.
            */
            if(checkDiagonal(board, size, j, i))    // Check if current cell is valid for placing a Queen
            {
                board[j][i] = QUEEN;                // Place the Queen in the current cell
                board[0][i] = OCCUPIED;             // Mark row as Occupied 
                board[j][0] = OCCUPIED;             // Mark column as Occupied
                position.first = j;                 // Make the position of the pair
                position.second = i;                // Column position
                stack.push(position);               // Push the pair to stack
                queensPlaced++;                     // Incrementing the count of Queens
                break;                              // Exit from this column
            }
        }

        if(queensPlaced == size)                    // If all the Queens are placed
            return true;                            // Board is completed

        /*
            If no valid cell is found in the current column, 
            then we need to go to previous Queen and change its position.
        */
        if( j > size )                              // If j has gone out of the bounds mean no Queen is placed in the next column
        {
            if(stack.empty())                       // Empty stack means that even the First Queen is removed and has gone out of bound
                return false;                       // It means that there exists no solution for this Board

            position = stack.top();                 // Otherwise pop() the previous Queen which was placed in the previous column
            stack.pop();
            j = position.first;                     // Assign previous row
            i = position.second;                    // Assign previous Column
            board[j][i] = EMPTY;                    // Remove the Queen from the board
            board[0][i] = EMPTY;                    // Mark Column as unoccupied
            board[j][0] = EMPTY;                    // Mark row as unoccupied
            j++;                                    // Start again by placing in the next row of previous column
            queensPlaced--;                         // Decrement Queen's count
        }
        /*
            Otherwise the Queen has placed in the perfect postion and 
            we can move to the next Queen.
        */
        else                
        {
            i++;                                    // Next Column
            j = 1;                                  // Start from first row
        }   
    }
    if(queensPlaced == size)                    // If all the Queens are placed
        return true;                            // Board is completed
    
    return false;                               // Cannot even place the first Queen correctly, -> No solution
}

// Checks if the board is valid, for cases in which the inpuut board is alread invalid
bool baordIsValid(int** board, int size)
{
    int i, j;
    bool isFound;           
    // Checking by rows, if any 2 Queens are in same row
    for(i=1; i<=size; i++)
    {
        isFound = false;
        for(j=1; j<=size; j++)
        {
            if(isFound && board[i][j] == QUEEN)     // Another Queen found in same Row
                return false;
            else if(board[i][j] == QUEEN)           // First Queen found in Row
                isFound = true;
        }
    }

    // Checking by columns, if any 2 Queens are in same column
    for(i=1; i<=size; i++)
    {
        isFound = false;
        for(j=1; j<=size; j++)
        {
            if(isFound && board[j][i] == QUEEN)     // Another Queen found in same Column
                return false;
            else if(board[j][i] == QUEEN)           // First Queen found in Column
                isFound = true;
        }
    }

    // Checking by Diagonal, if any 2 Queens are in same diagonl
    for(i=1; i<=size; i++)
        for(j=1; j<=size; j++)
            if(board[i][j] == QUEEN)                    // Queen found  
                if(!checkDiagonal(board, size, i, j))   // Check its diagonals
                    return false;                       // If found, return
                    
    return true;
}

// Checks the Diagonal of a Cell, before placing a Queen
bool checkDiagonal(int** board, int size, int r, int c)
{
    int i, j;
    // Check Left - Upper diagonal
    for(i=r-1, j=c-1; i>0 && j>0; j--, i--)
        if(board[i][j] == QUEEN)                // Found another Queen
            return false;

    // Check Right - Upper diagonal
    for(i=r-1, j=c+1; i>0 && j<=size; j++, i--)
        if(board[i][j] == QUEEN)                // Found another Queen
            return false;

    // Check Left - Downward diagonal
    for(i=r+1, j=c-1; i<=size && j>0; j--, i++)
        if(board[i][j] == QUEEN)                // Found another Queen
            return false;

    // Check Right - Downward diagonal
    for(i=r+1, j=c+1; i<=size && j<=size; j++, i++)
        if(board[i][j] == QUEEN)                // Found another Queen
            return false;

    return true;
}
