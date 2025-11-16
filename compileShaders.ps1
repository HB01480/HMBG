
$outDirectory = "out/res/shaders"
$srcDirectory = "src/shaders"

del $outDirectory/*.spv

glslc -fshader-stage=vertex $srcDirectory/primitive.vert.glsl -o $outDirectory/primitive.vert.spv
glslc -fshader-stage=fragment $srcDirectory/primitive.frag.glsl -o $outDirectory/primitive.frag.spv
