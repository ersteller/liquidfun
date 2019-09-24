#pragma once
#include <limits>
#include <deque>
//#define max(x,y) x>=y?x:y

/* this should be usable like:
adding particles 
use iterator(curpos, qoutient) of tree and let tree decide which node you get

*/

/* 
stolen from https://code.google.com/p/kyle/
	addons and demos for openframework
 maxParticles describes how many particles are allowed
 in each leaf. a normal quadtree only has 1 particle per
 leaf. this can cause issues if two particles are very close
 together, as it causes a large number of subdivisions.
 in an evenly distributed system, if maxParticles has a high
 value, the tree will approximate a binning system.
*/




/* TODO: define qoutient s/d (spread / distance) to conditionally use above*/

/* TODO: binning: find large bodies of connectect particles */
/* 1. make round static bodys to a bin */
/* 2. tidal force or impact should break bins (testcase: check roche border) d= 2.423 * R * (rohM/rohm)^(1/3)*/

#define maxParticles 1

class Tree
{
public:
	
	struct HandleAndPos
	{
		b2ParticleHandle* handle;
		b2Vec2*           pos;
	};

	int nParticles;
	int nSubParticles;
	std::deque<HandleAndPos*> particles;
	std::deque<HandleAndPos*> subparticles;
	std::deque<HandleAndPos*>* ptmpparticles;
	b2Vec2 CenterOfMass;
	float32 Mass;

	bool hasChildren;
	Tree *nw, *ne, *sw, *se; 
	Tree *parent;
	float minX, minY, midX, midY, maxX, maxY;

	/* used for debug rendering tree */
	b2World* m_world;
	b2Body* m_debugBody;
	
	Tree() :
		nParticles(0),
		nSubParticles(0),
		hasChildren(false),
		CenterOfMass(0.f,0.f),
		Mass(0),
		parent(NULL),
		m_debugBody(NULL),
		nw(NULL),ne(NULL),
		sw(NULL),se(NULL),
		minX(0), minY(0),
		midX(0), midY(0),
		maxX(0), maxY(0) {
	}
	Tree(float _minX, float _minY, float _maxX, float _maxY, Tree* pparent) :
		nParticles(0),
		nSubParticles(0),
		hasChildren(false),
		CenterOfMass(0.f,0.f),
		Mass(0),
		parent(pparent),
		m_debugBody(NULL),
		nw(NULL),ne(NULL),
		sw(NULL),se(NULL),
		minX(_minX), minY(_minY),
		maxX(_maxX), maxY(_maxY) {
		setMid();
	}

	// not in step() please 
	~Tree() {
		destroyBox();
		delParticles();
		if(hasChildren || nw != NULL) {
			delete nw; 
			delete ne;
			delete sw;
			delete se;
			nw = NULL;
			ne = NULL;
			sw = NULL;
			se = NULL;
		}
	}

	void setMid() {
		midX = (minX + maxX) / 2;
		midY = (minY + maxY) / 2;
	}

	b2Vec2* GetNewDirection(b2Vec2* ptOrigin)
	{
		b2Vec2* ptRes = new b2Vec2;

		// direction
		ptRes->x = midX - ptOrigin->x;
		ptRes->y = midY - ptOrigin->y;
		return ptRes;
	}


	Tree* iterator()
	{
		if(parent)
			return parent->iterator(); // returns root pointer
		else
			return this;
	}

	int GetDepth()
	{
		Tree* w = this;
		int d = 0;
		while (w->parent != NULL)
		{
			d++;
			w = w->parent;
		}
		return d;
	}

	char* getAncestors(char* buff)
	{
		Tree* w = this;
		int d = GetDepth() * 3;
		memcpy(&buff[0], " ro", 3);
		while (w->parent != NULL)
		{
			if(w == w->parent->nw)
				memcpy(&buff[d], "_nw", 3);
			else if(w == w->parent->ne)
				memcpy(&buff[d], "_ne", 3);		
			else if(w == w->parent->sw)
				memcpy(&buff[d], "_sw", 3);
			else if(w == w->parent->se)
				memcpy(&buff[d], "_se", 3);
			else 
				assert(0);
			d-=3;
			assert(d>=0);
			w = w->parent;
		}	 
		return buff;
	}

