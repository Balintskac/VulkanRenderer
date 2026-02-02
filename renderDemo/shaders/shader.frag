#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

//layout(binding = 1) uniform sampler2D texSampler;

layout(set = 0, binding = 1) uniform sampler2D diffuseSampler;

/*
    layout(push_constant) uniform PushConstants {
        int texIndex;
    } pc;
*/

//layout(location = 2) flat in int texIndex;

 float log10(float x) {
  return log(x) / log(10.0);
 }
 float satf(float x) {
  return clamp(x, 0.0, 1.0);
 }
 vec2 satv(vec2 x) {
  return clamp(x, vec2(0.0), vec2(1.0));
 }
 float max2(vec2 v) {
  return max(v.x, v.y);
 }

 float gridSize = 100.0;
 float gridCellSize = 0.025;
 vec4 gridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
 vec4 gridColorThick = vec4(0.0, 0.0, 0.0, 1.0);
 const float gridMinPixelsBetweenCells = 2.0;

void main() {


//  vec4 finalColor =  texture(textures[texIndex], fragTexCoord);
      vec4 baseColor = vec4(0.4);
  //  if (texIndex >= 0) {
        baseColor = texture(diffuseSampler, fragTexCoord);
  //  }
     outColor = baseColor;


   // outColor = vec4(fragColor, 1.0);
   //  outColor = texture(texSampler, fragTexCoord);
  // outColor = texture(texSampler, fragTexCoord);
  //finalColor.y = pc.texIndex;
    // outColor = texture(textures[pc.texIndex], fragTexCoord);


   /*
     vec2 dudv = vec2(
      length(vec2(dFdx(fragTexCoord.x), dFdy(fragTexCoord.x))),
      length(vec2(dFdx(fragTexCoord.y), dFdy(fragTexCoord.y)))
     );

      float lodLevel = max(0.0, log10((length(dudv) * 
  gridMinPixelsBetweenCells) / gridCellSize) + 1.0);
 float lodFade = fract(lodLevel);

      float lod0 = 
     gridCellSize * pow(10.0, floor(lodLevel+0));
     float lod1 = 
      gridCellSize * pow(10.0, floor(lodLevel+1));
     float lod2 = 
      gridCellSize * pow(10.0, floor(lodLevel+2));

       dudv *= 4.0;
   float lod0a = max2( vec2(1.0) -
   abs(satv(mod(fragTexCoord, lod0) / dudv) * 2.0  - vec2(1.0)));
 float lod1a = max2(vec2(1.0) -  
  abs(satv(mod(fragTexCoord, lod1) / dudv) * 2.0 - vec2(1.0)));
 float lod2a = max2(vec2(1.0) -  
  abs(satv(mod(fragTexCoord, lod2) / dudv) * 2.0 - vec2(1.0)));

 vec4 c = lod2a > 0.0 ? gridColorThick : lod1a > 0.0 ? 
  mix(gridColorThick, gridColorThin, lodFade) : 
  gridColorThin;

  float opacityFalloff = 
  (1.0 - satf(length(fragTexCoord) / gridSize));

      c.a *= lod2a > 0.0 ? lod2a : lod1a > 0.0 ? 
      lod1a : (lod0a * (1.0-lodFade));
c.a *= opacityFalloff;  // Esetleg ezt finomhangolni
     outColor = c;
     */
}