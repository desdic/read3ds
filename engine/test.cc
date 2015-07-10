#include "read3ds.h"

int main(int argc, char **argv) {

    iscene scene;
    i3ds loader;

    //loader.load3ds("sdscene8.3ds", scene);
    //loader.load3ds("puls7.3ds", scene);
    loader.load3ds("spaceship.3ds", scene);

    return 0;
}
