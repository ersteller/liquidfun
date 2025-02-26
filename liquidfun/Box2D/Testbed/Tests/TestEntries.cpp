/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "../Framework/Test.h"
#include "../Framework/Render.h"
#include "../Framework/Main.h"
#include "../Framework/ParticleParameter.h"

#if defined(__APPLE__) && !defined(__IOS__)
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

//#include "AABB.h"
#include "AddPair.h"
#include "ApplyForce.h"
#include "BodyTypes.h"
#include "Breakable.h"
#include "Bridge.h"
#include "BulletTest.h"
#include "Cantilever.h"
#include "Car.h"
#include "ContinuousTest.h"
#include "Chain.h"
#include "CharacterCollision.h"
#include "CollisionFiltering.h"
#include "CollisionProcessing.h"
#include "CompoundShapes.h"
#include "Confined.h"
#include "ConvexHull.h"
#include "ConveyorBelt.h"
#include "DistanceTest.h"
#include "Dominos.h"
#include "DumpShell.h"
#include "DynamicTreeTest.h"
#include "EdgeShapes.h"
#include "EdgeTest.h"
#include "Gears.h"
#include "Mobile.h"
#include "MobileBalanced.h"
#include "MotorJoint.h"
#include "MutualForce.h"
#include "OneSidedPlatform.h"
#include "Pinball.h"
#include "PolyCollision.h"
#include "PolyShapes.h"
#include "Prismatic.h"
#include "Pulleys.h"
#include "Pyramid.h"
#include "RayCast.h"
#include "Revolute.h"
#include "RopeJoint.h"
#include "SensorTest.h"
#include "ShapeEditing.h"
#include "SliderCrank.h"
#include "SphereStack.h"
#include "TheoJansen.h"
#include "Tiles.h"
#include "TimeOfImpact.h"
#include "Tumbler.h"
#include "VaryingFriction.h"
#include "VaryingRestitution.h"
#include "VerticalStack.h"
#include "Web.h"

#include "AntiPointy.h"
#include "CornerCase.h"
#include "DamBreak.h"
#include "DrawingParticles.h"
#include "ElasticParticles.h"
#include "Faucet.h"
#include "Fracker.h"
#include "Impulse.h"
#include "LiquidTimer.h"
#include "Maxwell.h"
#include "MultipleParticleSystems.h"
#include "Particles.h"
#include "ParticleCollisionFilter.h"
#include "ParticlesSurfaceTension.h"
#include "Pointy.h"
#include "Ramp.h"
#include "RigidParticles.h"
#include "Sandbox.h"
#include "Soup.h"
#include "SoupStirrer.h"
#include "Sparky.h"
#include "WaveMachine.h"

TestEntry g_testEntries[] =
{
	{"MutualForce", MutualForce::Create},
	//{"AABB", AABBs::Create},
	{"Sandbox", Sandbox::Create},
	{"Sparky", Sparky::Create},
	{"DamBreak", DamBreak::Create},
	{"Liquid Timer", LiquidTimer::Create},
	{"Wave Machine", WaveMachine::Create},
	{"Particles", Particles::Create},
	{"Faucet", Faucet::Create},
	{"Particle Drawing", DrawingParticles::Create},
	{"Soup", Soup::Create},
	{"Surface Tension", ParticlesSurfaceTension::Create},
	{"Elastic Particles", ElasticParticles::Create},
	{"Rigid Particles", RigidParticles::Create},
	{"Multiple Systems", MultipleParticleSystems::Create},
	{"Impulse", Impulse::Create},
	{"Soup Stirrer", SoupStirrer::Create},
	{"Fracker", Fracker::Create},
	{"Maxwell", Maxwell::Create},
	{"Ramp", Ramp::Create},
	{"Pointy", Pointy::Create},
	{"AntiPointy", AntiPointy::Create},
	{"Corner Case", CornerCase::Create},
	{"Particle Collisions", ParticleCollisionFilter::Create},

	{"Ray-Cast", RayCast::Create},
	{"Dump Shell", DumpShell::Create},
	{"Convex Hull", ConvexHull::Create},
	{"Apply Force", ApplyForce::Create},
	{"Continuous Test", ContinuousTest::Create},
	{"Time of Impact", TimeOfImpact::Create},
	{"Motor Joint", MotorJoint::Create},
	{"One-Sided Platform", OneSidedPlatform::Create},
	{"Mobile", Mobile::Create},
	{"MobileBalanced", MobileBalanced::Create},
	{"Conveyor Belt", ConveyorBelt::Create},
	{"Gears", Gears::Create},
	{"Varying Restitution", VaryingRestitution::Create},
	{"Tumbler", Tumbler::Create},
	{"Tiles", Tiles::Create},
	{"Cantilever", Cantilever::Create},
	{"Character Collision", CharacterCollision::Create},
	{"Edge Test", EdgeTest::Create},
	{"Body Types", BodyTypes::Create},
	{"Shape Editing", ShapeEditing::Create},
	{"Car", Car::Create},
	{"Prismatic", Prismatic::Create},
	{"Vertical Stack", VerticalStack::Create},
	{"SphereStack", SphereStack::Create},
	{"Revolute", Revolute::Create},
	{"Pulleys", Pulleys::Create},
	{"Polygon Shapes", PolyShapes::Create},
	{"Web", Web::Create},
	{"RopeJoint", RopeJoint::Create},
	{"Pinball", Pinball::Create},
	{"Bullet Test", BulletTest::Create},
	{"Confined", Confined::Create},
	{"Pyramid", Pyramid::Create},
	{"Theo Jansen's Walker", TheoJansen::Create},
	{"Edge Shapes", EdgeShapes::Create},
	{"PolyCollision", PolyCollision::Create},
	{"Bridge", Bridge::Create},
	{"Breakable", Breakable::Create},
	{"Chain", Chain::Create},
	{"Collision Filtering", CollisionFiltering::Create},
	{"Collision Processing", CollisionProcessing::Create},
	{"Compound Shapes", CompoundShapes::Create},
	{"Distance Test", DistanceTest::Create},
	{"Dominos", Dominos::Create},
	{"Dynamic Tree", DynamicTreeTest::Create},
	{"Sensor Test", SensorTest::Create},
	{"Slider Crank", SliderCrank::Create},
	{"Varying Friction", VaryingFriction::Create},
	{"Add Pair Stress Test", AddPair::Create},

	{NULL, NULL}
};
