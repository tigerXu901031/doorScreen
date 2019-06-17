#include "task.h"

void taskInit()
{
    IO_INIT();
    delay_ms(200);
    INIT_LCD();
    clear_screen();

    /* need to consider to use cyclic task or not */

    P52 = 0;
    P53 = 0;
    networkInit();
    timerInit();
    timer10ms_Enable();
    timer1ms_Enable();
}

/* test purpose only */
const unsigned char testReadReqMsg[6] = {0x01, 0x03, 0xF1, 0x02, 0x11, 0x22};
const unsigned char testReadRepMsg[8] = {0xFF, 0x03, 0xF2, 0x02, 0xAA, 0x55, 0x23, 0x33};
const unsigned char testReadLongRepMsg[39] = {0xFF, 0x03, 0x30, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                              0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                              0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
                                              0x29, 0x30, 0x31, 0x32, 0xcc, 0xcc};
const unsigned char testWriteRepMsg[8] = {0x01, 0x06, 0xF0, 0x99, 0x32, 0x55, 0x55, 0xAA};
const unsigned char testReadFailMsg[8] = {0xFF, 0x83, 0xF0, 0x99, 0x88, 0x55, 0x55, 0xAA};
void task1ms()
{

    static unsigned char testCnt = 0;
    /* test purpose only
       to simulate an test message that set in the FIFO buffer cyclic */
    if(testCnt < sizeof(testReadFailMsg))
    {
        setFifoData(&uartRxFifo_Obj[busIdx_private], &testReadFailMsg[testCnt]);
        testCnt ++;
    }
    else
    {
        testCnt = 0;
    }
    
    /* 1ms send out data via uart seems very critial for 8bit send,
       maybe need to increase to 2ms later
       TODO: now its 2ms ---  */
    networkUpdate();
}

/* test purpose only */
unsigned char testSendByte[2] = {0x55, 0xaa};
unsigned char testRecByte[2] = {0, 0};

void task10ms()
{
    /* test purpose only */
    static networkDataBuf_type testNwData;
    static networkDataBuf_type testSetNwData;
    static unsigned char iCounter = 0;

    testSetNwData.dataLength =1;
    testSetNwData.networkData[0].cmd = 0x03;
    testSetNwData.networkData[0].add[0] = 0xf1;
    testSetNwData.networkData[0].add[1] = 0xf2;
    testSetNwData.networkData[0].opData[0] = 0x13;
    testSetNwData.networkData[0].opData[1] = 0x22;    
    /* some test for FIFO buffer + uart drvier */
    // getUartReceiveBuf(&testRecByte[0],busIdx_public);
    // getUartReceiveBuf(&testRecByte[1],busIdx_public);
    // setUartSendBuf(&testSendByte[0], busIdx_public);
    // setUartSendBuf(&testSendByte[1], busIdx_public);
    if(iCounter == 0)
    {
        P44 = 0;
        iCounter ++;
    }
    else
    {
        P44 = 1;
        iCounter --;
    }

    getNetworkData(&testNwData);

    setNetworkData(&testSetNwData);


}