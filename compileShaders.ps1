
$outDirectory = "out/res/shaders"
$srcDirectory = "src/shaders"

del $outDirectory/*.spv

glslc -fshader-stage=vertex $srcDirectory/primitive.vert.glsl -o $outDirectory/primitive.vert.spv
glslc -fshader-stage=fragment $srcDirectory/primitive.frag.glsl -o $outDirectory/primitive.frag.spv

glslc -fshader-stage=vertex $srcDirectory/basic.vert.glsl -o $outDirectory/basic.vert.spv
glslc -fshader-stage=fragment $srcDirectory/basic.frag.glsl -o $outDirectory/basic.frag.spv

glslc -fshader-stage=vertex $srcDirectory/textured.vert.glsl -o $outDirectory/textured.vert.spv
glslc -fshader-stage=fragment $srcDirectory/textured.frag.glsl -o $outDirectory/textured.frag.spv
