#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;

#define MAX_LIGHTS 64

struct Light {
    int type;               // 0=ambient, 1=point, 2=directional, 3=reflector
    vec3 color;             
    float intensity;    
    mat4 lightMatrix;       
    float constant;         
    float linear;           
    float quadratic;        
    vec3 direction;         //for directional and reflector
    float angle;            //for ref
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
    
    vec3 result = vec3(0.0, 0.0, 0.0);
    
    for (int i = 0; i < numberOfLights; i++) {
        vec3 lightResult = vec3(0.0);
        
        // Ambient složka
        float ambientStrength = 0.001;
        vec3 ambient = lights[i].color * ambientStrength * lights[i].intensity;



        if (lights[i].type == 0) { //AMBIENT
            lightResult =  lights[i].color * lights[i].intensity;

        }
        else if (lights[i].type == 1) { //point
            vec3 lightPos = vec3(lights[i].lightMatrix[3]);

            float distance = length(lightPos - FragPos);
            float attenuation = calculateAttenuation(distance, lights[i].constant, lights[i].linear, lights[i].quadratic);

            //diffuse
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = lights[i].color * diff * lights[i].intensity;
        
            //specular
            float specularStrength = 0.2;
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = lights[i].color * spec * specularStrength * lights[i].intensity;
        
            //attenuation
            diffuse = diffuse * attenuation;
            specular = specular * attenuation;

            //resultlight
            lightResult = ambient + diffuse + specular;
        }



        else if (lights[i].type == 2) { //directional
            vec3 lightDir = normalize(-lights[i].direction);
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = lights[i].color * diff * lights[i].intensity;
            
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = lights[i].color * spec * 0.5 * lights[i].intensity;
            
            lightResult = ambient + diffuse + specular;
        }
else if (lights[i].type == 3) { //reflector
    vec3 lightPos = vec3(lights[i].lightMatrix[3]);
    vec3 lightDir = normalize(lightPos - FragPos);
    

    float distance = length(FragPos - lightPos);
    
    //make  come perspective
    float perspectiveAngle = lights[i].angle * (1.0 + distance * 0.05);
    float alpha = cos(radians(perspectiveAngle));
    
    float spot = dot(normalize(-lights[i].direction), lightDir);
    
    if (spot >= alpha) {
        float coneAttenuation = (spot - alpha) / (1.0 - alpha);
        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].color * diff * lights[i].intensity;
        
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = lights[i].color * spec * 0.5 * lights[i].intensity;
        
        float distAttenuation = calculateAttenuation(distance, lights[i].constant, lights[i].linear, lights[i].quadratic);
        
        lightResult = ambient + (diffuse + specular) * coneAttenuation * distAttenuation;
    }
    
    else {
        lightResult = ambient;
    }
}
        
        result += lightResult;
    }
    
    result = result * norm;
    FragColor = vec4(result, 1.0);
}