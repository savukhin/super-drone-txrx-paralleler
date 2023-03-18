#include <Arduino.h>
#include <vector>
#include <memory>
#include <string>
#include <crsf_serial.h>

#include <target.h>
#include "crsf_analyzer.h"

static std::vector<void*> UsartInputs = USART_INPUTS;
static HardwareSerial OutputSerialStream(USART_OUTPUT);
static std::vector<HardwareSerial> CrsfSerialInputStreams = { HardwareSerial(USART2) };
CrsfSerial crsf_1(CrsfSerialInputStreams[0]);

std::vector<CrsfSerial> crsfs = { crsf_1 };

static u_int8_t chosenSerial = 0;

static CRSFAnalyzer Analyzer;


static void crsfLinkUp()
{
    Serial.println("Csrf linked up!");
}

static void setupCrsf(std::shared_ptr<CrsfSerial> csrf_serial)
{
    csrf_serial->onLinkUp = &crsfLinkUp;
    // csrf_serial->onLinkDown = &crsfLinkDown;
    // csrf_serial->onShiftyByte = &crsfShiftyByte;
    // csrf_serial->onPacketChannels = &packetChannels;
    // csrf_serial->onPacketLinkStatistics = &packetLinkStatistics;
}

void setup() {
    // for (auto usart : UsartInputs) {
    //     CrsfSerialInputStreams.emplace_back(usart);
    // }

    // CrsfSerialInputStreams.emplace_back(USART2);
    // HardwareSerial serial(USART2);

    // auto crsf = new CrsfSerial(CrsfSerialInputStreams[0]);
    // crsfs.push_back(crsf);
    // crsfs.emplace_back<CrsfSerial>(CrsfSerialStream);
    // crsfs.push_back(crsf);

    // CrsfSerialStream = new HardwareSerial(USART2);
    // crsf = new CrsfSerial(CrsfSerialStream);
    // crsf = new CrsfSerial(CrsfSerialInputStreams[0]);
    // auto crsf_local = new CrsfSerial(CrsfSerialStream);
    // crsfs.push_back(crsf_local);
    // crsf = crsf_local;
    // crsfs.emplace_back(CrsfSerialStream);
    // crsfs.emplace_back(CrsfSerialInputStreams[0]);
    // crsfs.emplace_back(std::make_shared<CrsfSerial>(CrsfSerialInputStreams[0]));

    // for (auto stream : CrsfSerialInputStreams) {
    //     crsfs.emplace_back(std::make_shared<CrsfSerial>(stream));
    // }


    // for (auto csrf_serial : crsfs) {
    //     setupCrsf(csrf_serial);
    // }
    // OutputSerialStream.begin(CRSF_BAUDRATE);

    Serial.begin(115200);
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
    // std::vector<String*> read_strings(crsfs.size());
    for (size_t i = 0; i < crsfs.size(); i++) {
        crsfs[i].loop();
        // Serial.println("chan 7 = " + String(crsfs[0].getChannel(7)));
        // read_strings[i] = crsfs[i].loop();
    }
    return ;


    // bool should_swap = Analyzer.analyze(crsfs[chosenSerial]);
    // if (should_swap)
    //     chosenSerial = (chosenSerial + 1) % crsfs.size();

    // String* chosen_string = read_strings[chosenSerial];

    // if (chosen_string != nullptr) {
    //     // OutputSerialStream.print(*chosen_string);
    //     // OutputSerialStream.flush();
    // }
}

uint32_t last_heartbeat_time = 0;

void loop() {
    // loop_read_from_output();
    loop_analyze_input();
    // Serial.println("Heartbeat:\t");

    // if (crsf == nullptr)
    //     return;

    // auto crsf = crsfs.front();

    // crsfs[0].loop();

    // ------------------------------------------------------ //
    // ------------- NEED TO CHECK uplink_RSSI_1 ------------ //
    // ------------------ (Lower is better) ----------------- //
    // ------------------------------------------------------ //


    if (millis() > last_heartbeat_time + 1000) {
        Serial.print("Heartbeat:\t");
        if (crsfs[0].isLinkUp()) {
            Serial.println("[0] is available");
            auto stats = crsfs[0].getLinkStatistics();
            // Serial.printf("Statistics: stats->downlink_Link_quality:%d, stats->uplink_Link_quality:%d\n", stats->downlink_Link_quality, stats->uplink_Link_quality);

            Serial.printf("Statistics:\
\tstats->active_antenna:%d\n\tstats->downlink_Link_quality:%d\n\tstats->downlink_RSSI:%d\n\tstats->downlink_SNR:%d\n\tstats->rf_Mode:%d\n\
\tstats->uplink_Link_quality:%d\n\tstats->uplink_RSSI_1:%d\n\tstats->uplink_RSSI_2:%d\n\tstats->uplink_SNR:%d\n\tstats->uplink_TX_Power\n",
                stats->active_antenna, stats->downlink_Link_quality, stats->downlink_RSSI, stats->downlink_SNR, stats->rf_Mode,
                stats->uplink_Link_quality, stats->uplink_RSSI_1, stats->uplink_RSSI_2, stats->uplink_SNR, stats->uplink_TX_Power);

            auto isLinkUp = crsfs[0].isLinkUp();
            Serial.printf("\tisLinkUp:%d\n", isLinkUp);

            Serial.printf("\tchannels:");

            for (int i = 0; i < 16; i++) {
                Serial.printf(" %d", crsfs[0].getChannel(i));
            }

        } else {
            Serial.println("[0] is not available");
        }
        
        Serial.flush();


        last_heartbeat_time = millis();
    }
}
