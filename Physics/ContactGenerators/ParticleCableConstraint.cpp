#include "ParticleCableConstraint.h"

ParticleCableConstraint::ParticleCableConstraint(Particle* p1, Particle* p2, float length)
	: ParticleConstraint(p1, p2), length(length)
{

}

size_t ParticleCableConstraint::AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const
{
	float currentLen = CurrentSeparation();

	if (currentLen <= length)
	{
		return 0;
	}

	contactInstance->p1 = p1;
	contactInstance->p2 = p2;

	glm::vec3 normal = glm::normalize(glm::vec3(p2->GetPosition()) - glm::vec3(p1->GetPosition()));

	contactInstance->contactNormal = normal;
	contactInstance->penetration = currentLen - length;
	contactInstance->restitution = 0.0f;

	return 1;
}