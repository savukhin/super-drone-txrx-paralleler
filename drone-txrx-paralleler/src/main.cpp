#include <Arduino.h>
#include <vector>
#include <memory>
#include <crsf_serial.h>

#include <target.h>

static HardwareSerial CrsfSerialStream1(USART_INPUT_1);
static HardwareSerial CrsfSerialStream2(USART_INPUT_2);
static HardwareSerial OutputSerialStream(USART_OUTPUT);
static std::vector<std::shared_ptr<CrsfSerial>> crsfs = { 
    std::make_shared<CrsfSerial>(CrsfSerialStream1), 
    std::make_shared<CrsfSerial>(CrsfSerialStream2) };



static void setupCrsf(std::shared_ptr<CrsfSerial> csrf_serial)
{
//     csrf_serial->onLinkUp = &crsfLinkUp;
//     csrf_serial->onLinkDown = &crsfLinkDown;
//     csrf_serial->onShiftyByte = &crsfShiftyByte;
//     csrf_serial->onPacketChannels = &packetChannels;
//     csrf_serial->onPacketLinkStatistics = &packetLinkStatistics;
}

void setup() {
    for (auto csrf_serial : crsfs) {
        setupCrsf(csrf_serial);
    }
    OutputSerialStream.begin(CRSF_BAUDRATE);
}

void loop_read_from_output() {
    while (OutputSerialStream.available() == 0) {}
    String content = OutputSerialStream.readString();

    CrsfSerialStream1.print(content);
    CrsfSerialStream1.flush();
    CrsfSerialStream2.print(content);
    CrsfSerialStream2.flush();
}

void loop_analyze_input() {
    std::vector<String*> read_strings(crsfs.size());
    for (int i = 0; i < crsfs.size(); i++) {
        read_strings[i] = crsfs[i]->loop();
    }

    if (read_strings[0] != nullptr) {
        OutputSerialStream.print(*read_strings[0]);
        OutputSerialStream.flush();
    }
}

void loop() {
    loop_read_from_output();
    loop_analyze_input();
}