	// k error between 0 and 1 
	// if lager than 1 we are only evaluating root 
	// if k == 0 we use direct method
	float32 GetK(b2Vec2* cur)
	{
		// spread of box
		b2Vec2 direction;
		float32 radius = ((midY - minY) * 2);
	    direction.x = midX - cur->x;
		direction.y = midY - cur->y;
		float32 length = abs(direction.x) + abs(direction.y); // direction.Length(); // this takes 10%of all time direction.Length(); 


		// k is an indicator for expectet error impact 
		// sensable between 0 and 1 if lager than 1 we are very close to or inside this node
		float32 k = 0;
		if (length != 0)
			k = radius / length;
		else 
			k = std::numeric_limits<float>::max();		
		return k;
	}


	/*Tree* oldNext(float32 kerror, b2Vec2* cur)
	{
		Tree* ret = NULL;
		Tree* worker = this;
		return NULL;
	}*/


	/// find next node after this one (sequence is nw --> ne --> sw --> se --> parent-parent)
	Tree* Next(float32 kerror, b2Vec2* cur)
	{	
		Tree* ret = NULL;
		Tree* worker = this;

		/* use short circuit before calling GetK */
		if (worker->hasChildren && worker->GetK(cur) > kerror )
		{
			while (worker->hasChildren && worker->GetK(cur) > kerror)
			{
				// we should go deeper
				worker = worker->nw;
			}
			// we are now at the correct level
			// skip border nodes
			if (worker->nSubParticles != 0 || worker->nParticles != 0)
				ret = worker;
			else 
				ret = worker->Next(kerror,cur);
			return ret;
		}

		while (worker->parent != NULL && worker == worker->parent->se )
		{
			// at the end of children go up
			worker = worker->parent; 
			// and one more next to the side 
		}
				
		// get next of same layer 
		if (worker->parent != NULL)
		{
			if(worker == worker->parent->nw)
				worker = worker->parent->ne;
			else if(worker == worker->parent->ne)
				worker = worker->parent->sw;			
			else if(worker == worker->parent->sw)
				worker = worker->parent->se;
			else 
				assert(0); // did not find itself in the children of parent
			

			while (worker->hasChildren && worker->GetK(cur) > kerror)
			{
				// we should go deeper
				worker = worker->nw;
			}
			// we are now at the correct level
			// skip border nodes
			if (worker->nSubParticles != 0 || worker->nParticles != 0)
				ret = worker;
			else 
				ret = worker->Next(kerror,cur);
			return ret;


		}
		// we arrived in root node 
		else 
			return NULL;	
	}

	std::deque<HandleAndPos*>* GetParticles()
	{	// member of the tree updatet with each add function
		if (hasChildren)
		{ 
			if (!subparticles.size())
			{ // try to avoid collecting child particles too many times
				assert(0); // we should have suparticles traked by add function
			}
			return &subparticles;
		}
		else
			return &particles;
	}

	void UpdateCenterOfMass(b2Vec2* cur, float32 CurMass)
	{
		float32 NewMass = Mass + CurMass;

		/* arithmetic middle incrementally  */
		CenterOfMass.x = CenterOfMass.x * Mass / NewMass + cur->x * CurMass / NewMass;
		CenterOfMass.y = CenterOfMass.y * Mass / NewMass + cur->y * CurMass / NewMass;
		Mass = NewMass;
	}

