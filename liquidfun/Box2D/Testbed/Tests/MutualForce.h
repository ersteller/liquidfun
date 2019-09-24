#ifndef MUTUAL_FORCE_H
#define MUTUAL_FORCE_H

#include "Quadtree.h"

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
			shape.SetAsBox(0.8f, 0.8f, b2Vec2(1.2f, 1.01f), 0);
			b2ParticleGroupDef pd;
			pd.flags = TestMain::GetParticleParameterValue();
			pd.shape = &shape;
			b2ParticleGroup * const group = m_particleSystem->CreateParticleGroup(pd);
			if (pd.flags & b2_colorMixingParticle)
			{
				ColorParticleGroup(group, 0);
			}
		}

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

	virtual void Step(Settings *settings)
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
		//float32 mass = body->GetMass();
		float32 fltOurG = 0.00002f;




		// once every steps
		if (this->m_stepCount % 1 == 0)
		{	
			m_tree->cleanup(); 
			m_tree->setup(paHeadPos, iParticleCount);
			//printf(" ################## add to tree ");
			for (int32 idx = 0; idx < iParticleCount; idx++)
			{
				//printf("%d ",idx);
				m_tree->add(&paHeadPos[idx], m_particleSystem->GetParticleHandleFromIndex(idx));
			}
			//printf("\n\n\n\n");
			// m_tree->draw(m_world);
		}
		


		for (int32 idx = 0; idx < iParticleCount; idx++)
		{
			// particle of interest
			vPos = paHeadPos[idx];
			//paColor[idx].Set(255,255,255,128);
			vForce = b2Vec2(0.0f, 0.0f);
			float32 k = 0.5f;

			//printf("\n %d with \n",idx);

			int iterations = 0;
			// float32 seenmass = 0;  // debug help
			for (Tree* it = m_tree->iterator()->Next(k,&vPos);
				it != NULL;
				it = it->Next(k,&vPos))
			{
				//printf("    d:%d X:%f Y:%f K:%f \n",it->GetDepth() ,it->midX, it->midY, it->GetK(&vPos));
				

				// highlight all nodes of first particle
				//if (idx == 0)
				//{
					//char szAncestors[200] = {0};
					//it->drawonlythis(m_world);

					//printf("idx %d iteration %d treeid %d %s\n",idx,iterations,  (int)it , it->getAncestors(szAncestors));
				//}

				iterations++;

				mass = it->Mass;
				//seenmass +=mass; // debug help
				CoM = it->CenterOfMass;

				vDistance = CoM - vPos;
				fltDistance = vDistance.Length();
				
				if ( fltDistance < m_radius/2)
				{

					// this is too cloose to exist must be touching forces or self
					//printf(" ### skipping distance %f idx %d iteration %d treeid %d\n",fltDistance,idx,iterations,  (int)it );
					continue;
				}

				vDistance.Normalize();

				fltForce = fltOurG * mass / (fltDistance * fltDistance) ; //b2Dot(vDistance,vDistance)
				vForce += vDistance * fltForce;	
			}

			//printf("idx %d seenmass %f\n",idx, seenmass);

			/*printf("  %f Force \n",vForce.Length());*/

			/* // direct method
			for (int32 jdx = 0; jdx < iParticleCount; jdx++)
			{	
				if( jdx == idx)
				{
					continue;
				}

				vDistance = paHeadPos[jdx] - vPos;
				//if( 5 < (
				fltDistance = vDistance.Length();
				//))
				//{   
				//	// limit distance
				//	continue;
				//}

				vDistance.Normalize();

				fltForce = fltOurG / (fltDistance*fltDistance) ; //b2Dot(vDistance,vDistance)
				vForce += vDistance * fltForce;	
			}*/

			float32 force = vForce.Length();
			if (force > 10)
			{
				// limit force
				//printf("normalyze idx: %d\n",idx);
				vForce.Normalize();
				paColor[idx].Set(200,10,10,250);
			}
			else 
			{
				paColor[idx].Set( (uint8)(255*(1-force)), (uint8)(255*(1-force)), (uint8)(255*(1-force)),(uint8)(255*(1-force)));
			}

			// aply sum of all forces to particle
			m_particleSystem->ParticleApplyForce( idx, vForce);	
			//printf("iterations %d\n\n",iterations);
		}
		paColor[0].Set(0,0,255,255);


		//m_particleSystem->SetColorBuffer(paColor, 1);	
		Test::Step(settings);
	}



	Tree* m_tree;
	float32 m_radius;

	float32 GetDefaultViewZoom() const
	{
		return 0.1f;
	}

	static Test* Create()
	{
		return new MutualForce;
	}

private:
	uint32 m_particleFlags;
};



#endif // MUTUAL_FORCE_H