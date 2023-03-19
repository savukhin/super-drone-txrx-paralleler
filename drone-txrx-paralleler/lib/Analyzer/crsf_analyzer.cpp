#include <memory>
#include <vector>
#include <utility>
#include <crsf_serial.h>

#include "crsf_analyzer.h"

CRSFAnalyzer::CrsfIndexes CRSFAnalyzer::makeCrsfIndexes(std::vector<CrsfSerial> &crsfs) {
    CRSFAnalyzer::CrsfIndexes res;
    for (size_t i = 0; i < crsfs.size(); i++) {
        res.push_back({crsfs[i], i});
    }
    return res;
}

CRSFAnalyzer::CrsfIndexes CRSFAnalyzer::filterInactiveConns(CRSFAnalyzer::CrsfIndexes &crsfs) {
    CRSFAnalyzer::CrsfIndexes filtered;
    for (auto crsf : crsfs) {
        if (crsf.first.isLinkUp())
            filtered.push_back(crsf);
    }

    return filtered;
}

int CRSFAnalyzer::analyze(std::vector<CrsfSerial> &crsfs, size_t chosen) {
    CRSFAnalyzer::CrsfIndexes crsfs_indexes = this->makeCrsfIndexes(crsfs);
    crsfs_indexes = this->filterInactiveConns(crsfs_indexes);

    if (crsfs_indexes.size() == 0)
        return 1;

    int min_uplink_RSSI_1_ind = 0;
    auto min_uplink_RSSI_1 = crsfs[0].getLinkStatistics()->uplink_RSSI_1;

    for (auto crsf : crsfs_indexes) {
        auto serial = crsf.first;
        auto stats = serial.getLinkStatistics();

        if (stats->uplink_RSSI_1 < min_uplink_RSSI_1) {
            min_uplink_RSSI_1 = stats->uplink_RSSI_1;
            min_uplink_RSSI_1_ind = crsf.second;
        }
        
    }

    return min_uplink_RSSI_1_ind;
}
