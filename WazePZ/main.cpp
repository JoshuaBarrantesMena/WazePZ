#include "Map.h"

int main(){

    Map waze;

    while (waze.isRunning()) {
        waze.update();
        waze.updateDebug();
        waze.render();
    }

    return 0;
}