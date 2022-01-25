/**
 * @file
 * @brief CCSDS で規定される Space Packet の実装
 * @note  資料: https://sma.jaxa.jp/TechDoc/Docs/JAXA-JERG-2-403.pdf
 * @note  C2A においては， CommonHogePacket の実体として，以下のように Space Packet を標準とする
 *        CommonTlmCmdPacket -> SpacePacket
 *        CommonTlmPacket    -> TlmSpacePacket
 *        CommonCmdPacket    -> CmdSpacePacket
 * @note  Packet Secondary Header は C2A 独自定義． いくつかのバージョンを用意している
 * @note  /Docs/Core/communication.md などを参照
 */
#ifndef SPACE_PACKET_H_
#define SPACE_PACKET_H_


#endif
