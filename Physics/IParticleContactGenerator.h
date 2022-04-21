#pragma once

#include "ParticleContact.h"

#include <vector>

class IParticleContactGenerator
{
public:
	virtual ~IParticleContactGenerator() = default;

	virtual size_t AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const = 0;
};