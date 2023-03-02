#include <memory>
#include "crsf_serial.h"
#include "crsf_analyzer.h"


bool CRSFAnalyzer::analyze(std::shared_ptr<CrsfSerial> crsf) {
    auto stats = crsf.get()->getLinkStatistics();
    Serial.printf("Statistics:\
\tstats->active_antenna:%d\n\tstats->downlink_Link_quality:%d\n\tstats->downlink_RSSI:%d\n\tstats->downlink_SNR:%d\n\tstats->rf_Mode:%d\n\
\tstats->uplink_Link_quality:%d\n\tstats->uplink_RSSI_1:%d\n\tstats->uplink_RSSI_2:%d\n\tstats->uplink_SNR:%d\n\tstats->uplink_TX_Power\n",
        stats->active_antenna, stats->downlink_Link_quality, stats->downlink_RSSI, stats->downlink_SNR, stats->rf_Mode,
        stats->uplink_Link_quality, stats->uplink_RSSI_1, stats->uplink_RSSI_2, stats->uplink_SNR, stats->uplink_TX_Power);

    auto isLinkUp = crsf.get()->isLinkUp();
    Serial.printf("\tisLinkUp:%d\n", isLinkUp);

    Serial.printf("\tchannels:");

    for (int i = 0; i < 16; i++) {
        Serial.printf(" %d", crsf.get()->getChannel(i));
    }
    return false;
}
