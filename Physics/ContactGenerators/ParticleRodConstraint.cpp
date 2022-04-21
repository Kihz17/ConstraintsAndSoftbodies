#include "ParticleRodConstraint.h"

ParticleRodConstraint::ParticleRodConstraint(Particle* p1, Particle* p2)
	: ParticleConstraint(p1, p2)
{
	length = CurrentSeparation();
}

size_t ParticleRodConstraint::AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const
{
	float currentLen = CurrentSeparation();

	if (currentLen == length)
	{
		return 0;
	}

	contactInstance->p1 = p1;
	contactInstance->p2 = p2;

	glm::vec3 normal = glm::normalize(p2->GetPosition() - p1->GetPosition());

	if (currentLen > length)
	{
		contactInstance->contactNormal = normal;
		contactInstance->penetration = currentLen - length;
	}
	else
	{
		contactInstance->contactNormal = normal * -1.0f;
		contactInstance->penetration = length - currentLen;
	}

	contactInstance->restitution = 0.0f;
	return 1;
}