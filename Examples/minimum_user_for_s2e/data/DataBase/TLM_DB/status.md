# Status Table

Length|Index|Status
-|-|-
2|1|{'0': '7bit', '1': '8bit'}
2|2|{'0': 'CCSDS', '1': 'UART'}
2|3|{'0': 'DIS', '1': 'ENA'}
2|4|{'0': 'DISABLE', '1': 'ENABLE', '*': 'N/A'}
2|5|{'0': 'FALSE', '1': 'TRUE'}
2|6|{'0': 'FALSE:G', '1': 'TRUE:R'}
2|7|{'0': 'FINISHED', '1': 'PROGRESS', '*': 'N/A'}
2|8|{'0': 'INACTIVE', '1': 'ACTIVE', '*': 'N/A'}
2|9|{'0': 'INVALID', '1': 'VALID'}
2|10|{'0': 'NO', '1': 'YES'}
2|11|{'0': 'NO', '1': 'YES', '*': 'N/A'}
2|12|{'0': 'OK', '1': 'ERROR'}
2|13|{'0': 'SUCCESS', '1': 'ERROR'}
2|14|{'0': 'SUCCESS:G', '1': 'ERROR:R'}
3|1|{'0': '1bit', '1': '1.5bit', '2': '2bit'}
3|2|{'0': 'AD', '2': 'BD', '3': 'BC', '*': 'UNKNOWN'}
3|3|{'0': 'SINGLE', '1': 'CONTINUOUS', '2': 'CUMULATIVE', '*': 'N/A'}
3|4|{'1': 'REALTIME', '2': 'REPLAY', '63': 'FILL', '*': 'UNKNOWN'}
4|1|{'0': 'FINISHED', '1': 'PROGRESS', '2': 'ABORTED_BY_ERR', '3': 'ABORTED_BY_CMD', '*': 'N/A'}
5|1|{'0': 'HIGH', '1': 'MIDDLE', '2': 'LOW', '3': 'EL', '4': 'EH', '*': 'N/A'}
5|2|{'0': 'ME', '1': 'MOBC', '2': 'AOBC', '3': 'TOBC', '4': 'UNKNOWN'}
5|3|{'0': 'NONE', '1': 'ODD', '2': 'EVEN', '3': 'MARK', '4': 'SPACE'}
6|1|{'0': 'MASTER', '1': 'HK_TLM', '2': 'SYSTEM_TLM', '3': 'HIGH_FREQ_TLM', '4': 'LOW_FREQ_TLM', '5': 'RESERVE'}
6|2|{'0': 'NONE', '1': '1BYTE', '2': '2BYTE', '3': '4BYTE', '4': '8BYTE', '15': 'RAW', '*': 'N/A'}
7|1|{'0': 'SUC', '1': 'LEN', '2': 'PRM', '3': 'CNT', '4': 'NDF', '5': 'RUT', '6': 'FMT', '*': 'ERR'}
11|1|{'0': 'FINDING_HEADER', '1': 'RECEIVING_HEADER', '2': 'RECEIVING_FRAMELENGTH', '3': 'RECEIVING_DATA', '4': 'RECEIVING_FOOTER', '5': 'FIXED_FRAME', '6': 'DISABLE', '7': 'RX_FRAME_TOO_LONG', '8': 'RX_ERR', '9': 'VALIDATE_ERR', '10': 'OTHER_ERR'}
13|1|{'0': 'SUCCESS', '1': 'FORWARDED', '2': 'LIST_FULL', '3': 'PACKET_NOT_FOUND', '4': 'INVALID_PACKET', '5': 'TLC_SUCCESS', '6': 'TLC_PAST_TIME', '7': 'TLC_ALREADY_EXISTS', '8': 'BC_SUCCESS', '9': 'BC_INVALID_BLOCK_NO', '10': 'BC_ISORATED_CMD', '11': 'BC_CMD_TOO_LONG', '12': 'UNKNOWN'}
17|1|{'0': 'START_UP', '1': 'INITIAL', '2': 'GND_TEST', '3': 'WDFAR', '4': 'RTC_HIGH_RATE', '5': '5', '6': '6', '7': '7', '8': '8', '9': '9', '10': '10', '11': '11', '12': '12', '13': '13', '14': '14', '15': '15', '16': 'MAX', '*': 'N/A'}
18|1|{'0': 'SUCCESS', '1': 'TCTF_VER', '2': 'TCTF_SCID', '3': 'TCTF_VCID', '4': 'TCTF_TYPE', '5': 'LOCKOUT', '6': 'TCTF_SEQ_FLAG', '7': 'TCP_VER', '8': 'TCP_2ND_HDR_FLAG', '9': 'APID', '10': 'TCP_TYPE_IS_NOT_CMD', '11': 'TCP_SEQ_IS_NOTE_SINGLE', '12': 'FECW_MISSMATCH', '13': 'FARM1_PWA', '14': 'FARM1_NWA', '15': 'FARM1_LOCKOUT_AREA', '16': 'FARM1_SAME_NUMBER', '17': 'INVALID_BC_CMD'}
