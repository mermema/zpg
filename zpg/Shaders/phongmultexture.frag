#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor; //fallback

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;
};
uniform Material material;
uniform bool hasTexture;

#define MAX_LIGHTS 64

struct Light {
    int type;
    vec3 color; 
    float intensity; 
    mat4 lightMatrix; 
    float constant; 
    float linear; 
    float quadratic; 
    vec3 direction; 
    float angle; 
};
uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;

float calculateAttenuation(float distance, float constant, float linear, float quadratic) {
    float denominator = constant + linear * distance + quadratic * (distance * distance);
    if (denominator <= 0.001) return 1.0;
    return 1.0 / denominator;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 materialDiffuse;
    if (hasTexture) {
        materialDiffuse = texture(material.diffuseMap, TexCoords).rgb;
    } else {

            materialDiffuse = material.diffuse;
    
    }
    
    vec3 result = vec3(0.0);
    
    for (int i = 0; i < numberOfLights; i++) {
        vec3 lightResult = vec3(0.0);
        vec3 ambient = material.ambient * lights[i].color * lights[i].intensity;

        if (lights[i].type == 0) { //ambient
            lightResult = material.ambient * lights[i].color * lights[i].intensity;
        }
        else if (lights[i].type == 1) { //point
            vec3 lightPos = vec3(lights[i].lightMatrix[3]);
            float distance = length(lightPos - FragPos);
            float attenuation = calculateAttenuation(distance, lights[i].constant, lights[i].linear, lights[i].quadratic);

            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = materialDiffuse * diff * lights[i].color * lights[i].intensity;
        
            float spec = 0.0;
            if (diff > 0.0) {
                vec3 reflectDir = reflect(-lightDir, norm);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            }
            vec3 specular = material.specular * spec * lights[i].color * lights[i].intensity;
        
            diffuse = diffuse * attenuation;
            specular = specular * attenuation;

            lightResult = ambient + diffuse + specular;
        }
        else if (lights[i].type == 2) { //directional
            vec3 lightDir = normalize(-lights[i].direction);
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = materialDiffuse * diff * lights[i].color * lights[i].intensity;
            
            float spec = 0.0;
            if (diff > 0.0) {
                vec3 reflectDir = reflect(-lightDir, norm);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            }
            vec3 specular = material.specular * spec * lights[i].color * lights[i].intensity;
            
            lightResult = ambient + diffuse + specular;
        }
        else if (lights[i].type == 3) { //reflecttor
            vec3 lightPos = vec3(lights[i].lightMatrix[3]);
            vec3 lightDir = normalize(lightPos - FragPos);//normalized vector betwean fragment and light
            
            float distance = length(FragPos - lightPos);
            float perspectiveAngle = lights[i].angle;//* (1.0 + distance * 0.05);
            float alpha = cos(radians(perspectiveAngle));
            
            float spot = dot(normalize(-lights[i].direction), lightDir);
            
            if (spot >= alpha) {
                float coneAttenuation = (spot - alpha) / (1.0 - alpha);
                
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = materialDiffuse * diff * lights[i].color * lights[i].intensity;
                
                float spec = 0.0;
                if (diff > 0.0) {
                    vec3 reflectDir = reflect(-lightDir, norm);
                    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                }

                vec3 specular = material.specular * spec * lights[i].color * lights[i].intensity;
                
                float distAttenuation = calculateAttenuation(distance, lights[i].constant, lights[i].linear, lights[i].quadratic);
                
                lightResult = ambient + (diffuse + specular) * coneAttenuation * distAttenuation;
            }
            else {
                lightResult = ambient;
            }
        }
        
        result += lightResult;
    }
    
    FragColor = vec4(result, 1.0);
}