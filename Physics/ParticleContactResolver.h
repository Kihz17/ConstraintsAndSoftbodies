#pragma once

#include "ParticleContact.h"

class ParticleContactResolver
{
public:
	ParticleContactResolver(size_t iterations);

	void SetIterations(size_t iterations);
	void ResolveContacts(ParticleContact* contacts, size_t contactAmount, float deltaTime);

private:
	size_t iterations; // The number of iterations we are allowed to execute
	size_t iterationsUsed; // Keeps a record of the iterations used
};