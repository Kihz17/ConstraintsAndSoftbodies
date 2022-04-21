#pragma once

#include "ParticleConstraint.h"

class ParticleRodConstraint : public ParticleConstraint
{
public:
	ParticleRodConstraint(Particle* p1, Particle* p2);

	virtual size_t AddContact(std::vector<Particle*>& paricles, ParticleContact* contactInstance, size_t contactLimit) const override;

protected:
	float length;
};