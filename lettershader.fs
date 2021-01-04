#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;
uniform bool isTransparency; //sq queremos aplicar transparência na textura através do preto/transparente
uniform bool isTexture;


void main()
{    
	
	if(isTexture){
		vec4 texcolor = texture(text, TexCoords);
		if((texcolor.r <= 0.5 && texcolor.g <=0.5 && texcolor.b <=0.5 && isTransparency) || (texcolor.a <= 0.5 && isTransparency)){
			discard;
		}
		vec4 sampled;
		if(texcolor.r >= 0.50 && texcolor.g <= 0.0 && texcolor.b <=0.0){
			sampled = vec4(1.0,1.0,1.0,texcolor.r);
		}else{
			sampled = vec4(texcolor.rgb, 1.0);
		}
		color = vec4(textColor, 1.0) * sampled;
	}
	else{
		color = vec4(textColor, 1.0);
	}
	
}  