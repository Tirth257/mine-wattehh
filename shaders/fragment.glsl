#version 330 core

uniform vec3 lightColor;
uniform vec3 light_pos;
uniform vec3 cam_pos;
uniform sampler2D atlas;

flat in int blockType;
in vec3 OutNormal;
in vec3 fragPos;
in vec2 UV;

out vec4 FragColor;

void main()
{
    // Sample texture atlas
    vec3 objectColor = texture(atlas, UV).rgb;

    vec3 norm = normalize(OutNormal);
    vec3 lightDir = normalize(light_pos - fragPos);

    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specStrength = 0.3;
    vec3 viewDir = normalize(cam_pos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specStrength * spec * lightColor;

    vec3 result = objectColor * (ambient + diffuse) + specular;

    FragColor = vec4(result, 1.0);
}
