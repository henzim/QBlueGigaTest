/** Quick and dirty C++ interface for handling UART communication with
 * Bluegiga adapters such as BLED112. Based on the uart.h/uart.c from
 * the the Bluegiga SDK
 * TODO: Maybe singleton, protect from multiple opens, etc. */

#ifndef BGUARTHandler_H
#define BGUARTHandler_H

#include <vector>

class BluegigaUARTHandler
{
public:
    enum RXStatus { RX_READ_ALL_DATA, RX_TIMEOUT, RX_ERROR};

    BluegigaUARTHandler();
    ~BluegigaUARTHandler();

    /** List COM port(s) with connected BLE(D) device
      * returns a vector with ints i, corresponding to
      * the resp. COMi */
    std::vector< int > list_devices() const;

    /** I am feeling lucky - lists devices and tries to open a connection to the first found*/
    bool feeling_lucky_open();

    /** Open handle on port
      * closes any current connection and tries 5 times to open a new */
    bool close_and_open( int _com);

    /** Close current handle */
    void close();

    /** Check handle */
    bool handle_ok() const;

    /** Transmit byte data */
    bool tx(int len, unsigned char *data);

    /** Receive byte data */
    RXStatus rx(int len,unsigned char *data,int timeout_ms);

private:
    /** Open handle on port without first closing*/
    bool uart_open(int _com);

    /** Close current handle */
    void uart_close();

};

#endif // BGUARTHandler_H
