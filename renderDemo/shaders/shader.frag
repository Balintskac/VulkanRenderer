#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

//layout(binding = 1) uniform sampler2D texSampler;

layout(set = 0, binding = 1) uniform sampler2D textures[54];

/*
    layout(push_constant) uniform PushConstants {
        int texIndex;
    } pc;
*/

layout(location = 2) flat in int texIndex;


void main() {
   // outColor = vec4(fragColor, 1.0);
   //  outColor = texture(texSampler, fragTexCoord);
  // outColor = texture(texSampler, fragTexCoord);

  vec4 finalColor =  texture(textures[texIndex], fragTexCoord);
  //finalColor.y = pc.texIndex;
  outColor = finalColor;
    // outColor = texture(textures[pc.texIndex], fragTexCoord);
}