	HandleAndPos* add(b2Vec2* cur, const b2ParticleHandle* phP, HandleAndPos* ptHPin = NULL) {
		// TODO: maybe optimizable
		HandleAndPos* pRes = NULL;
		
		if(hasChildren) 
		{
			if(cur->x < midX) 
			{
				if(cur->y < midY) 
				{
					pRes = nw->add(cur, phP, ptHPin);
				} 
				else 
				{
					pRes = sw->add(cur, phP, ptHPin);
				}
			} 
			else 
			{
				if(cur->y < midY) 
				{
					pRes = ne->add(cur, phP, ptHPin);
				} 
				else 
				{
					pRes = se->add(cur, phP, ptHPin);
				}
			}
			// add the current particle to the subparticle list because we have added it to the child
			nSubParticles++;
			subparticles.push_back(pRes);
			
			/* update center of mass */
			if(ptHPin == NULL)
			{  // so this is a new/unknown particle (not moved to children from parent)
				float32 CurMass = 1; //TODO: mass of this particle may be individuel
				UpdateCenterOfMass(cur, CurMass);
			}
		} 
		else 
		{
			if(nParticles < maxParticles) {
				if (ptHPin == NULL)
				{										
					ptHPin = new HandleAndPos;
					ptHPin->pos = cur;
					ptHPin->handle = (b2ParticleHandle*)phP;
				}
				pRes = ptHPin;
				particles.push_back(ptHPin); 
				nParticles++;
				
				{ 
				    float32 CurMass = 1; //TODO: mass of this particle may be individuel
				    UpdateCenterOfMass(cur, CurMass);
			    }

				
			} else {

				if(nw == NULL)
				{
					nw = new Tree(minX, minY, midX, midY, this);
					ne = new Tree(midX, minY, maxX, midY, this);
					sw = new Tree(minX, midY, midX, maxY, this);
					se = new Tree(midX, midY, maxX, maxY, this);
				}
				else
				{ 
					/* only update positions*/
					nw->minX = minX;
					nw->minY = minY;
					nw->maxX = midX;
					nw->maxY = midY;
					nw->setMid();

					ne->minX = midX;
					ne->minY = minY;
					ne->maxX = maxX;
					ne->maxY = midY;
					ne->setMid();

					sw->minX = minX;
					sw->minY = midY;
					sw->maxX = midX;
					sw->maxY = maxY;
					sw->setMid();

					se->minX = midX;
					se->minY = midY;
					se->maxX = maxX;
					se->maxY = maxY;
					se->setMid();
				}
				hasChildren = true;

				/* TODO: Binning here somewhere */

				/* move particles to children*/
				HandleAndPos* ptHP = NULL;
				std::deque<HandleAndPos*>::iterator it;
				while(nParticles > 0)
				{
					it = particles.begin();
					ptHP = it[0];
					// calling add again but now we have children and pointer
					add(ptHP->pos, ptHP->handle, ptHP);

					// We keep track of all subparticles when we have children 
					particles.pop_front();
					nParticles--;
				}
				assert(nParticles == 0);
				// finally adding the current particle now reentering to add to correct child 
				pRes = add(cur, phP);
			}
		}
		return pRes; // return pointer of partricle for subparticle list of parents
	}

	void setup(b2Vec2* all, int32 iParticleCount) 
	{
		int n = iParticleCount;
		if(n > 0) 
		{
			// find boundaries
			minX = all[0].x;
			minY = all[0].y;
			maxX = minX;
			maxY = minY;

			for(int i = 0; i < n; i++) 
			{
				if(all[i].x < minX)
					minX = all[i].x;
				if(all[i].y < minY)
					minY = all[i].y;
				if(all[i].x > maxX)
					maxX = all[i].x;
				if(all[i].y > maxY)
					maxY = all[i].y;
			}

			// center and square boundaries
			setMid();
			float diffX = maxX - minX;
			float diffY = maxY - minY;
			float halfSide = std::max(diffX, diffY) / 2;
			minX = midX - halfSide;
			maxX = midX + halfSide;
			minY = midY - halfSide;
			maxY = midY + halfSide;

			//draw(); //debugdraw
		}
	}


	void delParticles()
	{
		if (nParticles > 0)
		// delete all storage structures in the particle queue
		for (std::deque<HandleAndPos*>::iterator it = particles.begin();
				it != particles.end();
				it++ )
		{
			delete it[0];
		}
		particles.clear();
		nParticles = 0;
	}


	/*cleanup tree 
	particlereferences are removed from tree
	nodes without particles and children (border cleanup)
	delets particle list 
	keeps children for later use if they just had particles or subparticles */
	void cleanup()
	{
		if (hasChildren)
		{
			nw->cleanup();
			ne->cleanup();
			sw->cleanup();
			se->cleanup();
		}
		else if(nw != NULL)
		{ // in this node no children were required 
			delete nw;
			delete ne;
			delete sw;
			delete se;
			nw = NULL;
			ne = NULL;
			sw = NULL;
			se = NULL;
		}

		delParticles();
		hasChildren = false;

		subparticles.clear();
		nSubParticles = 0;

		CenterOfMass.x = 0.f;
		CenterOfMass.y = 0.f;
		Mass = 0; 

		destroyBox();
	}



