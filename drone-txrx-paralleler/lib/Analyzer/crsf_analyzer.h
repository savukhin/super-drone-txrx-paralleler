#pragma once
#include "crsf_serial.h"
#include <memory>
#include <utility>
#include <vector>


class CRSFAnalyzer {
public:

private:
using CrsfIndexes = std::vector<std::pair<CrsfSerial, int>>;
    // Mapper
    CrsfIndexes makeCrsfIndexes(std::vector<CrsfSerial> &crsfs) ;

    // Returns filtered indexes
    CrsfIndexes filterInactiveConns(CrsfIndexes &crsfs);

public:
    CRSFAnalyzer() = default;

    // ------------------------------------------------------ //
    // ------------- NEED TO CHECK uplink_RSSI_1 ------------ //
    // ------------------ (Lower is better) ----------------- //
    // ------------------------------------------------------ //
    // Return index of best crsf (-1 if not exists)
    int analyze(std::vector<CrsfSerial> &crsfs, size_t chosen);
};
