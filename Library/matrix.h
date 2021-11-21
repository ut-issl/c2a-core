#ifndef MATRIX_H_
#define MATRIX_H_
// TODO: このソースコードは現在管理されてません．数学ライブラリは現在別にて実装中です


#define DIM 3

// 3x3行列の計算
extern void MatrixVector(float A[][DIM], float* b, float* c, float C);       // c = Ab
extern void MatrixMatrix(float A[][DIM], float B[][DIM], float C[][DIM]);   // C = AB
int MatrixInverse(float A[][DIM], float B[][DIM]);

// それ以外
void nonSquareMatrixDot(float* A, float* B, float* C, int n, int m, int l);

// EKF用
void trans36(float A[3][6], float B[6][3]);
void trans63(float A[6][3], float B[3][6]);
void trans66(float A[6][6], float B[6][6]);
void mult6663(float A[6][6], float B[6][3], float C[6][3]);
void mult3663(float A[3][6], float B[6][3], float C[3][3]);
void mult3336(float A[3][3], float B[3][6], float C[3][6]);
void mult6333(float A[6][3], float B[3][3], float C[6][3]);
void mult6336(float A[6][3], float B[3][6], float C[6][6]);
void mult6666(float A[6][6], float B[6][6], float C[6][6]);
void multMatvec633(float A[6][3], float b[3], float c[6]);
void multMatvec444(float A[4][4], float b[4], float c[4]);
void multCMat44(float a, float B[4][4], float C[4][4]);
void multCMat66(float a, float B[6][6], float C[6][6]);
void unitalize66(float A[6][6]);
void MatrixAdd33(float A[3][3], float B[3][3], float res[3][3]);
void MatrixAdd66(float A[6][6], float B[6][6], float C[6][6]);
void add3(float* a, float* b, float* temp);
void add4(float* a, float* b, float* temp);

// RW用
void TransMat34(float A[3][4], float A_t[4][3]);
void mult3443(float A[3][4], float B[4][3], float C[3][3]);
void mult4334(float A[4][3], float B[3][4], float C[4][4]);
void mult4333(float A[4][3], float B[3][3], float C[4][3]);
void MatrixSub44(float A[4][4], float B[4][4], float C[4][4]);
void sub4(float* a, float* b, float* temp);

#endif
