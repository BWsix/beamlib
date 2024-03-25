#version 430 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_diffuse2;
  sampler2D texture_diffuse3;
  sampler2D texture_diffuse4;
  sampler2D texture_diffuse5;
  sampler2D texture_specular1;
  sampler2D texture_specular2;
  sampler2D texture_specular3;
  sampler2D texture_specular4;
  sampler2D texture_specular5;
};

uniform Material material;
uniform samplerCube skybox;

void main() {
  vec3 lightDir = vec3(0.0, -1.0, 0.0);
  vec3 normal = normalize(Normal);
  // vec3 reflectDir = reflect(lightDir, normal);
  //
  float ambientIntensity = 0.3;
  // vec3 ambient = ambientIntensity * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 ambient = ambientIntensity * vec3(1.0);
  //
  float diffuseIntensity = max(0.0, dot(normal, -lightDir));
  // vec3 diffuse = diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = diffuseIntensity * vec3(1.0);
  //
  // float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 16.0);
  // vec3 specular = specularIntensity * vec3(texture(material.texture_specular1, TexCoords));
  //
  // vec3 result = ambient + diffuse + specular;
  // FragColor = vec4(result, 1.0);
  // FragColor = texture(material.texture_diffuse1, TexCoords);

  // FragColor = vec4(texture(skybox, reflect(Position, normal)).rgb, 1.0);
  // FragColor = vec4(reflect(Position, normal), 1.0);

  // FragColor = vec4(texture(material.texture_specular1, TexCoords).rgb, 1.0);
  // FragColor = vec4(1.0, 1.0, 1.0, 1.0);

  vec3 result = ambient + diffuse;
  FragColor = vec4(result, 1.0);
}
