/**
 * E32-TTL-100 Transceiver Interface
 *
 * @author Bob Chen (bob-0505@gotmail.com)
 * @date 1 November 2017
 * https://github.com/Bob0505/E32-TTL-100
 */
#include <Thread.h>
#include <ThreadController.h>
#include "E32-TTL-100.h"

Thread phySerialSbusThread = Thread();

unsigned int sbusChlData[18];
byte sbusChlIdx = 0;
byte sbusChlBit = 0;
byte sbusPktBit = 0;
byte sbusPktData[25];
byte sbusPktPosition = 0;
byte sbusFailsafeAct = 0;
byte sbusLostFrame = 0;

void SwitchPhySerialBaud(BAUD_RATE_TYPE baud)
{
    if(Serial)
    {
        Serial.end();
    }
    if(baud == BAUD_RATE_100000)
    {
        Serial.begin(BAUD_RATE_100000, SERIAL_8E2);
    }else
    {
        Serial.begin(baud);
    }
    return;
}

void initPinMode()
{
    pinMode(LED_BUILTIN, OUTPUT);
    return;
}


void loraRecvHandleUavAct(uint8_t UavAct)
{
    switch(UavAct)
    {
        case 0x00:
            normalCtrlChlSet(992, 992, 272, 992, 1472, 1692);
            //Serial.println(F("Slave: I get control command for UAV(ARM OFF)!"));
            break;
        case 0x01:
            normalCtrlChlSet(992, 992, 272, 992, 1472, 306);
            //Serial.println(F("Slave: I get control command for UAV(ARM ON)!"));
            break;
        case 0x02:
            normalCtrlChlSet(992, 992, 992, 992, 1472, 1692);
            //Serial.println(F("Slave: I get control command for UAV(ARM ON)!"));
            break;
        default:
            //Serial.println(F("Slave: I get unknow command for UAV!"));
            break;
    }
}

void blinkLED()
{
    static bool LedStatus = LOW;
    digitalWrite(LED_BUILTIN, LedStatus);
    LedStatus = !LedStatus;
}

void init_sbus_channel(void)
{
    //initaial SBUS values
    sbusChlData[0] = 992; // row
    sbusChlData[1] = 992; // pitch
    sbusChlData[2] = 992-960; // throttle
    sbusChlData[3] = 992; // yaw
    // no use
    for(int idx = 4; idx<16; idx++)
    {
      sbusChlData[7] = 992-960;
    }
    sbusChlData[16] = 0;
    sbusChlData[17] = 0;
    sbusFailsafeAct = 0;
    sbusLostFrame = 0;
    return;
}
// r, p , t, y, m, arm
void normalCtrlChlSet(int row, int pitch, int thro, int yaw, int angle, int armStat)
{
    sbusChlData[0] = row;   // row
    sbusChlData[1] = pitch;   // pitch
    sbusChlData[2] = thro;   // throw
    sbusChlData[3] = yaw;   // yaw
    sbusChlData[4] = angle;  // 手動.自穩.定高
    sbusChlData[6] = armStat;   // arm
    return;
}

void initSbusPktData(void)
{
    for(sbusPktPosition = 0; sbusPktPosition < 25; sbusPktPosition++)
        sbusPktData[sbusPktPosition] = 0x00;  //Zero out packet data
}

void refreshSbusPktData(void)
{
    sbusPktBit = 0;
    sbusPktPosition = 0;
    sbusPktData[sbusPktPosition] = 0x0F;  //Start Byte
    sbusPktPosition++;

    for(sbusChlIdx = 0; sbusChlIdx < 16; sbusChlIdx++)
    {
        for(sbusChlBit = 0; sbusChlBit < 11; sbusChlBit++)
        {
            if(sbusPktBit > 7)
            {
                sbusPktBit = 0;  //If we just set bit 7 in a previous step, reset the packet bit to 0 and
                sbusPktPosition++;       //Move to the next packet byte
            }
            sbusPktData[sbusPktPosition] |= (((sbusChlData[sbusChlIdx]>>sbusChlBit) & 0x01)<<sbusPktBit);  //Downshift the channel data bit, then upshift it to set the packet data byte
            sbusPktBit++;
        }
    }

    if(sbusChlData[16] > 1023)
        sbusPktData[23] |= (1<<0);  //Any number above 1023 will set the digital servo bit
    if(sbusChlData[17] > 1023)
        sbusPktData[23] |= (1<<1);
    if(sbusLostFrame != 0)
        sbusPktData[23] |= (1<<2);          //Any number above 0 will set the lost frame and failsafe bits
    if(sbusFailsafeAct != 0)
        sbusPktData[23] |= (1<<3);

    sbusPktData[24] = 0x00;  //End byte
}

void sendSbusPacket(void)
{
#if 0
    unsigned long costTime;
    costTime = millis();
#endif
    initSbusPktData();
    refreshSbusPktData();
    Serial.write(sbusPktData, 25); //Send data to SBUS
    // cost 1-2 millis
#if 0
    Serial.print(F("Cost Time: "));
    costTime = millis() - costTime;
    Serial.println(costTime);
#endif      
    return;
}

void initSysThreadProc(void)
{
    delay(10);
    phySerialSbusThread.onRun(sendSbusPacket);
    phySerialSbusThread.setInterval(7);
    return; 
}

void runSysThreadProc(void)
{
    if(phySerialSbusThread.shouldRun())
    {
        phySerialSbusThread.run();
    }
    return;
}

void initSysPhySerial()
{
    //SwitchPhySerialBaud(BAUD_RATE_115200);
#if MSG_PRINT_DEBUG            
    Serial.println(F("System Role: Slave"));
#endif
    SwitchPhySerialBaud(BAUD_RATE_100000);
    init_sbus_channel();
    return;
}

//The setup function is called once at startup of the sketch
void setup()
{
    initPinMode();
    initSysPhySerial();
    initSysThreadProc();  // TDMA Handle
    
#if MSG_PRINT_DEBUG
    Serial.println(F("Init: Setup done!"));
#endif
    for(int val=0; val<500;val++){
        normalCtrlChlSet(992, 992, 320, 992, 1192, 306);
        sendSbusPacket();
        delay(7);
    }
    normalCtrlChlSet(992, 992, 320, 992, 1192, 1692);
}

// The loop function is called in an endless loop
void loop()
{
    runSysThreadProc();
    delay(random(20, 30));
}
