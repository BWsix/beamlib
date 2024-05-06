#include "base.frag.glsl"

in flat uint FaceID;
in flat uint TexID;

layout(location = 0) out uvec4 elementID;

void main() {
    elementID = uvec4(FaceID, 0, 0, 0);
}