	/*
	vector<Particle*> getNeighbors(float targetX, float targetY, float radius) {
		vector<Particle*> intersection, neighbors;
		getIntersection(intersection, targetX, targetY, radius);
		float xd, yd, rsq;
		float maxrsq = radius * radius;
		int n = intersection.size();
		for(int i = 0; i < n; i++) {
			Particle& cur = *intersection[i];
			xd = targetX - cur.x;
			yd = targetY - cur.y;
			rsq = xd * xd + yd * yd;
			if(rsq < maxrsq)
				neighbors.push_back(&cur);
		}
		return neighbors;
	}*/
	/*
	void getIntersection(vector<Particle*>& intersection, float targetX, float targetY, float radius) {
		if(targetX > minX - radius && targetX < maxX + radius &&
			targetY > minY - radius && targetY < maxY + radius) {
			if(nParticles) {
				for(int i = 0; i < nParticles; i++)
					intersection.push_back(particles[i]);
			} else if(hasChildren) {
				nw->getIntersection(intersection, targetX, targetY, radius);
				ne->getIntersection(intersection, targetX, targetY, radius);
				sw->getIntersection(intersection, targetX, targetY, radius);
				se->getIntersection(intersection, targetX, targetY, radius);
			}
		}
	}
	*/
	/*
	void addForce(float targetX, float targetY, float radius, float scale) {
		std::deque<Tree*> toProcess;
		toProcess.push_back(this);
		float xd, yd, length, effect;
		float sqradius = radius * radius;
		while(!toProcess.empty()) {
			Tree& curTree = *(toProcess.front());
			toProcess.pop_front();
			if(targetX > curTree.minX - radius && targetX < curTree.maxX + radius &&
				targetY > curTree.minY - radius && targetY < curTree.maxY + radius) {
				if(curTree.nParticles) {
					for(int i = 0; i < curTree.nParticles; i++) {
						Particle& curParticle = *(curTree.particles[i]);
						xd = curParticle.x - targetX;
						yd = curParticle.y - targetY;
						if(xd != 0 && yd != 0) {
							length = xd * xd + yd * yd;
							if(length < sqradius) {
								length = sqrtf(length);
								xd /= length;
								yd /= length;
								effect = 1 - (length / radius);
								effect *= scale;
								curParticle.xf += effect * xd;
								curParticle.yf += effect * yd;
							}
						}
					}
				} else if(curTree.hasChildren) {
					toProcess.push_back(curTree.nw);
					toProcess.push_back(curTree.ne);
					toProcess.push_back(curTree.sw);
					toProcess.push_back(curTree.se);
				}
			}
		}
	}*/


	void makeBox()
	{
		b2BodyDef bd;
		m_debugBody = m_world->CreateBody(&bd);
		m_debugBody->SetActive(false);
		b2ChainShape shape;
		const b2Vec2 vertices[4] = {
			b2Vec2(minX, minY),
			b2Vec2(maxX, minY),
			b2Vec2(maxX, maxY),
			b2Vec2(minX, maxY)};
		shape.CreateLoop(vertices, 4);
		m_debugBody->CreateFixture(&shape, 0.0f); 
	}

	void destroyBox()
	{
		if(m_debugBody)
		{	
			m_world->DestroyBody(m_debugBody);
			m_debugBody = NULL;
		}
	}

	void drawonlythis(b2World* m_world) {
		this->m_world = m_world;
		//printf("%f" ,(this->maxX - this->minX ) / (this->maxY - this->minY)); 
		
		destroyBox();
		makeBox();
		
	}


	void draw(b2World* m_world) {
		this->m_world = m_world;
		destroyBox();
		if(nParticles)
			makeBox();
		
		//ofRect(minX, minY, maxX - minX, maxY - minY);
		if(hasChildren) {
			nw->draw( m_world);
			ne->draw( m_world);
			sw->draw( m_world);
			se->draw( m_world);
		}
	}
};
