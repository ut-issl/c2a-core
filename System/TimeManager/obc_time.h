#ifndef OBC_TIME_H_
#define OBC_TIME_H_

#include <src_user/Library/stdint.h>

// step, cycle�ɂ��Ă�TimeManager���Q�Ƃ̂���
#define OBCT_STEP_IN_MSEC (1)                                                    //!< 1step�ŉ�ms��
#define OBCT_STEPS_PER_CYCLE (100)                                               //!< ��step��1cycle��
#define OBCT_CYCLES_PER_SEC (1000 / OBCT_STEP_IN_MSEC / OBCT_STEPS_PER_CYCLE)    //!< 1s�ŉ�cycle��
#define OBCT_MAX_CYCLE (0xfffffff0u)                                             //!< �ő�cycle���D�܂�TI�������ŃI�[�o�[�t���[���邩

#include <src_user/Settings/System/obc_time_params.h>

typedef uint32_t cycle_t;
typedef uint32_t step_t;

typedef struct
{
  cycle_t total_cycle;   // OBC���N�����Ă���i���m�ɂ́C�\���̂�����������Ă���j�̌o�ߎ��� (cycle)
  cycle_t mode_cycle;    // �Ō��mode��ς��Ă���̌o�ߎ��� (cycle)
  step_t  step;          // ����step�DTimeLine�ł̏������ԕ����Ɏg���� (step)
} ObcTime;

/**
 * @brief unixtime��ObcTime��R�Â���\����
 */
typedef struct
{
  double unixtime_at_ti0; //!< �ϑ���񂩂�v�Z����master_clock��(0, 0)�̎���unixtime
  cycle_t ti_at_last_update;  //!< �Ō�ɍX�V�������Ɏg����unixtime�̏���(GPS�Ȃǂ���)�ϑ���������ObcTime
} OBCT_UnixtimeInfo;

ObcTime OBCT_create(cycle_t total_cycle,
                    cycle_t mode_cycle,
                    step_t step);
ObcTime OBCT_get_max(void);
void OBCT_clear(ObcTime* time);
void OBCT_count_up(ObcTime* time);
cycle_t OBCT_get_total_cycle(const ObcTime* time);
cycle_t OBCT_get_mode_cycle(const ObcTime* time);
step_t  OBCT_get_step(const ObcTime* time);
uint32_t OBCT_get_total_cycle_in_msec(const ObcTime* time);  // �v�Z���step���l��
uint32_t OBCT_get_mode_cycle_in_msec(const ObcTime* time);   // �v�Z���step���l��
float    OBCT_get_total_cycle_in_sec(const ObcTime* time);   // �v�Z���step���l���i�I�[�o�[�t���[�ɒ��Ӂj
float    OBCT_get_mode_cycle_in_sec(const ObcTime* time);    // �v�Z���step���l���i�I�[�o�[�t���[�ɒ��Ӂj
cycle_t  OBCT_sec2cycle(uint32_t sec);        // �K���Ɋۂ߂��邱�Ƃɒ���
uint32_t OBCT_cycle2sec(cycle_t cycle);       // �K���Ɋۂ߂��邱�Ƃɒ���
cycle_t  OBCT_msec2cycle(uint32_t msec);      // �K���Ɋۂ߂��邱�Ƃɒ���
uint32_t OBCT_cycle2msec(cycle_t cycle);      // �K���Ɋۂ߂��邱�Ƃɒ���
ObcTime OBCT_diff(const ObcTime* before,
                  const ObcTime* after);
ObcTime OBCT_add(const ObcTime* left, const ObcTime* right);
step_t OBCT_diff_in_step(const ObcTime* before,
                         const ObcTime* after);
uint32_t OBCT_diff_in_msec(const ObcTime* before,
                               const ObcTime* after);
float OBCT_diff_in_sec(const ObcTime* before,
                       const ObcTime* after);

/**
 * @brief ObcTime �̔�r
 * @note  mode_cycle �݂͂Ȃ�
 * @param[in] t1: ��r�Ώ�
 * @param[in] t2: ��r�Ώ�
 * @return 1  : t1 < t2
 * @return 0  : t1 == t2
 * @return -1 : t2 > t1
 */
int OBCT_compare(const ObcTime* t1, const ObcTime* t2);

void OBCT_print(const ObcTime* time);

/**
 * @brief OBCT_UnixtimeInfo �쐬�֐�
 *
 *        �ϑ����� UnixTime �Ƃ�����ϑ��������� ObcTime ���󂯎�� OBCT_UnixtimeInfo �֕ς���.
 * @param[in] time (GPS ������) unixtime ���ϑ��������� ObcTime
 * @param[in] unixtime (GPS ������ϑ�����) unixtime
 * @return OBCT_UnixtimeInfo ��������v�Z���� OBCT_UnixtimeInfo
 */
OBCT_UnixtimeInfo OBCT_create_unixtime_info(const double unixtime, const ObcTime* time);
/**
 * @brief OBCT_UnixtimeInfo �N���A�֐�, �S��0�ɂ���
 * @param[in] uti �N���A���� OBCT_UnixtimeInfo
 */
void OBCT_clear_unixtime_info(OBCT_UnixtimeInfo* uti);
/**
 * @brief OBCT_UnixtimeInfo �C���֐�
 * @param[in] uti �C������ OBCT_UnixtimeInfo
 * @param[in] time (GPS ������) unixtime ���ϑ��������� ObcTime
 * @param[in] unixtime (GPS ������ϑ�����) unixtime
 */
void OBCT_modify_unixtime_info(OBCT_UnixtimeInfo* uti, const double unixtime, const ObcTime time);

#endif
