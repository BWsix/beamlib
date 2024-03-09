#pragma once

#include <beamlib.h>

class Root : public beamlib::Object {
public:
    Root(beamlib::ShaderProgram program) : Object(program) {}
    void Update() override {}
    void Render() override {}
};

class RootInstance : public beamlib::Instance {
public:
    RootInstance(Root *object, std::string name) : Instance(object, name) {}
};
