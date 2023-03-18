#include <Arduino.h>
#include <vector>
#include <memory>
#include <string>
#include <crsf_serial.h>

#include <target.h>
#include "crsf_analyzer.h"

static std::vector<void*> UsartInputs = USART_INPUTS;
static HardwareSerial OutputSerialStream(USART_OUTPUT);
static std::vector<HardwareSerial> CrsfSerialInputStreams = {};
static std::vector<std::shared_ptr<CrsfSerial>> crsfs(0);

static u_int8_t chosenSerial = 0;

static CRSFAnalyzer Analyzer;

static void setupCrsf(std::shared_ptr<CrsfSerial> csrf_serial)
{
//     csrf_serial->onLinkUp = &crsfLinkUp;
//     csrf_serial->onLinkDown = &crsfLinkDown;
//     csrf_serial->onShiftyByte = &crsfShiftyByte;
//     csrf_serial->onPacketChannels = &packetChannels;
//     csrf_serial->onPacketLinkStatistics = &packetLinkStatistics;
}

void setup() {
    for (auto usart : UsartInputs) {
        CrsfSerialInputStreams.emplace_back(usart);
    }

    for (auto stream : CrsfSerialInputStreams) {
        crsfs.emplace_back(std::make_shared<CrsfSerial>(stream));
    }


    for (auto csrf_serial : crsfs) {
        setupCrsf(csrf_serial);
    }
    OutputSerialStream.begin(CRSF_BAUDRATE);
}

void loop_read_from_output() {
    while (OutputSerialStream.available() == 0) {}
    String content = OutputSerialStream.readString();

    for (auto stream : CrsfSerialInputStreams) {
        stream.print(content);
        stream.flush();
    }
}

void loop_analyze_input() {
    std::vector<String*> read_strings(crsfs.size());
    for (int i = 0; i < crsfs.size(); i++) {
        read_strings[i] = crsfs[i]->loop();
    }

    bool should_swap = Analyzer.analyze(crsfs[chosenSerial]);
    if (should_swap)
        chosenSerial = (chosenSerial + 1) % crsfs.size();

    String* chosen_string = read_strings[chosenSerial];

    if (chosen_string != nullptr) {
        // OutputSerialStream.print(*chosen_string);
        // OutputSerialStream.flush();
    }
}

void loop() {
    loop_read_from_output();
    loop_analyze_input();
}
