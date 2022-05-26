# SAMPLE_MOBC_TLM_DB_EL

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
STATISTICS.RECORD_COUNTER_TOTAL|uint32_t|event_logger->statistics.record_counter_total|26|0|32|||||||||||
STATISTICS.RECORD_COUNTERS_HIGH|uint32_t|event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]|30|0|32|||||||||||
STATISTICS.RECORD_COUNTERS_MIDDLE|uint32_t|event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]|34|0|32|||||||||||
STATISTICS.RECORD_COUNTERS_LOW|uint32_t|event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]|38|0|32|||||||||||
STATISTICS.RECORD_COUNTERS_EL|uint32_t|event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]|42|0|32|||||||||||
STATISTICS.RECORD_COUNTERS_EH|uint32_t|event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]|46|0|32|||||||||||
TLM_INFO.TLOG.PAGE_NO|uint8_t|event_logger->tlm_info.tlog.page_no|50|0|8|||||||||||
TLM_INFO.TLOG.ERR_LEVEL|uint8_t|(uint8_t)event_logger->tlm_info.tlog.err_level|51|0|8||5.1|||||||||
TLM_INFO.CLOG.PAGE_NO|uint8_t|event_logger->tlm_info.clog.page_no|52|0|8|||||||||||
TLM_INFO.CLOG.ERR_LEVEL|uint8_t|(uint8_t)event_logger->tlm_info.clog.err_level|53|0|8||5.1|||||||||
LATEST_EVENT.GROUP|uint32_t|(uint32_t)event_logger->latest_event.group|54|0|32||||||||||他はu8だがここはわざとu32で|
LATEST_EVENT.LOCAL|uint32_t|event_logger->latest_event.local|58|0|32|||||||||||
LATEST_EVENT.ERR_LEVEL|uint8_t|(uint8_t)event_logger->latest_event.err_level|62|0|8||5.1|||||||||
LATEST_EVENT.TIME.TOTAL_CYCLE|uint32_t|event_logger->latest_event.time.total_cycle|63|0|32|||||||||||
LATEST_EVENT.TIME.STEP|uint8_t|(uint8_t)event_logger->latest_event.time.step|67|0|8|||||||||||
LATEST_EVENT.NOTE|uint32_t|event_logger->latest_event.note|68|0|32||||||||||他はu8だがここはわざとu32で|
IS_LOGGING_ENABLE0|uint8_t|event_logger->is_logging_enable[0]|72|0|1|||||||||||
IS_LOGGING_ENABLE1|||72|1|1|||||||||||
IS_LOGGING_ENABLE2|||72|2|1|||||||||||
IS_LOGGING_ENABLE3|||72|3|1|||||||||||
IS_LOGGING_ENABLE4|||72|4|1|||||||||||
IS_LOGGING_ENABLE5|||72|5|1|||||||||||
IS_LOGGING_ENABLE6|||72|6|1|||||||||||
IS_LOGGING_ENABLE7|||72|7|1|||||||||||
IS_LOGGING_ENABLE8|uint8_t|event_logger->is_logging_enable[1]|73|0|1|||||||||||
IS_LOGGING_ENABLE9|||73|1|1|||||||||||
IS_LOGGING_ENABLE10|||73|2|1|||||||||||
IS_LOGGING_ENABLE11|||73|3|1|||||||||||
IS_LOGGING_ENABLE12|||73|4|1|||||||||||
IS_LOGGING_ENABLE13|||73|5|1|||||||||||
IS_LOGGING_ENABLE14|||73|6|1|||||||||||
IS_LOGGING_ENABLE15|||73|7|1|||||||||||
IS_LOGGING_ENABLE16|uint8_t|event_logger->is_logging_enable[2]|74|0|1|||||||||||
IS_LOGGING_ENABLE17|||74|1|1|||||||||||
IS_LOGGING_ENABLE18|||74|2|1|||||||||||
IS_LOGGING_ENABLE19|||74|3|1|||||||||||
IS_LOGGING_ENABLE20|||74|4|1|||||||||||
IS_LOGGING_ENABLE21|||74|5|1|||||||||||
IS_LOGGING_ENABLE22|||74|6|1|||||||||||
IS_LOGGING_ENABLE23|||74|7|1|||||||||||
IS_LOGGING_ENABLE24|uint8_t|event_logger->is_logging_enable[3]|75|0|1|||||||||||
IS_LOGGING_ENABLE25|||75|1|1|||||||||||
IS_LOGGING_ENABLE26|||75|2|1|||||||||||
IS_LOGGING_ENABLE27|||75|3|1|||||||||||
IS_LOGGING_ENABLE28|||75|4|1|||||||||||
IS_LOGGING_ENABLE29|||75|5|1|||||||||||
IS_LOGGING_ENABLE30|||75|6|1|||||||||||
IS_LOGGING_ENABLE31|||75|7|1|||||||||||
IS_LOGGING_ENABLE32|uint8_t|event_logger->is_logging_enable[4]|76|0|1|||||||||||
IS_LOGGING_ENABLE33|||76|1|1|||||||||||
IS_LOGGING_ENABLE34|||76|2|1|||||||||||
IS_LOGGING_ENABLE35|||76|3|1|||||||||||
IS_LOGGING_ENABLE36|||76|4|1|||||||||||
IS_LOGGING_ENABLE37|||76|5|1|||||||||||
IS_LOGGING_ENABLE38|||76|6|1|||||||||||
IS_LOGGING_ENABLE39|||76|7|1|||||||||||
IS_LOGGING_ENABLE40|uint8_t|event_logger->is_logging_enable[5]|77|0|1|||||||||||
IS_LOGGING_ENABLE41|||77|1|1|||||||||||
IS_LOGGING_ENABLE42|||77|2|1|||||||||||
IS_LOGGING_ENABLE43|||77|3|1|||||||||||
IS_LOGGING_ENABLE44|||77|4|1|||||||||||
IS_LOGGING_ENABLE45|||77|5|1|||||||||||
IS_LOGGING_ENABLE46|||77|6|1|||||||||||
IS_LOGGING_ENABLE47|||77|7|1|||||||||||
IS_LOGGING_ENABLE48|uint8_t|event_logger->is_logging_enable[6]|78|0|1|||||||||||
IS_LOGGING_ENABLE49|||78|1|1|||||||||||
IS_LOGGING_ENABLE50|||78|2|1|||||||||||
IS_LOGGING_ENABLE51|||78|3|1|||||||||||
IS_LOGGING_ENABLE52|||78|4|1|||||||||||
IS_LOGGING_ENABLE53|||78|5|1|||||||||||
IS_LOGGING_ENABLE54|||78|6|1|||||||||||
IS_LOGGING_ENABLE55|||78|7|1|||||||||||
IS_LOGGING_ENABLE56|uint8_t|event_logger->is_logging_enable[7]|79|0|1|||||||||||
IS_LOGGING_ENABLE57|||79|1|1|||||||||||
IS_LOGGING_ENABLE58|||79|2|1|||||||||||
IS_LOGGING_ENABLE59|||79|3|1|||||||||||
IS_LOGGING_ENABLE60|||79|4|1|||||||||||
IS_LOGGING_ENABLE61|||79|5|1|||||||||||
IS_LOGGING_ENABLE62|||79|6|1|||||||||||
IS_LOGGING_ENABLE63|||79|7|1|||||||||||
IS_LOGGING_ENABLE64|uint8_t|event_logger->is_logging_enable[8]|80|0|1|||||||||||
IS_LOGGING_ENABLE65|||80|1|1|||||||||||
IS_LOGGING_ENABLE66|||80|2|1|||||||||||
IS_LOGGING_ENABLE67|||80|3|1|||||||||||
IS_LOGGING_ENABLE68|||80|4|1|||||||||||
IS_LOGGING_ENABLE69|||80|5|1|||||||||||
IS_LOGGING_ENABLE70|||80|6|1|||||||||||
IS_LOGGING_ENABLE71|||80|7|1|||||||||||
IS_LOGGING_ENABLE72|uint8_t|event_logger->is_logging_enable[9]|81|0|1|||||||||||
IS_LOGGING_ENABLE73|||81|1|1|||||||||||
IS_LOGGING_ENABLE74|||81|2|1|||||||||||
IS_LOGGING_ENABLE75|||81|3|1|||||||||||
IS_LOGGING_ENABLE76|||81|4|1|||||||||||
IS_LOGGING_ENABLE77|||81|5|1|||||||||||
IS_LOGGING_ENABLE78|||81|6|1|||||||||||
IS_LOGGING_ENABLE79|||81|7|1|||||||||||
IS_LOGGING_ENABLE80|uint8_t|event_logger->is_logging_enable[10]|82|0|1|||||||||||
IS_LOGGING_ENABLE81|||82|1|1|||||||||||
IS_LOGGING_ENABLE82|||82|2|1|||||||||||
IS_LOGGING_ENABLE83|||82|3|1|||||||||||
IS_LOGGING_ENABLE84|||82|4|1|||||||||||
IS_LOGGING_ENABLE85|||82|5|1|||||||||||
IS_LOGGING_ENABLE86|||82|6|1|||||||||||
IS_LOGGING_ENABLE87|||82|7|1|||||||||||
IS_LOGGING_ENABLE88|uint8_t|event_logger->is_logging_enable[11]|83|0|1|||||||||||
IS_LOGGING_ENABLE89|||83|1|1|||||||||||
IS_LOGGING_ENABLE90|||83|2|1|||||||||||
IS_LOGGING_ENABLE91|||83|3|1|||||||||||
IS_LOGGING_ENABLE92|||83|4|1|||||||||||
IS_LOGGING_ENABLE93|||83|5|1|||||||||||
IS_LOGGING_ENABLE94|||83|6|1|||||||||||
IS_LOGGING_ENABLE95|||83|7|1|||||||||||
IS_LOGGING_ENABLE96|uint8_t|event_logger->is_logging_enable[12]|84|0|1|||||||||||
IS_LOGGING_ENABLE97|||84|1|1|||||||||||
IS_LOGGING_ENABLE98|||84|2|1|||||||||||
IS_LOGGING_ENABLE99|||84|3|1|||||||||||
IS_LOGGING_ENABLE100|||84|4|1|||||||||||
IS_LOGGING_ENABLE101|||84|5|1|||||||||||
IS_LOGGING_ENABLE102|||84|6|1|||||||||||
IS_LOGGING_ENABLE103|||84|7|1|||||||||||
IS_LOGGING_ENABLE104|uint8_t|event_logger->is_logging_enable[13]|85|0|1|||||||||||
IS_LOGGING_ENABLE105|||85|1|1|||||||||||
IS_LOGGING_ENABLE106|||85|2|1|||||||||||
IS_LOGGING_ENABLE107|||85|3|1|||||||||||
IS_LOGGING_ENABLE108|||85|4|1|||||||||||
IS_LOGGING_ENABLE109|||85|5|1|||||||||||
IS_LOGGING_ENABLE110|||85|6|1|||||||||||
IS_LOGGING_ENABLE111|||85|7|1|||||||||||
IS_LOGGING_ENABLE112|uint8_t|event_logger->is_logging_enable[14]|86|0|1|||||||||||
IS_LOGGING_ENABLE113|||86|1|1|||||||||||
IS_LOGGING_ENABLE114|||86|2|1|||||||||||
IS_LOGGING_ENABLE115|||86|3|1|||||||||||
IS_LOGGING_ENABLE116|||86|4|1|||||||||||
IS_LOGGING_ENABLE117|||86|5|1|||||||||||
IS_LOGGING_ENABLE118|||86|6|1|||||||||||
IS_LOGGING_ENABLE119|||86|7|1|||||||||||
IS_LOGGING_ENABLE120|uint8_t|event_logger->is_logging_enable[15]|87|0|1|||||||||||
IS_LOGGING_ENABLE121|||87|1|1|||||||||||
IS_LOGGING_ENABLE122|||87|2|1|||||||||||
IS_LOGGING_ENABLE123|||87|3|1|||||||||||
IS_LOGGING_ENABLE124|||87|4|1|||||||||||
IS_LOGGING_ENABLE125|||87|5|1|||||||||||
IS_LOGGING_ENABLE126|||87|6|1|||||||||||
IS_LOGGING_ENABLE127|||87|7|1|||||||||||
IS_LOGGING_ENABLE128|uint8_t|event_logger->is_logging_enable[16]|88|0|1|||||||||||
IS_LOGGING_ENABLE129|||88|1|1|||||||||||
IS_LOGGING_ENABLE130|||88|2|1|||||||||||
IS_LOGGING_ENABLE131|||88|3|1|||||||||||
IS_LOGGING_ENABLE132|||88|4|1|||||||||||
IS_LOGGING_ENABLE133|||88|5|1|||||||||||
IS_LOGGING_ENABLE134|||88|6|1|||||||||||
IS_LOGGING_ENABLE135|||88|7|1|||||||||||
IS_LOGGING_ENABLE136|uint8_t|event_logger->is_logging_enable[17]|89|0|1|||||||||||
IS_LOGGING_ENABLE137|||89|1|1|||||||||||
IS_LOGGING_ENABLE138|||89|2|1|||||||||||
IS_LOGGING_ENABLE139|||89|3|1|||||||||||
IS_LOGGING_ENABLE140|||89|4|1|||||||||||
IS_LOGGING_ENABLE141|||89|5|1|||||||||||
IS_LOGGING_ENABLE142|||89|6|1|||||||||||
IS_LOGGING_ENABLE143|||89|7|1|||||||||||
IS_LOGGING_ENABLE144|uint8_t|event_logger->is_logging_enable[18]|90|0|1|||||||||||
IS_LOGGING_ENABLE145|||90|1|1|||||||||||
IS_LOGGING_ENABLE146|||90|2|1|||||||||||
IS_LOGGING_ENABLE147|||90|3|1|||||||||||
IS_LOGGING_ENABLE148|||90|4|1|||||||||||
IS_LOGGING_ENABLE149|||90|5|1|||||||||||
IS_LOGGING_ENABLE150|||90|6|1|||||||||||
IS_LOGGING_ENABLE151|||90|7|1|||||||||||
IS_LOGGING_ENABLE152|uint8_t|event_logger->is_logging_enable[19]|91|0|1|||||||||||
IS_LOGGING_ENABLE153|||91|1|1|||||||||||
IS_LOGGING_ENABLE154|||91|2|1|||||||||||
IS_LOGGING_ENABLE155|||91|3|1|||||||||||
IS_LOGGING_ENABLE156|||91|4|1|||||||||||
IS_LOGGING_ENABLE157|||91|5|1|||||||||||
IS_LOGGING_ENABLE158|||91|6|1|||||||||||
IS_LOGGING_ENABLE159|||91|7|1|||||||||||
IS_LOGGING_ENABLE160|uint8_t|event_logger->is_logging_enable[20]|92|0|1|||||||||||
IS_LOGGING_ENABLE161|||92|1|1|||||||||||
IS_LOGGING_ENABLE162|||92|2|1|||||||||||
IS_LOGGING_ENABLE163|||92|3|1|||||||||||
IS_LOGGING_ENABLE164|||92|4|1|||||||||||
IS_LOGGING_ENABLE165|||92|5|1|||||||||||
IS_LOGGING_ENABLE166|||92|6|1|||||||||||
IS_LOGGING_ENABLE167|||92|7|1|||||||||||
IS_LOGGING_ENABLE168|uint8_t|event_logger->is_logging_enable[21]|93|0|1|||||||||||
IS_LOGGING_ENABLE169|||93|1|1|||||||||||
IS_LOGGING_ENABLE170|||93|2|1|||||||||||
IS_LOGGING_ENABLE171|||93|3|1|||||||||||
IS_LOGGING_ENABLE172|||93|4|1|||||||||||
IS_LOGGING_ENABLE173|||93|5|1|||||||||||
IS_LOGGING_ENABLE174|||93|6|1|||||||||||
IS_LOGGING_ENABLE175|||93|7|1|||||||||||
IS_LOGGING_ENABLE176|uint8_t|event_logger->is_logging_enable[22]|94|0|1|||||||||||
IS_LOGGING_ENABLE177|||94|1|1|||||||||||
IS_LOGGING_ENABLE178|||94|2|1|||||||||||
IS_LOGGING_ENABLE179|||94|3|1|||||||||||
IS_LOGGING_ENABLE180|||94|4|1|||||||||||
IS_LOGGING_ENABLE181|||94|5|1|||||||||||
IS_LOGGING_ENABLE182|||94|6|1|||||||||||
IS_LOGGING_ENABLE183|||94|7|1|||||||||||
IS_LOGGING_ENABLE184|uint8_t|event_logger->is_logging_enable[23]|95|0|1|||||||||||
IS_LOGGING_ENABLE185|||95|1|1|||||||||||
IS_LOGGING_ENABLE186|||95|2|1|||||||||||
IS_LOGGING_ENABLE187|||95|3|1|||||||||||
IS_LOGGING_ENABLE188|||95|4|1|||||||||||
IS_LOGGING_ENABLE189|||95|5|1|||||||||||
IS_LOGGING_ENABLE190|||95|6|1|||||||||||
IS_LOGGING_ENABLE191|||95|7|1|||||||||||
IS_LOGGING_ENABLE192|uint8_t|event_logger->is_logging_enable[24]|96|0|1|||||||||||
IS_LOGGING_ENABLE193|||96|1|1|||||||||||
IS_LOGGING_ENABLE194|||96|2|1|||||||||||
IS_LOGGING_ENABLE195|||96|3|1|||||||||||
IS_LOGGING_ENABLE196|||96|4|1|||||||||||
IS_LOGGING_ENABLE197|||96|5|1|||||||||||
IS_LOGGING_ENABLE198|||96|6|1|||||||||||
IS_LOGGING_ENABLE199|||96|7|1|||||||||||
IS_LOGGING_ENABLE200|uint8_t|event_logger->is_logging_enable[25]|97|0|1|||||||||||
IS_LOGGING_ENABLE201|||97|1|1|||||||||||
IS_LOGGING_ENABLE202|||97|2|1|||||||||||
IS_LOGGING_ENABLE203|||97|3|1|||||||||||
IS_LOGGING_ENABLE204|||97|4|1|||||||||||
IS_LOGGING_ENABLE205|||97|5|1|||||||||||
IS_LOGGING_ENABLE206|||97|6|1|||||||||||
IS_LOGGING_ENABLE207|||97|7|1|||||||||||
IS_LOGGING_ENABLE208|uint8_t|event_logger->is_logging_enable[26]|98|0|1|||||||||||
IS_LOGGING_ENABLE209|||98|1|1|||||||||||
IS_LOGGING_ENABLE210|||98|2|1|||||||||||
IS_LOGGING_ENABLE211|||98|3|1|||||||||||
IS_LOGGING_ENABLE212|||98|4|1|||||||||||
IS_LOGGING_ENABLE213|||98|5|1|||||||||||
IS_LOGGING_ENABLE214|||98|6|1|||||||||||
IS_LOGGING_ENABLE215|||98|7|1|||||||||||
IS_LOGGING_ENABLE216|uint8_t|event_logger->is_logging_enable[27]|99|0|1|||||||||||
IS_LOGGING_ENABLE217|||99|1|1|||||||||||
IS_LOGGING_ENABLE218|||99|2|1|||||||||||
IS_LOGGING_ENABLE219|||99|3|1|||||||||||
IS_LOGGING_ENABLE220|||99|4|1|||||||||||
IS_LOGGING_ENABLE221|||99|5|1|||||||||||
IS_LOGGING_ENABLE222|||99|6|1|||||||||||
IS_LOGGING_ENABLE223|||99|7|1|||||||||||
IS_LOGGING_ENABLE224|uint8_t|event_logger->is_logging_enable[28]|100|0|1|||||||||||
IS_LOGGING_ENABLE225|||100|1|1|||||||||||
IS_LOGGING_ENABLE226|||100|2|1|||||||||||
IS_LOGGING_ENABLE227|||100|3|1|||||||||||
IS_LOGGING_ENABLE228|||100|4|1|||||||||||
IS_LOGGING_ENABLE229|||100|5|1|||||||||||
IS_LOGGING_ENABLE230|||100|6|1|||||||||||
IS_LOGGING_ENABLE231|||100|7|1|||||||||||
IS_LOGGING_ENABLE232|uint8_t|event_logger->is_logging_enable[29]|101|0|1|||||||||||
IS_LOGGING_ENABLE233|||101|1|1|||||||||||
IS_LOGGING_ENABLE234|||101|2|1|||||||||||
IS_LOGGING_ENABLE235|||101|3|1|||||||||||
IS_LOGGING_ENABLE236|||101|4|1|||||||||||
IS_LOGGING_ENABLE237|||101|5|1|||||||||||
IS_LOGGING_ENABLE238|||101|6|1|||||||||||
IS_LOGGING_ENABLE239|||101|7|1|||||||||||
IS_LOGGING_ENABLE240|uint8_t|event_logger->is_logging_enable[30]|102|0|1|||||||||||
IS_LOGGING_ENABLE241|||102|1|1|||||||||||
IS_LOGGING_ENABLE242|||102|2|1|||||||||||
IS_LOGGING_ENABLE243|||102|3|1|||||||||||
IS_LOGGING_ENABLE244|||102|4|1|||||||||||
IS_LOGGING_ENABLE245|||102|5|1|||||||||||
IS_LOGGING_ENABLE246|||102|6|1|||||||||||
IS_LOGGING_ENABLE247|||102|7|1|||||||||||
IS_LOGGING_ENABLE248|uint8_t|event_logger->is_logging_enable[31]|103|0|1|||||||||||
IS_LOGGING_ENABLE249|||103|1|1|||||||||||
IS_LOGGING_ENABLE250|||103|2|1|||||||||||
IS_LOGGING_ENABLE251|||103|3|1|||||||||||
IS_LOGGING_ENABLE252|||103|4|1|||||||||||
IS_LOGGING_ENABLE253|||103|5|1|||||||||||
IS_LOGGING_ENABLE254|||103|6|1|||||||||||
IS_LOGGING_ENABLE255|||103|7|1|||||||||||
TLOGS.HIGH.EVENTS0.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group|104|0|8|||||||||||
TLOGS.HIGH.EVENTS0.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local|105|0|32|||||||||||
TLOGS.HIGH.EVENTS0.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle|109|0|32|||||||||||
TLOGS.HIGH.EVENTS0.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step|113|0|8|||||||||||
TLOGS.HIGH.EVENTS0.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note|114|0|32|||||||||ここぐらいはu32でおろす||
TLOGS.HIGH.EVENTS1.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->group|118|0|8|||||||||||
TLOGS.HIGH.EVENTS1.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->local|119|0|32|||||||||||
TLOGS.HIGH.EVENTS1.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->time.total_cycle|123|0|32|||||||||||
TLOGS.HIGH.EVENTS1.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->time.step|127|0|8|||||||||||
TLOGS.HIGH.EVENTS1.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->note|128|0|32|||||||||||
TLOGS.HIGH.EVENTS2.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->group|132|0|8|||||||||||
TLOGS.HIGH.EVENTS2.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->local|133|0|32|||||||||||
TLOGS.HIGH.EVENTS2.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->time.total_cycle|137|0|32|||||||||||
TLOGS.HIGH.EVENTS2.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->time.step|141|0|8|||||||||||
TLOGS.HIGH.EVENTS2.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->note|142|0|32|||||||||||
TLOGS.HIGH.EVENTS3.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 3)->group|146|0|8|||||||||||
TLOGS.HIGH.EVENTS3.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 3)->local|147|0|32|||||||||||
TLOGS.HIGH.EVENTS3.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 3)->time.total_cycle|151|0|32|||||||||||
TLOGS.HIGH.EVENTS3.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 3)->time.step|155|0|8|||||||||||
TLOGS.HIGH.EVENTS3.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 3)->note|156|0|32|||||||||||
TLOGS.MIDDLE.EVENTS0.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0)->group|160|0|8|||||||||||
TLOGS.MIDDLE.EVENTS0.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0)->local|161|0|32|||||||||||
TLOGS.MIDDLE.EVENTS0.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0)->time.total_cycle|165|0|32|||||||||||
TLOGS.MIDDLE.EVENTS0.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0)->time.step|169|0|8|||||||||||
TLOGS.MIDDLE.EVENTS0.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0)->note|170|0|32|||||||||||
TLOGS.MIDDLE.EVENTS1.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 1)->group|174|0|8|||||||||||
TLOGS.MIDDLE.EVENTS1.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 1)->local|175|0|32|||||||||||
TLOGS.MIDDLE.EVENTS1.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 1)->time.total_cycle|179|0|32|||||||||||
TLOGS.MIDDLE.EVENTS1.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 1)->time.step|183|0|8|||||||||||
TLOGS.MIDDLE.EVENTS1.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 1)->note|184|0|32|||||||||||
TLOGS.MIDDLE.EVENTS2.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 2)->group|188|0|8|||||||||||
TLOGS.MIDDLE.EVENTS2.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 2)->local|189|0|32|||||||||||
TLOGS.MIDDLE.EVENTS2.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 2)->time.total_cycle|193|0|32|||||||||||
TLOGS.MIDDLE.EVENTS2.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 2)->time.step|197|0|8|||||||||||
TLOGS.MIDDLE.EVENTS2.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 2)->note|198|0|32|||||||||||
TLOGS.MIDDLE.EVENTS3.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 3)->group|202|0|8|||||||||||
TLOGS.MIDDLE.EVENTS3.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 3)->local|203|0|32|||||||||||
TLOGS.MIDDLE.EVENTS3.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 3)->time.total_cycle|207|0|32|||||||||||
TLOGS.MIDDLE.EVENTS3.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 3)->time.step|211|0|8|||||||||||
TLOGS.MIDDLE.EVENTS3.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 3)->note|212|0|32|||||||||||
TLOGS.LOW.EVENTS0.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->group|216|0|8|||||||||||
TLOGS.LOW.EVENTS0.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->local|217|0|32|||||||||||
TLOGS.LOW.EVENTS0.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.total_cycle|221|0|32|||||||||||
TLOGS.LOW.EVENTS0.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.step|225|0|8|||||||||||
TLOGS.LOW.EVENTS0.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->note|226|0|32|||||||||||
TLOGS.LOW.EVENTS1.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->group|230|0|8|||||||||||
TLOGS.LOW.EVENTS1.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->local|231|0|32|||||||||||
TLOGS.LOW.EVENTS1.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->time.total_cycle|235|0|32|||||||||||
TLOGS.LOW.EVENTS1.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->time.step|239|0|8|||||||||||
TLOGS.LOW.EVENTS1.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->note|240|0|32|||||||||||
TLOGS.LOW.EVENTS2.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->group|244|0|8|||||||||||
TLOGS.LOW.EVENTS2.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->local|245|0|32|||||||||||
TLOGS.LOW.EVENTS2.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->time.total_cycle|249|0|32|||||||||||
TLOGS.LOW.EVENTS2.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->time.step|253|0|8|||||||||||
TLOGS.LOW.EVENTS2.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->note|254|0|32|||||||||||
TLOGS.LOW.EVENTS3.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 3)->group|258|0|8|||||||||||
TLOGS.LOW.EVENTS3.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 3)->local|259|0|32|||||||||||
TLOGS.LOW.EVENTS3.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 3)->time.total_cycle|263|0|32|||||||||||
TLOGS.LOW.EVENTS3.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 3)->time.step|267|0|8|||||||||||
TLOGS.LOW.EVENTS3.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 3)->note|268|0|32|||||||||||
TLOGS.LOW.EVENTS4.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 4)->group|272|0|8|||||||||||
TLOGS.LOW.EVENTS4.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 4)->local|273|0|32|||||||||||
TLOGS.LOW.EVENTS4.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 4)->time.total_cycle|277|0|32|||||||||||
TLOGS.LOW.EVENTS4.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 4)->time.step|281|0|8|||||||||||
TLOGS.LOW.EVENTS4.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 4)->note|282|0|32|||||||||||
TLOGS.LOW.EVENTS5.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 5)->group|286|0|8|||||||||||
TLOGS.LOW.EVENTS5.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 5)->local|287|0|32|||||||||||
TLOGS.LOW.EVENTS5.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 5)->time.total_cycle|291|0|32|||||||||||
TLOGS.LOW.EVENTS5.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 5)->time.step|295|0|8|||||||||||
TLOGS.LOW.EVENTS5.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 5)->note|296|0|32|||||||||||
TLOGS.EL.EVENTS0.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->group|300|0|8|||||||||||
TLOGS.EL.EVENTS0.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->local|301|0|32|||||||||||
TLOGS.EL.EVENTS0.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->time.total_cycle|305|0|32|||||||||||
TLOGS.EL.EVENTS0.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->time.step|309|0|8|||||||||||
TLOGS.EL.EVENTS0.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->note|310|0|32|||||||||||
TLOGS.EL.EVENTS1.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->group|314|0|8|||||||||||
TLOGS.EL.EVENTS1.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->local|315|0|32|||||||||||
TLOGS.EL.EVENTS1.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->time.total_cycle|319|0|32|||||||||||
TLOGS.EL.EVENTS1.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->time.step|323|0|8|||||||||||
TLOGS.EL.EVENTS1.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->note|324|0|32|||||||||||
TLOGS.EL.EVENTS2.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 2)->group|328|0|8|||||||||||
TLOGS.EL.EVENTS2.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 2)->local|329|0|32|||||||||||
TLOGS.EL.EVENTS2.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 2)->time.total_cycle|333|0|32|||||||||||
TLOGS.EL.EVENTS2.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 2)->time.step|337|0|8|||||||||||
TLOGS.EL.EVENTS2.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 2)->note|338|0|32|||||||||||
TLOGS.EL.EVENTS3.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 3)->group|342|0|8|||||||||||
TLOGS.EL.EVENTS3.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 3)->local|343|0|32|||||||||||
TLOGS.EL.EVENTS3.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 3)->time.total_cycle|347|0|32|||||||||||
TLOGS.EL.EVENTS3.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 3)->time.step|351|0|8|||||||||||
TLOGS.EL.EVENTS3.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 3)->note|352|0|32|||||||||||
TLOGS.EH.EVENTS0.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->group|356|0|8|||||||||||
TLOGS.EH.EVENTS0.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->local|357|0|32|||||||||||
TLOGS.EH.EVENTS0.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->time.total_cycle|361|0|32|||||||||||
TLOGS.EH.EVENTS0.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->time.step|365|0|8|||||||||||
TLOGS.EH.EVENTS0.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->note|366|0|32|||||||||||
TLOGS.EH.EVENTS1.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 1)->group|370|0|8|||||||||||
TLOGS.EH.EVENTS1.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 1)->local|371|0|32|||||||||||
TLOGS.EH.EVENTS1.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 1)->time.total_cycle|375|0|32|||||||||||
TLOGS.EH.EVENTS1.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 1)->time.step|379|0|8|||||||||||
TLOGS.EH.EVENTS1.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 1)->note|380|0|32|||||||||||
TLOGS.EH.EVENTS2.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 2)->group|384|0|8|||||||||||
TLOGS.EH.EVENTS2.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 2)->local|385|0|32|||||||||||
TLOGS.EH.EVENTS2.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 2)->time.total_cycle|389|0|32|||||||||||
TLOGS.EH.EVENTS2.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 2)->time.step|393|0|8|||||||||||
TLOGS.EH.EVENTS2.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 2)->note|394|0|32|||||||||||
TLOGS.EH.EVENTS3.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 3)->group|398|0|8|||||||||||
TLOGS.EH.EVENTS3.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 3)->local|399|0|32|||||||||||
TLOGS.EH.EVENTS3.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 3)->time.total_cycle|403|0|32|||||||||||
TLOGS.EH.EVENTS3.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 3)->time.step|407|0|8|||||||||||
TLOGS.EH.EVENTS3.NOTE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 3)->note|408|0|32|||||||||||
