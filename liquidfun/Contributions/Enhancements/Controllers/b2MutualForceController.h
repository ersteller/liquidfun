

#ifndef B2_MUTUALFORCECONTROLLER_H
#define B2_MUTUALFORCECONTROLLER_H

#include "b2Controller.h"
#include "Quadtree.h"
//#include "../../../Box2D/Box2D/Common/b2BlockAllocator.h"

class b2MutualForceController /*: public b2ControllerDef*/
{
    public: 
	void AddGroup(b2ParticleSystem* particleSystem);
	void Step(/*const b2TimeStep& step*/);
	b2MutualForceController();
	~b2MutualForceController();

	private:
	b2ParticleGroup* m_group;
	int m_stepCount;
	Tree* m_tree;
	b2ParticleSystem* m_particleSystem;
  	float m_radius;
};


/// Base class for controllers. Controllers are a convience for encapsulating common
/// per-step functionality.
/*class b2Controller
{
public:
	virtual ~b2Controller();

	/// Controllers override this to implement per-step functionality.
	virtual void Step(const b2TimeStep& step) = 0;

	/// Controllers override this to provide debug drawing.
	virtual void Draw(b2DebugDraw *debugDraw) {B2_NOT_USED(debugDraw);};

	/// Adds a body to the controller list.
	void AddBody(b2Body* body);

	/// Removes a body from the controller list.
	void RemoveBody(b2Body* body);

	/// Removes all bodies from the controller list.
	void Clear();

	/// Get the next controller in the world's body list.
	b2Controller* GetNext();
	const b2Controller* GetNext() const;

	/// Get the parent world of this body.
	b2World* GetWorld();
	const b2World* GetWorld() const;

	/// Get the attached body list
	b2ControllerEdge* GetBodyList();
	const b2ControllerEdge* GetBodyList() const;


protected:
	friend class b2World;

	b2World* m_world;

	b2ControllerEdge* m_bodyList;
	int32 m_bodyCount;

	b2Controller(const b2ControllerDef* def):
		m_world(NULL),
		m_bodyList(NULL),
		m_bodyCount(0),
		m_prev(NULL),
		m_next(NULL)
		
		{
			B2_NOT_USED(def);
		}
	virtual void Destroy(b2BlockAllocator* allocator) = 0;

private:
	b2Controller* m_prev;
	b2Controller* m_next;

	static void Destroy(b2Controller* controller, b2BlockAllocator* allocator);
};*/
/*
class b2ControllerDef
{
public:
	virtual ~b2ControllerDef() {};
	
private:
	friend class b2World;
	virtual b2Controller* Create(b2BlockAllocator* allocator) const = 0;
};

inline b2Controller* b2Controller::GetNext()
{
	return m_next;
}

inline const b2Controller* b2Controller::GetNext() const
{
	return m_next;
}

inline b2World* b2Controller::GetWorld()
{
	return m_world;
}

inline const b2World* b2Controller::GetWorld() const
{
	return m_world;
}

inline b2ControllerEdge* b2Controller::GetBodyList()
{
	return m_bodyList;
}

inline const b2ControllerEdge* b2Controller::GetBodyList() const
{
	return m_bodyList;
}

inline void b2Controller::Destroy(b2Controller* controller, b2BlockAllocator* allocator)
{
	controller->Destroy(allocator);
}*/

#endif
