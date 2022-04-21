#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver(size_t iterations)
	: iterations(iterations), iterationsUsed(0)
{

}

void ParticleContactResolver::SetIterations(size_t iterations)
{
	this->iterations = iterations;
}

void ParticleContactResolver::ResolveContacts(ParticleContact* contacts, size_t contactAmount, float deltaTime)
{
	this->iterationsUsed = 0;
	while (this->iterationsUsed < this->iterations)
	{
		// Finds the contact with the largest closing velocity
		float fMax = std::numeric_limits<float>::max();
		unsigned int maxIndex = contactAmount;
		for (int i = 0; i < contactAmount; i++)
		{
			float seperatingVelocity = contacts[i].CalculateSeparatingVelocity();
			if (seperatingVelocity < fMax && (seperatingVelocity < 0 || contacts[i].penetration > 0))
			{
				fMax = seperatingVelocity;
				maxIndex = i;
			}
		}

		if (maxIndex == contactAmount) // No contacts to resolve
		{
			break; 
		}

		contacts[maxIndex].Resolve(deltaTime);

		ParticleContact* contact = &contacts[maxIndex];
		for (int i = 0; i < contactAmount; i++)
		{
			if (contact[i].p1 == contact->p1) // We are the first particle, decrement the penetration based of the movement in the normal direction
			{
				contact[i].penetration -= glm::dot(contact->movement1, contact[i].contactNormal);
			}
			else if (contacts[i].p1 == contact->p2) // We are the second particle, decrement the penetration based of the movement in the normal direction
			{
				contacts[i].penetration -= glm::dot(contact->movement2, contacts[i].contactNormal);
			}

			if (contacts[i].p2)
			{
				if (contacts[i].p2 == contact->p1)
				{
					contacts[i].penetration += glm::dot(contact->movement1, contacts[i].contactNormal);
				}
				else if (contacts[i].p2 == contact->p2)
				{
					contacts[i].penetration += glm::dot(contact->movement2, contacts[i].contactNormal);
				}
			}
		}

		this->iterationsUsed++;
	}
}