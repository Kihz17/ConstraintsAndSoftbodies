#pragma once

#include "ConstraintEntity.h"
#include "ParticleCableConstraint.h"

#include <vector>

class WreckingBall : public ConstraintEntity
{
public:
	WreckingBall() = default; // No arg constructor for loading
	WreckingBall(const glm::vec3& position, const glm::vec3& forceDirection, float cableLength, float ballRadius);
	virtual ~WreckingBall();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnWorldAdd() override;

	virtual std::vector<Particle*> GetParticles() const override;
	virtual std::vector<ParticleConstraint*> GetConstraints() const override;

	inline virtual ConstraintEntityType GetEntityType() const override { return ConstraintEntityType::WreckingBall; }

	virtual void Save(YAML::Emitter& emitter) const override;
	virtual void Load(const YAML::Node& node) override;
private:
	void Setup(const glm::vec3& position, const glm::vec3& forceDirection, float cableLength, float ballRadius);

	Particle* anchor;
	Particle* ball;
	ParticleCableConstraint* cable;

	glm::vec3 anchorPos;
	glm::vec3 forceDirection;
	float cableLength;
	float ballRadius;

	float lastImpulseTime;
};