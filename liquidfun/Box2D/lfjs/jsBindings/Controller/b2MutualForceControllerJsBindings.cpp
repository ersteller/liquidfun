#include<Box2D/Box2D.h>


void b2MutualForceController_Step(void* mutualFrcCtrl /*const b2TimeStep& step*/) {
    ((b2MutualForceController*)mutualFrcCtrl)->Step();
}

void b2MutualForceController_AddGroup(void* mutualFrcCtrl, b2ParticleSystem* particleSystem) {
    ((b2MutualForceController*)mutualFrcCtrl)->AddGroup(particleSystem)
}