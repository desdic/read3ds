#ifndef __LIGHT_H__
#define __LIGHT_H__

//http://jerome.jouvie.free.fr/OpenGl/Tutorials/Tutorial13.php

class Light {
public:

    

    float posx,posy,posz, posw;
    bool enabled;
};

class PositionalLight: public Light {
};

class DirectionalLight: public Light {
};

class SpotLight: public Light {
    float cutoff;
};

#endif
