#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

// ============================================== //
// =            UART関連のポート設定             = //
// ============================================== //
#define  PORT_CH_RS422_MOBC_EXT            (0)  //!< RS-422 UART ch1
#define  PORT_CH_RS422_AOBC                (1)  //!< RS-422 UART ch2
#define  PORT_CH_RS422_SAMPLE2             (2)  //!< RS-422 UART ch3
#define  PORT_CH_RS422_SAMPLE3             (3)  //!< RS-422 UART ch4
#define  PORT_CH_RS422_SAMPLE4             (4)  //!< RS-422 UART ch5
#define  PORT_CH_RS422_SAMPLE5             (5)  //!< RS-422 UART ch6
#define  PORT_CH_RS422_SAMPLE6             (6)  //!< RS-422 UART ch7
#define  PORT_CH_RS422_SAMPLE7             (7)  //!< RS-422 UART ch8

#define  PORT_CH_UART_TEST                 (PORT_CH_RS422_AOBC)

#define  PORT_CH_MAX_UART_PORT             (8)  //!< UARTポート上限

// ============================================== //
// =            GPIO関連のポート設定             = //
// ============================================== //
#define   PORT_CH_GPIO_OUT_SAMPLE0         (0x0001)  //!< LVTTL OUT ch1
#define   PORT_CH_GPIO_OUT_SAMPLE1         (0x0002)  //!< LVTTL OUT ch2

#define   PORT_CH_GPIO_IN_SAMPLE0          (0x0001)  //!< LVTTL IN ch1
#define   PORT_CH_GPIO_IN_SAMPLE1          (0x0002)  //!< LVTTL IN ch2

// ============================================== //
// =            ADC関連のポート設定              = //
// ============================================== //
#define   PORT_CH_ADC_SAMPLE0              (0x00)  //!< ADC ch1
#define   PORT_CH_ADC_SAMPLE1              (0x01)  //!< ADC ch2

#endif
