#pragma once

#include "IParticleContactGenerator.h"

class ParticleParticleContactGenerator : public IParticleContactGenerator
{
public:
	ParticleParticleContactGenerator() = default;
	~ParticleParticleContactGenerator() = default;

	virtual size_t AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const override;
};