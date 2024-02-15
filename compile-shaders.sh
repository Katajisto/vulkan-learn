echo "Doing shader compile..."
glslc shader.frag -o shaders/frag.spv
glslc shader.vert -o shaders/vert.spv
echo "Great success!"