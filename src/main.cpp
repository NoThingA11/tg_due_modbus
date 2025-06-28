#include <Arduino.h>
#include <ModbusMaster.h>

ModbusMaster node;

String inputStr = ""; // A String to hold incoming data
static uint32_t i;
uint8_t j, result;
uint16_t data[6];
uint16_t uid_data[4]; // Array for UID data

// Default register settings
uint16_t regAddress = 0x0010;
uint8_t regCount = 2;

// Function to parse register command
bool parseRegisterCommand(String command, uint16_t &address, uint8_t &count)
{
    command.trim(); // Remove whitespace

    // Check if command contains comma
    int commaIndex = command.indexOf(',');
    if (commaIndex == -1)
    {
        Serial.println("Error: Invalid format. Use '0x0010,4' format");
        return false;
    }

    // Extract address part (before comma)
    String addrStr = command.substring(0, commaIndex);
    addrStr.trim();

    // Extract count part (after comma)
    String countStr = command.substring(commaIndex + 1);
    countStr.trim();

    // Convert address string to uint16_t
    if (addrStr.startsWith("0x") || addrStr.startsWith("0X"))
    {
        // Hexadecimal format
        address = strtol(addrStr.c_str(), NULL, 16);
    }
    else
    {
        // Decimal format
        address = addrStr.toInt();
    }

    // Convert count string to uint8_t
    count = countStr.toInt();

    // Validate count (1-125 is typical Modbus limit)
    if (count < 1 || count > 125)
    {
        Serial.println("Error: Register count must be between 1 and 125");
        return false;
    }

    return true;
}

void readRegister()
{
    // slave: read registers based on parsed command
    result = node.readHoldingRegisters(regAddress, regCount);

    // do something with data if read is successful
    if (result == node.ku8MBSuccess)
    {
        for (j = 0; j < regCount && j < 6; j++) // Limit to array size
        {
            data[j] = node.getResponseBuffer(j);
        }
        // Serial.print("Loop count: ");
        // Serial.println(i);

        // Print the data read from Modbus slave
        Serial.print("Data Binary: ");
        for (j = 0; j < regCount && j < 6; j++) // Changed from 6 to 2 to match the number of registers read
        {
            // Print each register as 16-bit binary
            for (int bit = 15; bit >= 0; bit--)
            {
                Serial.print((data[j] >> bit) & 1);
            }
            if (j < regCount - 1 && j < 5)
                Serial.print(" ");
        }

        Serial.print("  || Data Hex: ");
        for (j = 0; j < regCount && j < 6; j++) // Changed from 6 to 2 to match the number of registers read
        {
            Serial.print(data[j], HEX);
            if (j < regCount - 1 && j < 5)
                Serial.print(" ");
        }
        Serial.println();
    }
    else
    {
        // Print error message if read failed
        Serial.print("Error reading Modbus slave (Data): ");
        Serial.print(result, HEX);
        Serial.print(" (");

        // Explain each exception number
        switch (result)
        {
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
}

void readUID()
{
    // Read UID from registers 0x0054-0x0057
    result = node.readHoldingRegisters(0x0054, 4);

    if (result == node.ku8MBSuccess)
    {
        for (j = 0; j < 4; j++)
        {
            uid_data[j] = node.getResponseBuffer(j);
        }

        // Print the UID data read from Modbus slave
        Serial.print("UID Binary: ");
        for (j = 0; j < 4; j++)
        {
            // Print each register as 16-bit binary
            for (int bit = 15; bit >= 0; bit--)
            {
                Serial.print((uid_data[j] >> bit) & 1);
            }
            if (j < 3)
                Serial.print(" ");
        }

        Serial.print("  || UID Hex: ");
        for (j = 0; j < 4; j++)
        {
            Serial.print(uid_data[j], HEX);
            if (j < 3)
                Serial.print(" ");
        }
        Serial.println();
    }
    else
    {
        // Print error message if UID read failed
        Serial.print("Error reading UID: ");
        Serial.print(result, HEX);
        Serial.print(" (");

        // Explain each exception number
        switch (result)
        {
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

}

void setup()
{
    Serial.begin(115200);

    Serial1.begin(115200); // Initialize Serial2 at 9600 baud rate, 8 data bits, no parity, 1 stop bit

    // communicate with Modbus slave ID 1 over Serial (port 0)
    node.begin(1, Serial1);
}

void loop()
{
    if (Serial.available())
    {
        String teststr = Serial.readString(); // read until timeout
        teststr.trim();                       // remove any \r \n whitespace at the end of the String
        Serial.print("Received: ");
        Serial.println(teststr);

        // Parse the command if it's in register format
        if (parseRegisterCommand(teststr, regAddress, regCount))
        {
            Serial.print("Parsed - Address: 0x");
            Serial.print(regAddress, HEX);
            Serial.print(", Count: ");
            Serial.println(regCount);
        }
    } // wait for data available

    // readRegister(); // Read registers based on parsed command
    // readUID();      // Read UID from registers 0x0054-0x0057
    
    delay(50); // wait for 50ms before next loop iteration
}
