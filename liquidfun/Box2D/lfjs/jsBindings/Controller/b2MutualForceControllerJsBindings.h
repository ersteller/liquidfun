#ifndef B2MUTUALFORCECONTROLLERBINDINGS_H
#define B2MUTUALFORCECONTROLLERBINDINGS_H

extern "C" {
void b2MutualForceController_Step(void* mutualForceController);
void b2MutualForceController_AddGroup(void* mutualForceController, void* /*b2ParticleSystem**/ particleSystem);
}
#endif
