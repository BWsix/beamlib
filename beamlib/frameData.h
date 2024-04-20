#pragma once

#include "easings.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Blib {

struct FrameData {
    long idx;
    EaseFunc func;
    json& from;
    json& to;
    float progress = 0;
};

};
