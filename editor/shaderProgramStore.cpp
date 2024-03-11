#include <beamlib.h>

namespace store {

beamlib::ShaderProgram cubeProgram;
beamlib::ShaderProgram starProgram;
beamlib::ShaderProgram gridProgram;

void Init() {
    cubeProgram.Load("editor/shaders/cube.vert.glsl", "editor/shaders/cube.frag.glsl");
    starProgram.Load("editor/shaders/star.vert.glsl", "editor/shaders/star.frag.glsl");
    gridProgram.Load("editor/shaders/grid.vert.glsl", "editor/shaders/grid.frag.glsl");
}

}
