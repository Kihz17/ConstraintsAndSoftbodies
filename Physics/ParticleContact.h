#pragma once

#include "Particle.h"

// Represents one instance of contact between 2 particles
class ParticleContact
{
public:
	float CalculateSeparatingVelocity() const;
	void Resolve(float deltaTime);

	Particle* p1; // The first particle involved in this contact
	Particle* p2; // The second particle involved in this contact (can be NULL)

	glm::vec3 contactNormal; // The direction of the contact
	float restitution;
	float penetration; // How deep did we penetrate whatever we are colliding into?
	
	// The amount the particle will be moved when resolving positions
	glm::vec3 movement1;
	glm::vec3 movement2;
};