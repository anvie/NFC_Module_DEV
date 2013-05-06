#include <PN532.h>
#include <NFCLinkLayer.h>
#include <NDEFPushProtocol.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include <NdefMessage.h>

#define SCK 13
#define MOSI 11
#define SS 10
#define MISO 12

PN532 nfc(SCK, MISO, MOSI, SS);
NFCLinkLayer linkLayer(&nfc);
NDEFPushProtocol nppLayer(&linkLayer);


// This message shall be used to rx or tx 
// NDEF messages it shall never be released
#define MAX_PKT_HEADER_SIZE  50
#define MAX_PKT_PAYLOAD_SIZE 100
uint8_t txNDEFMessage[MAX_PKT_HEADER_SIZE + MAX_PKT_PAYLOAD_SIZE];
uint8_t *txNDEFMessagePtr; 
uint8_t txLen;

#define SHORT_RECORD_TYPE_LEN   0x0A
#define NDEF_SHORT_RECORD_MESSAGE_HDR_LEN   0x03 + SHORT_RECORD_TYPE_LEN
#define TYPE_STR "text/plain"

void phoneInRange()
{
  //sleep_disable(); // Prevents the arduino from going to sleep if it was about too. 
}

void setup(void) {
    Serial.begin(115200);
    Serial.println("----------------- nfc ndef demo --------------------");


    txNDEFMessagePtr = &txNDEFMessage[MAX_PKT_HEADER_SIZE];
    NdefMessage message = NdefMessage();
    message.addUriRecord("http://seeedstudio.com");
    txLen = message.getEncodedSize();
    if (txLen <= MAX_PKT_PAYLOAD_SIZE) {
      message.encode(txNDEFMessagePtr);
    } else {
      Serial.println("Tx Buffer is too small.");
      while (1) {
      }
    }
    
    
    nfc.initializeReader();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
        Serial.print("Didn't find PN53x board");
        while (1); // halt
    }
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    Serial.print("Supports "); Serial.println(versiondata & 0xFF, HEX);
    
    // set power sleep mode
    set_sleep_mode(SLEEP_MODE_ADC);

    nfc.SAMConfig();
    
    // interrupt to wake MCU
    attachInterrupt(0, phoneInRange, FALLING);
}

uint8_t buf[180];

void loop(void) 
{
   Serial.println();
   Serial.println(F("---------------- LOOP ----------------------"));
   Serial.println();


    uint32_t txResult = GEN_ERROR;
    
     if (IS_ERROR(nfc.configurePeerAsTarget(NPP_SERVER))) {
        sleepMCU();
//        nfc.configurePeerAsTarget(NPP_SERVER);
        PN532_CMD_RESPONSE *response = (PN532_CMD_RESPONSE *)buf;
        nfc.readspicommand(PN532_TGINITASTARGET, response);
     }
    
    do {
  
        txResult = nppLayer.pushPayload(txNDEFMessagePtr, txLen);
   

        Serial.print(F("Result: 0x"));

        Serial.println(txResult, HEX);     
     
        delay(3000);   
     } while(0);
     
     
}


uint32_t createNDEFShortRecord(uint8_t *message, uint8_t payloadLen, uint8_t *&NDEFMessage)
{
   //Serial.print("Message: ");
   //Serial.println((char *)message);
   uint8_t * NDEFMessageHdr = ALLOCATE_HEADER_SPACE(NDEFMessage, NDEF_SHORT_RECORD_MESSAGE_HDR_LEN);
   
   NDEFMessageHdr[0] =  NDEF_MESSAGE_BEGIN_FLAG | NDEF_MESSAGE_END_FLAG | NDEF_MESSAGE_SHORT_RECORD | TYPE_FORMAT_MEDIA_TYPE; 
   NDEFMessageHdr[1] =  SHORT_RECORD_TYPE_LEN;
   NDEFMessageHdr[2] =  payloadLen;
   memcpy(&NDEFMessageHdr[3], TYPE_STR, SHORT_RECORD_TYPE_LEN);
   memcpy(NDEFMessage, message, payloadLen);
   //Serial.print("NDEF Message: ");
   //Serial.println((char *)NDEFMessage);   
   NDEFMessage = NDEFMessageHdr;
   return (payloadLen + NDEF_SHORT_RECORD_MESSAGE_HDR_LEN);   
}


void sleepMCU()
{
    delay(100);  // delay so that debug message can be printed before the MCU goes to sleep
    
    // Enable sleep mode
    sleep_enable();          
    
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();
    
    //Serial.println("Going to Sleep\n");
    //delay(1000);
    
    // Puts the device to sleep.
    sleep_mode();  
    Serial.println("Woke up");          
    
    // Program continues execution HERE
    // when an interrupt is recieved.

    // Disable sleep mode
    sleep_disable();         
    
    
    power_all_enable();
}   
   
