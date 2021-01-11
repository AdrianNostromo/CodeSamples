#include "CCD_Base.h"
#include <LinearMath/btPoolAllocator.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "../manifold/CustomPersistentManifold.h"

using namespace rpg3D;

CCD_Base::CCD_Base(btCollisionConfiguration* collisionConfiguration)
	: super(collisionConfiguration)
{
	//void
}

btPersistentManifold* CCD_Base::getNewManifold(const btCollisionObject* body0, const btCollisionObject* body1)
{
	//btAssert(gNumManifold < 65535);

	//optional relative contact breaking threshold, turned on by default (use setDispatcherFlags to switch off feature for improved performance)

	btScalar contactBreakingThreshold = (m_dispatcherFlags & btCollisionDispatcher::CD_USE_RELATIVE_CONTACT_BREAKING_THRESHOLD) ? btMin(body0->getCollisionShape()->getContactBreakingThreshold(gContactBreakingThreshold), body1->getCollisionShape()->getContactBreakingThreshold(gContactBreakingThreshold))
		: gContactBreakingThreshold;

	btScalar contactProcessingThreshold = btMin(body0->getContactProcessingThreshold(), body1->getContactProcessingThreshold());

	void* mem = m_persistentManifoldPoolAllocator->allocate(sizeof(CustomPersistentManifold));
	if (NULL == mem)
	{
		//we got a pool memory overflow, by default we fallback to dynamically allocate memory. If we require a contiguous contact pool then assert.
		if ((m_dispatcherFlags & CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION) == 0)
		{
			mem = btAlignedAlloc(sizeof(CustomPersistentManifold), 16);
		} else
		{
			btAssert(0);
			//make sure to increase the m_defaultMaxPersistentManifoldPoolSize in the btDefaultCollisionConstructionInfo/btDefaultCollisionConfiguration
			return 0;
		}
	}
	CustomPersistentManifold* manifold = new (mem) CustomPersistentManifold(
		body0, body1, 0, contactBreakingThreshold, contactProcessingThreshold,
		this
	);
	manifold->m_index1a = m_manifoldsPtr.size();
	m_manifoldsPtr.push_back(manifold);

	return manifold;
}

void CCD_Base::releaseManifold(btPersistentManifold* manifold)
{
	//printf("releaseManifold: gNumManifold %d\n",gNumManifold);
	clearManifold(manifold);

	int findIndex = manifold->m_index1a;
	btAssert(findIndex < m_manifoldsPtr.size());
	m_manifoldsPtr.swap(findIndex, m_manifoldsPtr.size() - 1);
	m_manifoldsPtr[findIndex]->m_index1a = findIndex;
	m_manifoldsPtr.pop_back();

	CustomPersistentManifold* cManifold = static_cast<CustomPersistentManifold*>(manifold);

	cManifold->~CustomPersistentManifold();
	if (m_persistentManifoldPoolAllocator->validPtr(cManifold))
	{
		m_persistentManifoldPoolAllocator->freeMemory(cManifold);
	} else
	{
		btAlignedFree(cManifold);
	}
}

CCD_Base::~CCD_Base() {
	//void
}
