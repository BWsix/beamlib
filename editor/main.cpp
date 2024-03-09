#include <beamlib.h>

#include "cube.hpp"
#include "star.hpp"
#include "root.hpp"

// TODO: try slerp
// TODO: skeleton with `Transform`

const char *data = R"json(
{"children":[{"children":[{"children":[],"data":{"color":[0.3529411554336548,1.0,0.0]},"name":"cube2","transform":{"localRotationQuat":[-0.17556609213352203,0.27463194727897644,0.5522274374961853,0.7673317790031433],"position":[2.049999952316284,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":{"color":[0.0,0.8235292434692383,1.0]},"name":"star1","transform":{"localRotationQuat":[0.2997468411922455,0.48850908875465393,0.6379016041755676,0.5143837928771973],"position":[0.0,1.0,0.0],"rotationQuat":[-0.05826212465763092,0.04515324905514717,-0.021332738921046257,-0.9970489144325256],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,0.0,0.0]},"name":"cube1","transform":{"localRotationQuat":[0.6668828129768372,0.2672681510448456,0.6626741886138916,0.21141669154167175],"position":[1.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"root","transform":{"localRotationQuat":[0.31930866837501526,0.0,0.0,0.9476504325866699],"position":[-0.8500000238418579,1.0,-2.0999999046325684],"rotationQuat":[-0.12467435002326965,-4.7154194362519775e-06,0.00011116143286926672,0.9921945929527283],"scale":[0.25,1.0,1.0]}}
)json";

int main() {
    const auto window = beamlib::CreateWindow("beamlib");

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    beamlib::camera.transform.Translate({0, 0, 10});
    beamlib::ShaderProgram cubeProgram("editor/shaders/cube.vert.glsl", "editor/shaders/cube.frag.glsl");
    beamlib::ShaderProgram starProgram("editor/shaders/star.vert.glsl", "editor/shaders/star.frag.glsl");
    beamlib::ShaderProgram exampleProgram("editor/shaders/example.vert.glsl", "editor/shaders/example.frag.glsl");
    Cube cube(cubeProgram);
    Star star(starProgram);
    Root _root(exampleProgram);
    RootInstance root(&_root, "root");

    CubeInstance cube1(&cube, "cube1");
    cube1.transform.Translate({1, 0, 0});
    root.PushChild(&cube1);

    CubeInstance cube2(&cube, "cube2");
    cube2.transform.Translate({1, 0, 0});
    cube1.PushChild(&cube2);

    StarInstance star1(&star, "star1");
    star1.transform.Translate({0, 1, 0});
    cube1.PushChild(&star1);

    root.Load(json::parse(data));

    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        root.Update();
        root.Render();

        beamlib::BeginUI();
        root.RenderUI();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
