varying vec3 lightDir,normal;
uniform sampler2D tex;

#define textureWidth 80.0
#define textureHeight 80.0     
#define texel_size_x 1.0 / textureWidth
#define texel_size_y 1.0 / textureHeight

vec4 texture2D_bilinear( sampler2D tex, vec2 uv )
{
        vec2 f;

        f.x  = fract( uv.x * textureWidth );
        f.y  = fract( uv.y * textureHeight );

        vec4 t00 = texture2D( tex, uv + vec2( 0.0, 0.0 ));
        vec4 t10 = texture2D( tex, uv + vec2( texel_size_x, 0.0 ));
        vec4 tA = mix( t00, t10, f.x);

        vec4 t01 = texture2D( tex, uv + vec2( 0.0, texel_size_y ) );
        vec4 t11 = texture2D( tex, uv + vec2( texel_size_x, texel_size_y ) );
        vec4 tB = mix( t01, t11, f.x );

        return mix( tA, tB, f.y );
}

void main()
{
	vec3 ct,cf;
	vec4 texel;
	float intensity,at,af;
	
	intensity = max(dot(lightDir,normalize(normal)),0.0);
	
	cf = intensity * (gl_FrontMaterial.diffuse).rgb + 
					  gl_FrontMaterial.ambient.rgb;
	af = gl_FrontMaterial.diffuse.a;
	
	texel = texture2D_bilinear(tex,gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af);	
}
