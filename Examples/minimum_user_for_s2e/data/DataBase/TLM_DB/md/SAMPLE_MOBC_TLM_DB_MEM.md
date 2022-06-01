# SAMPLE_MOBC_TLM_DB_MEM

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
MEM.BEGIN|uint32_t|memory_dump->begin|26|0|32|o||||||||メモリ操作領域開始アドレス||
MEM.END|uint32_t|memory_dump->end|30|0|32|o||||||||メモリ操作領域終端アドレス||
MEM.ADU_SIZE|uint32_t|memory_dump->adu_size|34|0|32|||||||||メモリ操作領域サイズ = ADUサイズ|CCSDSパケットヘッダに設定される|
MEM.ADU_SEQ|uint16_t|memory_dump->adu_seq|38|0|16|||||||||ADUシーケンスカウンタ|CCSDSパケットヘッダに設定される|
MEM.ADU_COUNTER|uint8_t|memory_dump->adu_counter|40|0|8|||||||||ADUカウンタ|CCSDSパケットヘッダに設定される|
MEM.DESTINATION|uint32_t|memory_dump->dest|41|0|32|o||||||||コピー先アドレス||
MEM.RP|uint32_t|memory_dump->rp|45|0|32|o||||||||コーピー用RP||
