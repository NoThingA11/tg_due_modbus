#include <Arduino.h>
#include <ModbusMaster.h>

ModbusMaster node;

void setup()
{
  Serial.begin(115200);

  Serial1.begin(115200); // Initialize Serial2 at 9600 baud rate, 8 data bits, no parity, 1 stop bit

  // communicate with Modbus slave ID 1 over Serial (port 0)
  node.begin(1, Serial1);
}

void loop()
{
    static uint32_t i;
    uint8_t j, result;
    uint16_t data[6];

    // i++;

    // slave: read (6) 16-bit registers starting at register 2 to RX buffer
    result = node.readHoldingRegisters(0x0010, 2);
    

    // do something with data if read is successful
    if (result == node.ku8MBSuccess)
    {
        for (j = 0; j < 2; j++)
        {
            data[j] = node.getResponseBuffer(j);
        }
        // Serial.print("Loop count: ");
        // Serial.println(i);

        // Print the data read from Modbus slave
        Serial.print("Data Hex: ");
        for (j = 0; j < 2; j++)  // Changed from 6 to 2 to match the number of registers read
        {
            // Print each register as 16-bit binary
            for (int bit = 15; bit >= 0; bit--) {
                Serial.print((data[j] >> bit) & 1);
            }
            if (j < 1)
                Serial.print(" ");
        }

        Serial.print("  || Data: ");
        for (j = 0; j < 2; j++)  // Changed from 6 to 2 to match the number of registers read
        {
            // Print each register as 16-bit binary
            for (int bit = 15; bit >= 0; bit--) {
                Serial.print((data[j] >> bit) & 1);
            }
            if (j < 1)
                Serial.print(" ");
        }
        Serial.println();
    }else
    {
        // Print error message if read failed
        Serial.print("Error reading Modbus slave: ");
        Serial.print(result, HEX);
        Serial.print(" (");
        
        // Explain each exception number
        switch(result) {
            case 0x01:
                Serial.print("Illegal Function - Function code not supported");
                break;
            case 0x02:
                Serial.print("Illegal Data Address - Register address not supported");
                break;
            case 0x03:
                Serial.print("Illegal Data Value - Invalid data value");
                break;
            case 0x04:
                Serial.print("Slave Device Failure - Internal error in slave");
                break;
            case 0x05:
                Serial.print("Acknowledge - Request accepted, processing");
                break;
            case 0x06:
                Serial.print("Slave Device Busy - Slave is processing request");
                break;
            case 0x08:
                Serial.print("Memory Parity Error - Memory error in slave");
                break;
            case 0x0A:
                Serial.print("Gateway Path Unavailable - Gateway not available");
                break;
            case 0x0B:
                Serial.print("Gateway Target Device Failed - No response from target");
                break;
            case 0xE1:
                Serial.print("Invalid Function - Function code error");
                break;
            case 0xE2:
                Serial.print("Invalid Response - Response error");
                break;
            case 0xE3:
                Serial.print("Invalid CRC - CRC check failed");
                break;
            case 0xE4:
                Serial.print("Invalid Data - Data format error");
                break;
            case 0xE5:
                Serial.print("Invalid Length - Response length error");
                break;
            case 0xE6:
                Serial.print("Invalid Address - Address error");
                break;
            case 0xE7:
                Serial.print("Invalid Response - Response format error");
                break;
            case 0xE8:
                Serial.print("Invalid Response - Response timeout");
                break;
            case 0xE9:
                Serial.print("Invalid Response - Response incomplete");
                break;
            case 0xEA:
                Serial.print("Invalid Response - Response too long");
                break;
            case 0xEB:
                Serial.print("Invalid Response - Response too short");
                break;
            case 0xEC:
                Serial.print("Invalid Response - Response invalid");
                break;
            case 0xED:
                Serial.print("Invalid Response - Response error");
                break;
            case 0xEE:
                Serial.print("Invalid Response - Response timeout");
                break;
            case 0xEF:
                Serial.print("Invalid Response - Response error");
                break;
            default:
                Serial.print("Unknown Error Code");
                break;
        }
        Serial.println(")");
    }
            delay(50); // wait for 1 second before next loop iteration

}
