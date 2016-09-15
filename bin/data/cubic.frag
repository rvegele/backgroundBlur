#version 120

uniform vec2 screenSize;
uniform vec2 mousePos;

uniform float spread;
uniform float falloff;
uniform float amount;

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform int piesiamApskritima;
uniform float circleRadius;
uniform float innerCircleFade;
uniform float outerCircleFade;

float cubicPulse( float c, float w, float x ){
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    // Cubic Hermine Curve
    //return amount - x*x*(falloff-2.0*x);
    // good
    
    //float fadeColor = 1.0 - smoothstep(falloff, 1.0, x);
    //return amount * fadeColor;
    
    float clr =  abs(1.0 - smoothstep( 0.0+falloff, 1.0-falloff, x)) ;
    //color = vec3(smoothstep(0.0, mousePos.x-falloff, x) * abs (1.0 - smoothstep(mousePos.x+falloff, 1.0, x) ) );
    return clr * amount;
    
}

vec3 drawCircle() {
    vec2 _st = gl_FragCoord.xy/screenSize; // position centre
    vec2 midpoint = screenSize * mousePos;
    
    float radius = circleRadius;
    float pct = 0.0;
    
    float radiusScaled = min(screenSize.x, screenSize.y) * radius;
    float dd = length(gl_FragCoord.xy - midpoint);
    float dist = dd * 0.004;
    
    float circle = 1.0 - smoothstep(radius - (radius * innerCircleFade),
                                    
                                    radius + (radius * outerCircleFade),
                                    
                                    dot (dist, dist) * 4.0 );
    
    vec3 circleColor = vec3 ( circle, circle, circle  );
    
    // good
    //return abs(1.0 - circleColor) ;
    //return amount - /*abs(1.0 -*/ circleColor/*)*/;
    return ( 1.0 - amount) - /*abs(1.0 -*/ circleColor/*)*/;
    
    // bad
    //return vec3( circleColor );
}

void main() {
    
    vec4 staticBackground = texture2DRect(tex0, texCoordVarying);
    
   if ( piesiamApskritima == 0 ) {
       vec2 st = gl_FragCoord.xy / screenSize;
       float y = cubicPulse(mousePos.x,spread,st.x);
       vec3 color = vec3(y);
       vec3 colotInv = abs(1.0 - color);
       
       gl_FragColor = vec4(vec3(staticBackground.rgb), colotInv);
       //gl_FragColor = vec4(vec3(staticBackground.rgb), 1.0 - colotInv.x);
   } else  {
       
       vec3 colotInv = drawCircle();
       gl_FragColor = vec4(vec3(staticBackground.rgb), colotInv);
       //gl_FragColor = vec4(circol, amount);
   }
}





