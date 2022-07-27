#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

// ============================================== //
// =            UART関連のポート設定             = //
// ============================================== //
#define  PORT_CH_UART_MOBC                 (0)  //!< RS-422 UART ch1
#define  PORT_CH_RS422_MOBC_EXT            (PORT_CH_UART_MOBC)  //!< FIXME: 共有 S2E でビルドするため

#define  PORT_CH_MAX_UART_PORT             (8)  //!< UARTポート上限

#endif
