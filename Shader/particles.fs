#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 vColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 Position;

uniform sampler2D texture_diffuse1;

uniform vec3 color;

uniform vec3 lightPos;

uniform vec3 lightColor;

uniform vec3 viewPos;

uniform int shininess;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform int shaderChoice;

uniform samplerCube skybox; 

uniform float time;


vec3 calculateColor(float angle){

    vec3 color;
    if (angle < 60.0) {
        color.r = 1.0;
        color.g = angle / 60.0;
        color.b = 0.0;
    } else if (angle < 120.0) {
        color.r = (120.0 - angle) / 60.0;
        color.g = 1.0;
        color.b = 0.0;
    } else if (angle < 180.0) {
        color.r = 0.0;
        color.g = 1.0;
        color.b = (angle - 120.0) / 60.0;
    } else if (angle < 240.0) {
        color.r = 0.0;
        color.g = (240.0 - angle) / 60.0;
        color.b = 1.0;
    } else if (angle < 300.0) {
        color.r = (angle - 240.0) / 60.0;
        color.g = 0.0;
        color.b = 1.0;
    } else {
        color.r = 1.0;
        color.g = 0.0;
        color.b = (360.0 - angle) / 60.0;
    }
    return color;
}

vec3 calculateDirLight(){
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
    return result;
}

vec3 calculateReflection(){
    float ratio = 1.00 / 1.52;
	vec3 I = normalize(Position - viewPos); 
	vec3 reflection = reflect(I, normalize(Normal)); 
    reflection = texture(skybox, reflection).rgb;
	vec3 refraction = refract(I, normalize(Normal), ratio); //für Refraction
    return reflection;
}

vec3 calculateGradient(){
    float maxHeight = 1000.0;
    float height = (Position.x+maxHeight)/(2*maxHeight);

    float angle = height * 360.0;
    vec3 color = calculateColor(angle);
    return color;
}

vec3 calculateTimeGradient(){
    float maxLength = 1000.0;
    float length = sqrt(Position.x*Position.x + Position.y*Position.y + Position.z*Position.z);

    float distance = (length+maxLength)/maxLength - sqrt(sin(time/2)*sin(time/2))*1.5;

    float angle = distance * 360.0;
    vec3 color = calculateColor(angle);
    return color;
}
void main()
{    
    switch(shaderChoice){
        case 0: 
            vec3 result = calculateDirLight();
            FragColor = vec4(result, 1.0);
            break;
        case 1:
            vec3 reflection = calculateReflection();
            FragColor = vec4(reflection, 1.0);
            break;
        case 2: 
            FragColor = vec4(Position, 1.0f);
            break;

        case 3: 
            vec3 gradient = calculateGradient();
            FragColor = vec4(gradient, 1.0f);
            break;

        case 4: 
            vec3 timeGradient = calculateTimeGradient();
            FragColor = vec4(timeGradient, 1.0f);
            break;

        case 5: 
            vec3 shading;

            float distance = sqrt(Position.x*Position.x + Position.y*Position.y + Position.z*Position.z);

            if(distance <= 200){
                shading = calculateTimeGradient();
            } else if(distance <= 400){
                shading = calculateReflection();
            } else if(distance <= 600){
                shading = calculateDirLight();
            } else if(distance <= 800){
                shading = calculateGradient();
            } else if(distance <= 1000){
                shading = calculateGradient();
            } else{
                shading = vColor;
            }

            FragColor = vec4(shading, 1.0);

            break;

        default:
            FragColor = vec4(vColor, 1.0);
            break;
    }

}