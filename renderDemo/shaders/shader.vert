#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
//layout(location = 3) in vec3 inOffset; // instance adatok
//layout(location = 3) in int inTexIndex;  // instance texIndex

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
//layout(location = 2) flat out int texIndex;

 const vec3 pos[4] = vec3[4]( 
  vec3(-1.0, 0.0, -1.0), 
  vec3( 1.0, 0.0, -1.0), 
  vec3( 1.0, 0.0,  1.0), 
  vec3(-1.0, 0.0,  1.0) 
);

// const int indices[6] = int[6](0, 1, 2, 2, 3, 0);

void main() {
   vec3 worldPos = inPosition;// + inOffset;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(worldPos, 1.0);
 //   gl_Position = vec4(inPosition, 0.0, 1.0);

 //   vec3 vpos = pos[indices[gl_VertexIndex]] * 100.0;
 // gl_Position = ubo.proj * ubo.view * vec4(vpos, 1.0);

    fragColor = inPosition;
    fragTexCoord = inTexCoord; //vpos.xz;
   // texIndex = inTexIndex;   // <<< CPU által betöltött érték
}