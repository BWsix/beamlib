#pragma once

namespace Blib {

struct Scene {
    static void LoadResources();
    virtual void init() = 0;
    virtual void setup() = 0;
    virtual void render() = 0;
    virtual void renderUI() = 0;
};

}
