# SAMPLE_MOBC_TLM_DB_UART_TEST

Name|Type|Exp.|Octet Pos.|bit Pos.|bit Len.|HEX|Status|a0|a1|a2|a3|a4|a5|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
PH.VER|uint16_t||0|0|3|||||||||||
PH.TYPE|||0|3|1|||||||||||
PH.SH_FLAG|||0|4|1|||||||||||
PH.APID|||0|5|11|||||||||||
PH.SEQ_FLAG|uint16_t||2|0|2|||||||||||
PH.SEQ_COUNT|||2|2|14|||||||||||
PH.PACKET_LEN|uint16_t||4|0|16|||||||||||
SH.VER|uint8_t||6|0|8|||||||||||
SH.TI|uint32_t||7|0|32|||||||||||
SH.TLM_ID|uint8_t||11|0|8|o||||||||||
SH.GLOBAL_TIME|double||12|0|64|||||||||||
SH.ON_BOARD_SUBNET_TIME|uint32_t||20|0|32|||||||||||
SH.DEST_FLAGS|uint8_t||24|0|8|o||||||||||
SH.DR_PARTITION|uint8_t||25|0|8|||||||||||
DRIVER.REC_STATUS.RET_FROM_IF_RX|uint8_t|(uint8_t)(uart_test_instance->driver.super.config.rec_status_.ret_from_if_rx)|26|0|8|||||||||||
DRIVER.RX_COUNT|uint32_t|uart_test_instance->driver.super.config.rx_count_|27|0|32|||||||||||
DRIVER0.REC_STATUS.STATUS_CODE|uint8_t|(uint8_t)(uart_test_instance->driver.super.stream_config[0].rec_status_.status_code)|31|0|8||11.1|||||||||
DRIVER0.REC_STATUS.FIXED_FRAME_LEN|uint16_t|uart_test_instance->driver.super.stream_config[0].rec_status_.fixed_frame_len|32|0|16|||||||||||
DRIVER0.RX_FRAME_FIX_COUNT|uint32_t|uart_test_instance->driver.super.stream_config[0].rx_frame_fix_count_|34|0|32|||||||||||
DRIVER0.RX_FRAME_REC_LEN|uint16_t|uart_test_instance->driver.super.stream_config[0].rx_frame_rec_len_|38|0|16|||||||||||
DRIVER0.IS_RX_BUFFER_CARRY_OVER|uint8_t|uart_test_instance->driver.super.stream_config[0].is_rx_buffer_carry_over_|40|0|8|||||||||||
DRIVER1.REC_STATUS.STATUS_CODE|uint8_t|(uint8_t)(uart_test_instance->driver.super.stream_config[1].rec_status_.status_code)|41|0|8||11.1|||||||||
DRIVER1.REC_STATUS.FIXED_FRAME_LEN|uint16_t|uart_test_instance->driver.super.stream_config[1].rec_status_.fixed_frame_len|42|0|16|||||||||||
DRIVER1.RX_FRAME_FIX_COUNT|uint32_t|uart_test_instance->driver.super.stream_config[1].rx_frame_fix_count_|44|0|32|||||||||||
DRIVER1.RX_FRAME_REC_LEN|uint16_t|uart_test_instance->driver.super.stream_config[1].rx_frame_rec_len_|48|0|16|||||||||||
DRIVER1.IS_RX_BUFFER_CARRY_OVER|uint8_t|uart_test_instance->driver.super.stream_config[1].is_rx_buffer_carry_over_|50|0|8|||||||||||
