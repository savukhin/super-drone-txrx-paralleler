#include <Arduino.h>
#include <vector>
#include <memory>
#include <string>
#include <crsf_serial.h>

#include <target.h>
#include <crsf_analyzer.h>

static std::vector<void*> UsartInputs = USART_INPUTS;
static HardwareSerial OutputSerialStream(USART1);
static std::vector<HardwareSerial> CrsfSerialInputStreams = { HardwareSerial(USART2), HardwareSerial(USART3) };
CrsfSerial crsf_1(CrsfSerialInputStreams[0]);
CrsfSerial crsf_2(CrsfSerialInputStreams[1]);

std::vector<CrsfSerial> crsfs = { crsf_1 };

static u_int8_t chosenSerial = 0;

static CRSFAnalyzer Analyzer;


static void crsfLinkUp()
{
    Serial.println("Csrf linked up!");
}

static void setupCrsf(CrsfSerial &csrf_serial)
{
    csrf_serial.onLinkUp = &crsfLinkUp;
    // csrf_serial->onLinkDown = &crsfLinkDown;
    // csrf_serial->onShiftyByte = &crsfShiftyByte;
    // csrf_serial->onPacketChannels = &packetChannels;
    // csrf_serial->onPacketLinkStatistics = &packetLinkStatistics;
}

void setup() {
    for (auto csrf_serial : crsfs) {
        setupCrsf(csrf_serial);
    }
    OutputSerialStream.begin(CRSF_BAUDRATE);

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
    std::vector<String*> read_strings(crsfs.size());
    for (size_t i = 0; i < crsfs.size(); i++) {
        read_strings[i] = crsfs[i].loop();
    }

    bool should_swap = Analyzer.analyze(crsfs, chosenSerial);
    if (should_swap)
        chosenSerial = (chosenSerial + 1) % crsfs.size();

    String* chosen_string = read_strings[chosenSerial];

    if (chosen_string != nullptr) {
        OutputSerialStream.print(*chosen_string);
        OutputSerialStream.flush();
    }

    for (size_t i = 0; i < read_strings.size(); i++) {
        free(read_strings[i]);
    }
}

uint32_t last_heartbeat_time = 0;

void loop() {
    // loop_read_from_output();
    loop_analyze_input();


    if (millis() > last_heartbeat_time + 1000) {
        Serial.print("Heartbeat:\t");
        if (crsfs[0].isLinkUp()) {
            Serial.println("[0] is available");
//             auto stats = crsfs[0].getLinkStatistics();
//             // Serial.printf("Statistics: stats->downlink_Link_quality:%d, stats->uplink_Link_quality:%d\n", stats->downlink_Link_quality, stats->uplink_Link_quality);

//             Serial.printf("Statistics:
// \tstats->active_antenna:%d\n\tstats->downlink_Link_quality:%d\n\tstats->downlink_RSSI:%d\n\tstats->downlink_SNR:%d\n\tstats->rf_Mode:%d\n
// \tstats->uplink_Link_quality:%d\n\tstats->uplink_RSSI_1:%d\n\tstats->uplink_RSSI_2:%d\n\tstats->uplink_SNR:%d\n\tstats->uplink_TX_Power\n",
//                 stats->active_antenna, stats->downlink_Link_quality, stats->downlink_RSSI, stats->downlink_SNR, stats->rf_Mode,
//                 stats->uplink_Link_quality, stats->uplink_RSSI_1, stats->uplink_RSSI_2, stats->uplink_SNR, stats->uplink_TX_Power);

//             auto isLinkUp = crsfs[0].isLinkUp();
//             Serial.printf("\tisLinkUp:%d\n", isLinkUp);

//             Serial.printf("\tchannels:");

//             for (int i = 0; i < 16; i++) {
//                 Serial.printf(" %d", crsfs[0].getChannel(i));
//             }

        } else {
            Serial.println("[0] is not available");
        }
        
        Serial.flush();


        last_heartbeat_time = millis();
    }
}
