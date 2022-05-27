# SAMPLE_MOBC_TLM_DB_EH_INDEX

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
TLM_INFO.RULE_SORTED_INDEX.PAGE_NO|uint8_t|event_handler->tlm_info.rule_sorted_index.page_no|26|0|8|||||||||||
IDX0.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[0 + offset].group|27|0|8|||||||||||
IDX0.LOCAL|uint32_t|event_handler->sorted_idxes[0 + offset].local|28|0|32|||||||||||
IDX0.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[0 + offset].duplicate_id|32|0|8|||||||||||
IDX0.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[0 + offset].rule_id|33|0|8|||||||||||
IDX1.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[1 + offset].group|34|0|8|||||||||||
IDX1.LOCAL|uint32_t|event_handler->sorted_idxes[1 + offset].local|35|0|32|||||||||||
IDX1.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[1 + offset].duplicate_id|39|0|8|||||||||||
IDX1.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[1 + offset].rule_id|40|0|8|||||||||||
IDX2.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[2 + offset].group|41|0|8|||||||||||
IDX2.LOCAL|uint32_t|event_handler->sorted_idxes[2 + offset].local|42|0|32|||||||||||
IDX2.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[2 + offset].duplicate_id|46|0|8|||||||||||
IDX2.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[2 + offset].rule_id|47|0|8|||||||||||
IDX3.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[3 + offset].group|48|0|8|||||||||||
IDX3.LOCAL|uint32_t|event_handler->sorted_idxes[3 + offset].local|49|0|32|||||||||||
IDX3.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[3 + offset].duplicate_id|53|0|8|||||||||||
IDX3.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[3 + offset].rule_id|54|0|8|||||||||||
IDX4.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[4 + offset].group|55|0|8|||||||||||
IDX4.LOCAL|uint32_t|event_handler->sorted_idxes[4 + offset].local|56|0|32|||||||||||
IDX4.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[4 + offset].duplicate_id|60|0|8|||||||||||
IDX4.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[4 + offset].rule_id|61|0|8|||||||||||
IDX5.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[5 + offset].group|62|0|8|||||||||||
IDX5.LOCAL|uint32_t|event_handler->sorted_idxes[5 + offset].local|63|0|32|||||||||||
IDX5.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[5 + offset].duplicate_id|67|0|8|||||||||||
IDX5.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[5 + offset].rule_id|68|0|8|||||||||||
IDX6.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[6 + offset].group|69|0|8|||||||||||
IDX6.LOCAL|uint32_t|event_handler->sorted_idxes[6 + offset].local|70|0|32|||||||||||
IDX6.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[6 + offset].duplicate_id|74|0|8|||||||||||
IDX6.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[6 + offset].rule_id|75|0|8|||||||||||
IDX7.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[7 + offset].group|76|0|8|||||||||||
IDX7.LOCAL|uint32_t|event_handler->sorted_idxes[7 + offset].local|77|0|32|||||||||||
IDX7.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[7 + offset].duplicate_id|81|0|8|||||||||||
IDX7.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[7 + offset].rule_id|82|0|8|||||||||||
IDX8.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[8 + offset].group|83|0|8|||||||||||
IDX8.LOCAL|uint32_t|event_handler->sorted_idxes[8 + offset].local|84|0|32|||||||||||
IDX8.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[8 + offset].duplicate_id|88|0|8|||||||||||
IDX8.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[8 + offset].rule_id|89|0|8|||||||||||
IDX9.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[9 + offset].group|90|0|8|||||||||||
IDX9.LOCAL|uint32_t|event_handler->sorted_idxes[9 + offset].local|91|0|32|||||||||||
IDX9.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[9 + offset].duplicate_id|95|0|8|||||||||||
IDX9.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[9 + offset].rule_id|96|0|8|||||||||||
IDX10.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[10 + offset].group|97|0|8|||||||||||
IDX10.LOCAL|uint32_t|event_handler->sorted_idxes[10 + offset].local|98|0|32|||||||||||
IDX10.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[10 + offset].duplicate_id|102|0|8|||||||||||
IDX10.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[10 + offset].rule_id|103|0|8|||||||||||
IDX11.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[11 + offset].group|104|0|8|||||||||||
IDX11.LOCAL|uint32_t|event_handler->sorted_idxes[11 + offset].local|105|0|32|||||||||||
IDX11.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[11 + offset].duplicate_id|109|0|8|||||||||||
IDX11.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[11 + offset].rule_id|110|0|8|||||||||||
IDX12.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[12 + offset].group|111|0|8|||||||||||
IDX12.LOCAL|uint32_t|event_handler->sorted_idxes[12 + offset].local|112|0|32|||||||||||
IDX12.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[12 + offset].duplicate_id|116|0|8|||||||||||
IDX12.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[12 + offset].rule_id|117|0|8|||||||||||
IDX13.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[13 + offset].group|118|0|8|||||||||||
IDX13.LOCAL|uint32_t|event_handler->sorted_idxes[13 + offset].local|119|0|32|||||||||||
IDX13.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[13 + offset].duplicate_id|123|0|8|||||||||||
IDX13.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[13 + offset].rule_id|124|0|8|||||||||||
IDX14.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[14 + offset].group|125|0|8|||||||||||
IDX14.LOCAL|uint32_t|event_handler->sorted_idxes[14 + offset].local|126|0|32|||||||||||
IDX14.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[14 + offset].duplicate_id|130|0|8|||||||||||
IDX14.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[14 + offset].rule_id|131|0|8|||||||||||
IDX15.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[15 + offset].group|132|0|8|||||||||||
IDX15.LOCAL|uint32_t|event_handler->sorted_idxes[15 + offset].local|133|0|32|||||||||||
IDX15.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[15 + offset].duplicate_id|137|0|8|||||||||||
IDX15.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[15 + offset].rule_id|138|0|8|||||||||||
IDX16.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[16 + offset].group|139|0|8|||||||||||
IDX16.LOCAL|uint32_t|event_handler->sorted_idxes[16 + offset].local|140|0|32|||||||||||
IDX16.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[16 + offset].duplicate_id|144|0|8|||||||||||
IDX16.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[16 + offset].rule_id|145|0|8|||||||||||
IDX17.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[17 + offset].group|146|0|8|||||||||||
IDX17.LOCAL|uint32_t|event_handler->sorted_idxes[17 + offset].local|147|0|32|||||||||||
IDX17.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[17 + offset].duplicate_id|151|0|8|||||||||||
IDX17.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[17 + offset].rule_id|152|0|8|||||||||||
IDX18.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[18 + offset].group|153|0|8|||||||||||
IDX18.LOCAL|uint32_t|event_handler->sorted_idxes[18 + offset].local|154|0|32|||||||||||
IDX18.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[18 + offset].duplicate_id|158|0|8|||||||||||
IDX18.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[18 + offset].rule_id|159|0|8|||||||||||
IDX19.GROUP|uint8_t|(uint8_t)event_handler->sorted_idxes[19 + offset].group|160|0|8|||||||||||
IDX19.LOCAL|uint32_t|event_handler->sorted_idxes[19 + offset].local|161|0|32|||||||||||
IDX19.DUPLICATE_ID|uint8_t|event_handler->sorted_idxes[19 + offset].duplicate_id|165|0|8|||||||||||
IDX19.RULE_ID|uint8_t|(uint8_t)event_handler->sorted_idxes[19 + offset].rule_id|166|0|8|||||||||||
