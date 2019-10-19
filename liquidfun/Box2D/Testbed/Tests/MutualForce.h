#ifndef MUTUAL_FORCE_H
#define MUTUAL_FORCE_H

#include "../../../Contributions/Enhancements/Controllers/Quadtree.h"
#include "../../../Contributions/Enhancements/Controllers/b2MutualForceController.h"

class MutualForce : 	public Test
{
public:
	MutualForce()
	{
		m_particleFlags = 0;
		m_tree = NULL;
		m_radius = 0.025f;
		m_tree = new Tree;
		m_world->SetGravity(b2Vec2(0.0f, 0.0f));
		m_mfctl = new b2MutualForceController;

		{
			/* Outer Frame */
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2ChainShape shape;
			const b2Vec2 vertices[4] = {
				b2Vec2(-20, -20),
				b2Vec2(20, -20),
				b2Vec2(20, 20),
				b2Vec2(-20, 20)};
			shape.CreateLoop(vertices, 4);
			ground->CreateFixture(&shape, 0.0f);
		}

		m_particleSystem->SetRadius(m_radius);
		m_particleSystem->SetDamping(0.2f);

		{
			/* spawn particles shape */
			b2PolygonShape shape;
			shape.SetAsBox(1.5f, 1.5f, b2Vec2(1.2f, 1.01f), 0);
			b2ParticleGroupDef pd;
			pd.flags = TestMain::GetParticleParameterValue();
			pd.shape = &shape;
			b2ParticleGroup * const group = m_particleSystem->CreateParticleGroup(pd);


			if (pd.flags & b2_colorMixingParticle)
			{
				ColorParticleGroup(group, 0);
			}

			m_mfctl->AddGroup(m_particleSystem);
		}

		/* put particles in Mutualforce controller */
		// b2Vec2* paHeadPos = m_particleSystem->GetPositionBuffer();

		TestMain::SetRestartOnParticleParameterChange(false);

	}

	// Allows you to set particle flags on devices with keyboards
	void Keyboard(unsigned char key)
	{
		uint32 toggle = 0;
		float flDamping = m_particleSystem->GetDamping();
		switch (key)
		{
			case 'a':
				m_particleFlags = 0;
				break;
			case 'q':
				toggle = b2_powderParticle;
				break;
			case 't':
				toggle = b2_tensileParticle;
				break;
			case 'v':
				toggle = b2_viscousParticle;
				break;
			case 'w':
				toggle = b2_wallParticle;
				break;

			case 'g':
				if (m_world->GetGravity() == b2Vec2(0.0f, 0.0f))
					m_world->SetGravity(b2Vec2(0.0f, -1.0f));
				else
					m_world->SetGravity(b2Vec2(0.0f, 0.0f));
				break;

			case 'd':
				flDamping += 0.1f;		
				break;
			case 'e':
				flDamping -= 0.1f;
				break;
			default:
				//printf("0x%X",key);
				break;
			

		}
		printf("Damping: %f \n", flDamping);
		m_particleSystem->SetDamping(flDamping);

		if (toggle) {
			if (m_particleFlags & toggle) {
				m_particleFlags = m_particleFlags & ~toggle;
			} else {
				m_particleFlags = m_particleFlags | toggle;
			}
		}
		printf("setting: 0x%X\n",m_particleFlags);
		TestMain::SetParticleParameterValue(m_particleFlags);
	}

	void Step(Settings *settings)
	{
		int32 iParticleCount =  m_particleSystem->GetParticleCount();	
		b2Vec2* paHeadPos = m_particleSystem->GetPositionBuffer();
		b2ParticleColor* paColor = m_particleSystem->GetColorBuffer();



		b2Vec2 vPos;

		b2Vec2 vDistance;
		b2Vec2 vForce;
		float32 fltForce;
		float32 fltDistance;

		b2Vec2 CoM;
		float32 mass;
		
		// Constants 
		//float32 fltMass = 1;
		float32 fltOurG = 0.00002f;


        /* this shall be all of the interface needed to be provided */
		// iParticleCount
		// paHeadPos
		// paColor

		paColor[0].Set(0,0,255,255);


		//m_particleSystem->SetColorBuffer(paColor, 1);	
		m_mfctl->Step();
		Test::Step(settings);
	}



	Tree* m_tree;
	float32 m_radius;
	b2MutualForceController* m_mfctl;

	float32 GetDefaultViewZoom() const
	{
		return 0.1f;
	}

	static Test* Create()
	{
		return new MutualForce;
	}

	/* TODO cleanup some code () */

private:
	uint32 m_particleFlags;
};



#endif // MUTUAL_FORCE_H