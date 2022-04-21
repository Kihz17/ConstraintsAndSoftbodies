#pragma once

#include "ParticleConstraint.h"

class ParticleCableConstraint : public ParticleConstraint
{
public:
	ParticleCableConstraint(Particle* p1, Particle* p2, float length);

	virtual size_t AddContact(std::vector<Particle*>& paricles, ParticleContact* contactInstance, size_t contactLimit) const override;

protected:
	float length;
};