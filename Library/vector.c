#pragma section REPRO
#include "vector.h"

// C++にはmathf.hはない (math.hに統合されている。)
#include <src_user/Settings/sils_define.h>
#ifndef SILS_FW
#include <mathf.h>
#endif
#include <math.h>


// ベクトルvのノルムを返す
float VectorNorm(const float* v)
{
  return (float)sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

// ベクトルvを正規化したものをansに格納
int NormalizeVector(const float* v, float* ans)
{
  float norm = VectorNorm(v);
  if (norm < NEGLIGIBLE_VALUE)
  {
    ans[0] = v[0];
    ans[1] = v[1];
    ans[2] = v[2];
    return -1;
  }
  else
  {
    ans[0] = v[0] / norm;
    ans[1] = v[1] / norm;
    ans[2] = v[2] / norm;
    return 0;
  }
}
// ベクトルの内積v1・v2を返す
float VectorInnerProduct(const float* v1, const float* v2)
{
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
// ベクトルの外積v1×v2をansに格納
void VectorOuterProduct(const float* v1, const float* v2, float* ans)
{
  ans[0] = v1[1] * v2[2] - v1[2] * v2[1];
  ans[1] = v1[2] * v2[0] - v1[0] * v2[2];
  ans[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
// ベクトルのスカラー倍a*vをansに格納
void VectorScalarProduct(const float a, const float* v, float* ans)
{
  ans[0] = a * v[0];
  ans[1] = a * v[1];
  ans[2] = a * v[2];
}
// ベクトルの和v1+v2をansに格納
void VectorAdd(const float* v1, const float* v2, float* ans)
{
  ans[0] = v1[0] + v2[0];
  ans[1] = v1[1] + v2[1];
  ans[2] = v1[2] + v2[2];
}
// ベクトルの差v1-v2をansに格納
void VectorSub(const float* v1, const float* v2, float* ans)
{
  ans[0] = v1[0] - v2[0];
  ans[1] = v1[1] - v2[1];
  ans[2] = v1[2] - v2[2];
}
// ベクトルvの-1倍(反転)をansに格納
void VectorNeg(const float* v, float* ans)
{
  ans[0] = -v[0];
  ans[1] = -v[1];
  ans[2] = -v[2];
}
// ベクトルvの値をansにコピー
void VectorCopy(const float* v, float* ans)
{
  ans[0] = v[0];
  ans[1] = v[1];
  ans[2] = v[2];
}

// 0ベクトルであったら1を返す
extern char IsZeroVector(const float* v)
{

  if ((fabsf(v[0]) < NEGLIGIBLE_VALUE) && (fabsf(v[1]) < NEGLIGIBLE_VALUE) && (fabsf(v[2]) < NEGLIGIBLE_VALUE))
  {
    return 1;
  }
  return 0;
}

#pragma section
