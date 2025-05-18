// This file is a Cpp header file that contains the raw code for the shaders as strings.
// the extension is just for syntax highlighting. Don't try to shader compile that shit
#include <string>


const std::string vertex_deferred= R"( // vertex deferred start
#version 460 core

layout(location = 0) in vec3 RAW_POSITION; // screen space
layout(location = 1) in vec2 RAW_UV;
layout(location = 2) in vec3 RAW_NORMAL;
layout(location = 3) in vec3 RAW_TANGENT;
layout(location = 4) in vec3 RAW_BITANGENT;

uniform mat4 MVP;
uniform mat4 MODEL;

out float DEPTH;
out vec2 UV;
out vec3 POSITION;
out vec3 NORMAL;
out vec3 TANGENT;
out vec3 BITANGENT;


void main () {
    gl_Position = MVP * vec4(RAW_POSITION, 1);
    DEPTH = gl_Position.w;

    UV = RAW_UV;

    POSITION =( MODEL * vec4(RAW_POSITION, 1)).xyz;

    NORMAL = (MODEL * vec4(RAW_NORMAL, 0)).xyz;
    TANGENT = (MODEL * vec4(RAW_TANGENT, 0)).xyz;
    BITANGENT = (MODEL * vec4(RAW_BITANGENT, 0)).xyz;
}
)"; // vertex deferred end




const std::string fragment_deferred= R"( // fragment deferred start
#version 460 core
layout (location = 0) out vec3 _Position;
layout (location = 1) out vec3 _Normal;
layout (location = 2) out vec3 _Tangent;
layout (location = 3) out vec3 _Bitangent;
layout (location = 4) out vec4 _AlbedoTex;
layout (location = 5) out vec3 _NormalTex;
layout (location = 6) out vec3 _RoughnessTex;
layout (location = 7) out vec3 _MetallicTex;

uniform sampler2D _tex_diffuse;
uniform sampler2D _tex_normal;
uniform sampler2D _tex_roughness;
uniform sampler2D _tex_metallic;

in float DEPTH;
in vec2 UV;
in vec3 NORMAL;
in vec3 POSITION;
in vec3 TANGENT;
in vec3 BITANGENT;

void main(){    
   // store the fragment position vector in the first gbuffer texture
   _Position = POSITION;
   // also store the per-fragment normals into the gbuffer
   _Normal = normalize(NORMAL);
   _Tangent = normalize(TANGENT);
   _Bitangent = normalize(BITANGENT);
   // and the diffuse per-fragment color
   _AlbedoTex = texture(_tex_diffuse, UV);
   _NormalTex = texture(_tex_normal, UV).rgb;
   _RoughnessTex = texture(_tex_roughness, UV).rgb;
   _MetallicTex = texture(_tex_metallic, UV).rgb;

}  
)"; // fragment deferred end



const std::string PBR_vertex_deferred= R"( // vertex PBR deferred start
#version 460 core

layout(location = 0) in vec2 RAW_POSITION;

out vec2 uv;

void main () {
   gl_Position = vec4(RAW_POSITION, 0, 1);

   uv = (RAW_POSITION + vec2(1.0)) / 2.0;
}
)"; // vertex deferred end

const std::string fragment_PBR_deferred= R"( // PBR deferred start
#version 460 core

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gTangent;
uniform sampler2D gBitangent;
uniform sampler2D gDepth;
uniform sampler2D gAlbedoTex;
uniform sampler2D gNormalTex;
uniform sampler2D gRoughnessTex;
uniform sampler2D gMetallicTex;

uniform vec3 view;

const float PI = 3.14159265359;
const int atlas_nb_textures = 8;
const int atlas_size_textures = 8;
const int num_lights = 1;

struct Light{
   uint type;
   vec3 lightDir;
   vec3 lightPos;
   vec3 lightColor;
};

Light lights[num_lights]; 

in vec2 uv;

