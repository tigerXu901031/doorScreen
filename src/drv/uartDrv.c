/* 
   -----------------------------------------------------------------------
   Author   - Tiger.Xu
   Date     - 2019.06.08
   Version  - 0.0.1

   Change Notes:
        2019.06.08      Tiger.Xu
        Initial version

        ...             ...
        ...
    
   Description:
        - Use one level fifo buffer when receive and transmit data in byte
          level
        - Baud rate: 9600bps
        - one start bit, one stop bit, 8 data bit
        
   -----------------------------------------------------------------------
*/

#include "uartDrv.h"

uartSts_type uartGlobalSts;
fifo_type uartTxFifo_Obj[busIdx_max];
fifo_type uartRxFifo_Obj[busIdx_max];

/* This function should be called cyclic in TODO: 1 ms
   (Not final decided yet) and will be called in a timer
   interrupt
 */
static void sendDataCyclic()
{
    unsigned int data;
    busIdx_type i = 0;
    for(i = 0; i < busIdx_max; i ++)
    {
        if(uartTxFifo_Obj[i].curPtr > 0)
        {
            /* If the Tx FIFO buffer has data to send then put
               the send data into data register
             */
            if(uartGlobalSts != uartSts_busy)
            {
                uartGlobalSts = uartSts_busy;
                getFifoData(&uartTxFifo_Obj[i], &data);
                SBUF = data;
            }
        }
    }
}

void uartDrvInit()
{
    unsigned char i;
    /* TODO: Configure the uart register */


    /* Create an FIFO object buffer uart data */
    for(i = 0; i < busIdx_max; i ++)
    {
        uartTxFifo_Obj[i] = fifoInit(UART_DATA_LEN_IN_BYTE);
    }
}

void uartDrvUpdate()
{
    sendDataCyclic();
}

uartSts_type setUartSendBuf(unsigned int *data, busIdx_type nwChn)
{
    fifoSts_type writeOpsts;
    uartSts_type returnVal;

    /* write the data into the send buffer */
    writeOpsts = setFifoData(&uartTxFifo_Obj[nwChn], data);
    if(writeOpsts == writeSuccess)
    {
        returnVal = uartSts_normal;
    }
    else{
        returnVal = uartSts_sendFail; 
    }
    return returnVal;
}

uartSts_type getUartReceiveBuf(unsigned int *data, busIdx_type nwChn)
{
    fifoSts_type readOpsts;
    uartSts_type returnVal;

    /* Get the data from Rx fifo */
    readOpsts = getFifoData(&uartRxFifo_Obj[nwChn], data);

    if(readOpsts == readSuccess)
    {
        returnVal = uartSts_normal;
    }
    else{
        returnVal = uartSts_receiveFail;
    }
    return returnVal;
}

void uartInterruptService()
{
    /* TODO this interrupt shoud be copy for two channel */
    unsigned char data;
    if (RI)
    {
        RI = 0;

        data = SBUF;
        /* Put the data into the Rx FIFO */                 
		setFifoData(&uartRxFifo_Obj[0], &data);
    }
    if (TI)
    {
        TI = 0;
        uartGlobalSts = uartSts_normal;                 
    }
}

unsigned char isUartNewDataAvailable(busIdx_type nwChn)
{
    if(uartRxFifo_Obj[nwChn].curPtr > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}