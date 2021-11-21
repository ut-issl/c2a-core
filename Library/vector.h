#ifndef VECTOR_H_
#define VECTOR_H_
// TODO: このソースコードは現在管理されてません．数学ライブラリは現在別にて実装中です

#define DIM 3
#define NEGLIGIBLE_VALUE    1.0e-5f

extern float VectorNorm(const float* v);                                        // ベクトルvのノルムを返す
extern int NormalizeVector(const float* v, float* ans);                          // ベクトルvを正規化したものをansに格納
extern float VectorInnerProduct(const float* v1, const float* v2);               // ベクトルの内積v1・v2を返す
extern void VectorOuterProduct(const float* v1, const float* v2,  float* ans);    // ベクトルの外積v1×v2をansに格納
extern void VectorScalarProduct(const float a, const float* v, float* ans);               // ベクトルのスカラー倍a*vをansに格納
extern void VectorAdd(const float* v1, const float* v2, float* ans);              // ベクトルの和v1+v2をansに格納
extern void VectorSub(const float* v1, const float* v2, float* ans);              // ベクトルの差v1-v2をansに格納
extern void VectorNeg(const float* v, float* ans);                               // ベクトルvの-1倍(反転)をansに格納
extern void VectorCopy(const float* v, float* ans);                             // ベクトルvの値をansにコピー
extern char IsZeroVector(const float* v);                                       // 0ベクトルであったら1を返す

#endif
