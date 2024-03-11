#include <beamlib.h>

namespace store {

beamlib::ShaderProgram cubeProgram, starProgram;

void Init() {
    cubeProgram.Load("editor/shaders/cube.vert.glsl", "editor/shaders/cube.frag.glsl");
    starProgram.Load("editor/shaders/star.vert.glsl", "editor/shaders/star.frag.glsl");
}

}
