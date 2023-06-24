#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 vColor;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 color;

uniform vec3 lightPos;

uniform vec3 lightColor;

uniform vec3 viewPos;

uniform int shininess;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform int shaderChoice;

void main()
{    
    switch(shaderChoice){
        case 0: 
            float ambientStrength = 0.1;
    
            // Ambient Light
            vec3 ambient = ambientStrength * vColor;
    
            // Point Light
            vec3 norm = normalize(Normal); 
            vec3 lightDir = normalize(lightPos - FragPos);
    
            float diff = max(dot(norm, lightDir), 0.0); 
            vec3 diffuse = diff * lightColor;
    
            float specularStrength = 0.5;
    
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
    
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
            vec3 specular = specularStrength * spec * lightColor;
    
            // Directional Light
            vec3 dirLightDirNorm = normalize(dirLightDir);
            float dirDiff = max(dot(norm, -dirLightDirNorm), 0.0); 
            vec3 dirDiffuse = dirDiff * dirLightColor;
    
            vec3 dirReflectDir = reflect(dirLightDirNorm, norm);
            float dirSpec = pow(max(dot(viewDir, dirReflectDir), 0.0), shininess); 
            vec3 dirSpecular = specularStrength * dirSpec * dirLightColor;
    
            vec3 result = (ambient + diffuse + specular + dirDiffuse + dirSpecular) * vColor; 
            //vec3 result = (ambient+dirSpecular+dirDiffuse) * vColor; 
            FragColor = vec4(result, 1.0);
            break;

        default:
            FragColor = vec4(vColor, 1.0);
            break;
    }

}