/**
 * @file
 * @brief  3重冗長用汎用多数決関数
 */
#ifndef MAJORITY_VOTE_FOR3_H_
#define MAJORITY_VOTE_FOR3_H_

#include <stdio.h>
#include <stdint.h>

typedef enum
{
  MAJORITY_VOTE_MATCH    =  0,
  MAJORITY_VOTE_MAJORITY =  1,
  MAJORITY_VOTE_ERROR    =  2
} MAJORITY_VOTE_FOR3_STATUS;

/**
 * @brief 3重冗長用汎用多数決関数 (コピー)
 *
 *        3重冗長用配列を受け取って多数決を行った結果をresに入れ, 冗長の状態を返す
 * @param[in] array1_arg ~ array3_arg 判定したい3重冗長
 * @param[in] type_len sizeof(型)を入れる
 * @param[in] length 配列の長さ, 多次元でも実体は1次元なので全長を入れる, 単体の場合は1を
 * @param[out] res_arg 返り値のポインタ, あらかじめ確保しておくように
 * @retval MAJORITY_VOTE_ERROR:    全不一致が少なくとも1つ存在, 異常
 * @retval MAJORITY_VOTE_MAJORITY: 2:1が少なくとも1つ存在,     正常
 * @retval MAJORITY_VOTE_MATCH:    全てについて全一致,         正常
 * @note 配列に対して行われるので, 複数のindexで多数決が行われていてもMAJORITY_VOTE_MAJORITYが返ります.
 */
MAJORITY_VOTE_FOR3_STATUS majority_vote_for3_copy(void* res_arg,
                                                  void* array1_arg,
                                                  void* array2_arg,
                                                  void* array3_arg,
                                                  size_t type_len,
                                                  uint32_t length);

/**
 * @brief 3重冗長用汎用多数決関数 (同期)
 *
 *        3重冗長用配列を受け取って多数決を行い、全てを一致させた状態にする
 * @param[in] array1_arg ~ array3_arg 判定したい3重冗長
 * @param[in] type_len sizeof(型)を入れる
 * @param[in] length 配列の長さ, 多次元でも実体は1次元なので全長を入れる, 単体の場合は1を
 * @retval MAJORITY_VOTE_ERROR:    全不一致が少なくとも1つ存在, 異常
 * @retval MAJORITY_VOTE_MAJORITY: 2:1が少なくとも1つ存在,      正常
 * @retval MAJORITY_VOTE_MATCH:    全てについて全一致,          正常
 * @note 配列に対して行われるので, 複数のindexで多数決が行われていてもMAJORITY_VOTE_MAJORITYが返ります.
 *       また, これを使えば同期されるので三重冗長されたものの [0] のポインタを取って操作できます.
 */
MAJORITY_VOTE_FOR3_STATUS majority_vote_for3_sync(void* array1_arg,
                                                  void* array2_arg,
                                                  void* array3_arg,
                                                  size_t type_len,
                                                  uint32_t length);

#endif
