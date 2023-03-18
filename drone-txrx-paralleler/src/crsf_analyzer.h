#pragma once
#include "crsf_serial.h"
#include <memory>

class CRSFAnalyzer {
public:
    CRSFAnalyzer() = default;

    bool analyze(CrsfSerial &crsf);
};