out vec3 color;
mat3 TBN; // computed in main

Light sun = {0, vec3(1.0,1.0,1.0) , vec3(1.0,1.0,1.0), vec3(23.47, 21.31, 20.79)};


// PBR implementation by andrew

float DistributionGGX(vec3 N, vec3 H, float a)
{
   float a2     = a*a;
   float NdotH  = max(dot(N, H), 0.0);
   float NdotH2 = NdotH*NdotH;
  
   float nom    = a2;
   float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
   denom        = PI * denom * denom;
  
   return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
   float nom   = NdotV;
   float denom = NdotV * (1.0 - k) + k;
  
   return nom / denom;
}
 float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
   float NdotV = max(dot(N, V), 0.0);
   float NdotL = max(dot(N, L), 0.0);
   float ggx1 = GeometrySchlickGGX(NdotV, k);
   float ggx2 = GeometrySchlickGGX(NdotL, k);
  
   return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
   return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


vec3 pbr_lighting(vec3 albedo, vec3 normal, float roughness, float metallic, Light light_struct){

   vec3 N = normalize(normal); 
   vec3 V = normalize(view);

   vec3 light = light_struct.lightDir;
   vec3 lightColor = light_struct.lightColor;

   vec3 Lo = vec3(0.0);

   //make a for loop if we have more than 1 light probably
   vec3 L = normalize(light);
   vec3 H = normalize(V + L); //half vector
   
   float distance = length(light);
   float attenuation = 1.0 / (distance * distance);
   vec3 radiance = lightColor * attenuation; 

   vec3 F0 = vec3(0.04); 
   F0 = mix(F0, albedo, metallic);
   vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
   float NDF = DistributionGGX(N, H, roughness);       
   float G = GeometrySmith(N, V, L, roughness);   

   vec3 numerator = NDF * G * F;
   float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
   vec3 specular = numerator / denominator; 

   vec3 kS = F;
   vec3 kD = vec3(1.0) - kS;
   
   kD *= 1.0 - metallic;	

   float NdotL = max(dot(N, L), 0.0);                
   Lo += (kD * albedo / PI + specular) * radiance * NdotL; //+= cuz its supposed to be multiple lights

   //end of for loop

   vec3 ambient = vec3(0.03) * albedo; // * ao if we create an ao map but tbh idgaf
   vec3 tempColor = ambient + Lo;

   tempColor = tempColor / (tempColor + vec3(1.0));
   return pow(tempColor, vec3(1.0/2.2));  

}

void main(){    
   vec3 position = texture(gPosition, uv).xyz;
   vec3 normal = texture(gNormal, uv).xyz;
   vec3 tangent = texture(gTangent, uv).xyz;
   vec3 bitangent = texture(gBitangent, uv).xyz;

   vec4 albedo = texture(gAlbedoTex, uv);
   vec3 textureNormal = texture(gNormalTex, uv).xyz;
   float roughness = texture(gRoughnessTex, uv).x;
   float metallic = texture(gMetallicTex, uv).x;

   float depth = texture(gDepth, uv).x;
    
   mat3 TBN = mat3(tangent, bitangent, normal);


   float zNear = 0.1;
   float  zFar = 300.0;

   depth =  zNear * zFar / (zFar + depth * (zNear - zFar)) / 10.0;

   textureNormal = normal * 2.0 - 1.0;   
   textureNormal = normalize(TBN * textureNormal); //convert to tangent spage


   lights[0] = sun;

   //Our roughness map is brifhtness map 
   roughness = 1 - roughness; 

   //calculating our albedo map

   for(int i = 0 ; i < num_lights ; i++){
      color+= pbr_lighting(albedo.xyz, textureNormal, roughness, metallic, lights[i]);
   }


} 
)"; // PBR deferred end



// https://www.reddit.com/r/opengl/comments/y83zy1/performance_issues_with_deferred_rendering/