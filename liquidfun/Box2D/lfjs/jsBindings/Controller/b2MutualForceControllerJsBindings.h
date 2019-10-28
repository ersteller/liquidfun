#ifndef B2MUTUALFORCECONTROLLERBINDINGS_H
#define B2MUTUALFORCECONTROLLERBINDINGS_H

extern "C" {
double b2MutualForceController_Step(void* mutualForceController);
void b2MutualForceController_AddGroup(void* mutualForceController);
}
#endif
