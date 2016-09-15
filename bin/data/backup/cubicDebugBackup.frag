#version 120

uniform vec2 screenSize;
uniform vec2 mousePos;

uniform float spread;
uniform float falloff;
uniform float amount;

uniform int piesiamApskritima;
uniform float circleRadius;
uniform float innerCircleFade;
uniform float outerCircleFade;

// -----
float cubicPulse( float c, float w, float x ){
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    // Cubic Hermine Curve
    //return amount - x*x*(falloff-2.0*x);
    
    return amount - smoothstep(falloff, 1., x);
}

vec3 drawCircle() {
    vec2 _st = gl_FragCoord.xy/screenSize; // position centre
    
    //vec2 midpoint = screenSize.xy * 0.5;
    vec2 midpoint = screenSize * mousePos;//x = abs(x - c);
    
    float radius = circleRadius;
    float pct = 0.0;
    
    float radiusScaled = min(screenSize.x, screenSize.y) * radius;
    float dd = length(gl_FragCoord.xy - midpoint);
    float dist = dd * 0.004;
    //float dist = length(gl_FragCoord.xy - midpoint);
    //float dist = distance(_st,vec2(0.5));
    
    // The DISTANCE from the pixel to the center
    //pct = distance(_st,vec2(0.5));
    //vec2 dist = _st - vec2(0.5,0.5);
    //float circle = 1.0 - smoothstep(1.0 - 1.0 * 0.50,
    //
    //                                1.0 + 1.0 * 0.50,
    //
    //                                dot( dist , dist ) / 0.10);
    
    float circle = 1.0 - smoothstep(radius - (radius * innerCircleFade),
                                    
                                    radius + (radius * outerCircleFade),
                                    
                                    dot (dist, dist) * 4.0 );
    
    vec3 circleColor = vec3 ( circle, circle, circle  );
    //circleColor = abs (circleColor - 1.0);
    //return abs (1.0 - circleColor) ;
    
    // good
    //return vec3( circleColor );
    return amount - abs(1.0 - circleColor);
}


void main() {
    
    if ( piesiamApskritima == 0 ) {
        vec2 st = gl_FragCoord.xy / screenSize;
        float y = cubicPulse(mousePos.x,spread,st.x);
        vec3 color = vec3(y);
        vec3 colotInv = vec3(1.0 - color);
        gl_FragColor = vec4(color, 1.0);
    } else { // draw circle
        vec3 circol = drawCircle();
        gl_FragColor = vec4(circol, 1.0);
    }
}


