#pragma once

#include "ConstraintEntity.h"
#include "ParticleRodConstraint.h"

#include <vector>

class FerrisWheel : public ConstraintEntity
{
public:
	FerrisWheel() = default; // No arg constructor for loading
	FerrisWheel(const glm::vec3& position, const glm::vec3& force, float rodLength, float cartRadius);
	virtual ~FerrisWheel();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnWorldAdd() override;

	virtual std::vector<Particle*> GetParticles() const override;
	virtual std::vector<ParticleConstraint*> GetConstraints() const override;

	virtual void Save(YAML::Emitter& emitter) const override;
	virtual void Load(const YAML::Node& node) override;

	inline virtual ConstraintEntityType GetEntityType() const override { return ConstraintEntityType::FerrisWheel; }

private:
	void Setup(const glm::vec3& position, const glm::vec3& force, float rodLength, float cartRadius);

	Particle** particles;
	ParticleRodConstraint** constraints;

	float rodLength;
	glm::vec3 center;
	glm::vec3 force;
	float cartRadius;

	const int numOfGameObjects = 5;
	const int numOfConstraints = 10;
};