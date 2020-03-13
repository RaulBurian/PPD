
#define SIZE 200

void printMatrix(int **a);
void multiplyOneThread(int** a, int** b, int** result);
void addOneThread(int** a, int** b, int** result);
void multiplyNThread(int** a, int** b, int** result, int lineNr);
void addNThread(int** a, int** b, int** result, int lineNr);
void addElemThread(int** a, int** b, int** result, int lineNr, int colNr);
void multiplyElemThread(int** a, int** b, int** result, int lineNr, int colNr);