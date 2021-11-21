#ifndef QUATERNION_H_
#define QUATERNION_H_

// TODO: このソースコードは現在管理されてません．数学ライブラリは現在別にて実装中です

// #define ADCS_PI   3.14159265358979323846
// #define ADCS_PI_2 1.57079632679489661923


void Quat2DCM(const float* q, float DCM[][3]);      // クォータニオンqに対応するDCMを作成
void DCM2Quat(const float DCM[][3], float* q);      // DCMに対応するクォータニオンqを作成
void Angle2Quat(float Z, float Y, float X, float* q); // 各軸まわりの回転角Z, Y, X(オイラー角)からクォータニオンqを作成
void QuatMul(const float* q1, const float* q2, float* ans);    // クォータニオン積q1*q2をansに格納
float QuatNorm(const float* q);                    // qのノルムを返す
int NormalizeQuat(const float* q, float* ans);      // qを正規化したものをansに格納
void Quat0Positive(const float* q, float* ans);     // qのスカラー成分を正にしたものをansに格納
void QuatInverse(const float* q, float* ans);       // qの逆クォータニオンをansに格納
// double ADCS_sin(double x);  // 三角関数の姿勢系実装（mathライブラリが有効化されていない場合に使う）
// double ADCS_cos(double x);  // 三角関数の姿勢系実装（mathライブラリが有効化されていない場合に使う）

#endif
