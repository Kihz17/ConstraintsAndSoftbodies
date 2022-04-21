#include "ParticleContact.h"

#include <glm/glm.hpp>

float ParticleContact::CalculateSeparatingVelocity() const
{
	glm::vec3 velocity = this->p1->GetVelocity();
	if (this->p2)
	{
		velocity -= this->p2->GetVelocity();
	}

	return glm::dot(velocity, this->contactNormal);
}

void ParticleContact::Resolve(float deltaTime)
{
	// Resolve velocity
	{
		float separatingVelocity = CalculateSeparatingVelocity();
		if (separatingVelocity > 0.0f) // Already seperated or the object is stationary, no need to resolve
		{
			return;
		}

		float newSeparatingVelocity = -separatingVelocity * this->restitution;
		glm::vec3 accelerationVel = this->p1->GetAcceleration();
		if (this->p2)
		{
			accelerationVel -= this->p2->GetAcceleration();
		}

		float accelerationSeperatingVelocity = glm::dot(accelerationVel, this->contactNormal) * deltaTime; // Make our acceleration point in the direction of our normal
		if (accelerationSeperatingVelocity < 0.0f) // Closing velocity due to acceleration build up
		{
			newSeparatingVelocity += this->restitution * accelerationSeperatingVelocity; // Remove the acceleration "build up" from the new separating velocity
			if (newSeparatingVelocity < 0.0f) // Ensure that we don't remove too much
			{
				newSeparatingVelocity = 0.0f;
			}
		}

		float deltaVelocity = newSeparatingVelocity - separatingVelocity;

		// Calculate inverse mass so that we can apply velocities in proportion to their mass. (Higher mass = less velocity)
		float inverseMass = this->p1->GetInverseMass();
		if (this->p2)
		{
			inverseMass += this->p2->GetInverseMass();
		}

		if (inverseMass <= 0.0f) // Infinite mass
		{
			return;
		}

		float impulse = deltaVelocity / inverseMass; // Calculate the impulse to apply
		glm::vec3 impulsePerMass = this->contactNormal * impulse; // Find impulse per unit

		// Apply impulses in the direction of their collision
		this->p1->SetVelocity(p1->GetVelocity() + impulsePerMass * this->p1->GetInverseMass());
		if (this->p2)
		{
			this->p2->SetVelocity(this->p2->GetVelocity() + impulsePerMass * -this->p2->GetInverseMass());
		}
	}
	
	// Resolve position
	{
		if (this->penetration <= 0) // No penetration, nothing to resolve
		{
			return;
		}

		float inverseMass = this->p1->GetInverseMass();
		if (this->p2)
		{
			inverseMass += this->p2->GetInverseMass();
		}

		if (inverseMass <= 0.0f) // Infinite mass, do nothing
		{
			this->movement1.x = 0.0f;
			this->movement1.y = 0.0f;
			this->movement1.z = 0.0f;

			this->movement2.x = 0.0f;
			this->movement2.y = 0.0f;
			this->movement2.z = 0.0f;
			return;
		}

		glm::vec3 movePerMass = this->contactNormal * (this->penetration / inverseMass); // Find the the amount to move by per unit depending on the penetration

		this->movement1 = movePerMass * this->p1->GetInverseMass();
		if (this->p2)
		{
			this->movement2 = movePerMass * -this->p2->GetInverseMass();
		}
		else
		{
			this->movement2.x = 0.0f;
			this->movement2.y = 0.0f;
			this->movement2.z = 0.0f;
		}

		this->p1->SetPosition(this->p1->GetPosition() + this->movement1);
		if (this->p2)
		{
			this->p2->SetPosition(this->p2->GetPosition() + this->movement2);
		}
	}
}