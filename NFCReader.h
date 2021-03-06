

#ifndef NFC_READER_H
#define NFC_READER_H

#include "Arduino.h"

#define NFC_READER_CFG_BAUDRATE_106_KPS   0
#define NFC_READER_CFG_BAUDRATE_201_KPS  1
#define NFC_READER_CFG_BAUDRATE_424_KPS   2


#define SNEP_CLIENT  1
#define SNEP_SERVER  2

enum RESULTS
{
   RESULT_SUCCESS = 0x00000001,
   GEN_ERROR = 0x80000000,
   CONFIGURE_HARDWARE_ERROR,
   NFC_READER_COMMAND_FAILURE,
   NFC_READER_RESPONSE_FAILURE,
   CONNECT_RX_FAILURE,
   CONNECT_TX_FAILURE,
   CONNECT_COMPLETE_RX_FAILURE,
   CONNECT_COMPLETE_TX_FAILURE,
   UNEXPECTED_PDU_FAILURE,
   NDEF_MESSAGE_RX_FAILURE,
   NDEF_MESSAGE_TX_FAILURE, // 0x0A
   SNEP_UNSUPPORTED_VERSION,
   SNEP_INVALID_NUM_ENTRIES,
   SNEP_INVALID_ACTION_CODE,
   SEND_COMMAND_TX_TIMEOUT_ERROR,
   SEND_COMMAND_RX_ACK_ERROR,
   SEND_COMMAND_RX_TIMEOUT_ERROR,
   INVALID_CHECKSUM_RX,
   INVALID_RESPONSE,
   INVALID_POSTAMBLE,
   CLIENT_REQ_ERROR

};


#define IS_ERROR(result) ((result) & 0x80000000)
#define RESULT_OK(result) !IS_ERROR(result)
class NFCReader {

public:
   virtual void initializeReader() = 0;
   virtual uint32_t SAMConfig(void) = 0;
   virtual uint32_t getFirmwareVersion(void) = 0;

   virtual uint32_t readPassiveTargetID(uint8_t cardbaudrate) = 0;
   virtual uint32_t authenticateBlock(	uint8_t cardnumber /*1 or 2*/,
				uint32_t cid /*Card NUID*/,
				uint8_t blockaddress /*0 to 63*/,
				uint8_t authtype /*Either KEY_A or KEY_B */,
				uint8_t * keys) = 0;

   virtual uint32_t readMemoryBlock(uint8_t cardnumber /*1 or 2*/,
                            uint8_t blockaddress /*0 to 63*/,
                            uint8_t * block) = 0;

   virtual uint32_t writeMemoryBlock(uint8_t cardnumber /*1 or 2*/,
                             uint8_t blockaddress /*0 to 63*/,
                             uint8_t * block);

   virtual uint32_t configurePeerAsInitiator(uint8_t baudrate) = 0;
   virtual uint32_t configurePeerAsTarget(uint8_t type) = 0;


   virtual uint32_t initiatorTxRxData(uint8_t *DataOut,
                                       uint32_t dataSize,
                                       uint8_t *response) = 0;

   virtual uint32_t targetRxData(uint8_t *response) = 0;
   virtual uint32_t targetRxData(uint8_t *response, uint32_t len) = 0;
   virtual uint32_t targetTxData(uint8_t *DataOut, uint32_t dataSize) = 0;

   virtual uint32_t getTargetStatus(uint8_t *statusOut) = 0;

	 virtual uint32_t sendCommandCheckAck(uint8_t *cmd,
                              uint8_t cmdlen,
                              uint16_t timeout = 1000) = 0;

   virtual boolean isTargetReleasedError(uint32_t result) = 0;

   static char* errorString(uint32_t code){
       switch (code){
           case GEN_ERROR: return "GEN_ERROR";
           case CONNECT_RX_FAILURE: return "CONNECT_RX_FAILURE";
           case CONNECT_TX_FAILURE: return "CONNECT_TX_FAILURE";
           case CONNECT_COMPLETE_RX_FAILURE: return "CONNECT_COMPLETE_RX_FAILURE";
           case CONNECT_COMPLETE_TX_FAILURE: return "CONNECT_COMPLETE_TX_FAILURE";
           case UNEXPECTED_PDU_FAILURE: return "UNEXPECTED_PDU_FAILURE";
           case NDEF_MESSAGE_RX_FAILURE: return "NDEF_MESSAGE_RX_FAILURE";
           case NDEF_MESSAGE_TX_FAILURE: return "NDEF_MESSAGE_TX_FAILURE";
           case SNEP_UNSUPPORTED_VERSION: return "SNEP_UNSUPPORTED_VERSION";
           /*GEN_ERROR = 0x80000000,
              CONFIGURE_HARDWARE_ERROR,
              NFC_READER_COMMAND_FAILURE,
              NFC_READER_RESPONSE_FAILURE,
              CONNECT_RX_FAILURE,
              CONNECT_TX_FAILURE,
              CONNECT_COMPLETE_RX_FAILURE,
              CONNECT_COMPLETE_TX_FAILURE,
              UNEXPECTED_PDU_FAILURE,
              NDEF_MESSAGE_RX_FAILURE,
              NDEF_MESSAGE_TX_FAILURE,
              SNEP_UNSUPPORTED_VERSION,
              SNEP_INVALID_NUM_ENTRIES,
              SNEP_INVALID_ACTION_CODE,
              SEND_COMMAND_TX_TIMEOUT_ERROR,
              SEND_COMMAND_RX_ACK_ERROR,
              SEND_COMMAND_RX_TIMEOUT_ERROR,
              INVALID_CHECKSUM_RX,
              INVALID_RESPONSE,
              INVALID_POSTAMBLE,
              CLIENT_REQ_ERROR
            */
              case SNEP_INVALID_ACTION_CODE: return "SNEP_INVALID_ACTION_CODE";
            case CONFIGURE_HARDWARE_ERROR: return "CONFIGURE_HARDWARE_ERROR";
            case NFC_READER_COMMAND_FAILURE: return "NFC_READER_COMMAND_FAILURE";
            case INVALID_CHECKSUM_RX: return "INVALID_CHECKSUM_RX";
            case INVALID_RESPONSE: return "INVALID_RESPONSE";
            case INVALID_POSTAMBLE: return "INVALID_POSTAMBLE";
            case CLIENT_REQ_ERROR: return "CLIENT_REQ_ERROR";
       }
       return "UNKNOWN";
   }
};


#define ALLOCATE_HEADER_SPACE(buffer, numHdrBytes)  &buffer[-((int)numHdrBytes)];
#endif // NFC_READER_H
