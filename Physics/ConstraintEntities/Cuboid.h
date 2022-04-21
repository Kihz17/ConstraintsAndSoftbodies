#pragma once

#include "ConstraintEntity.h"
#include "ParticleRodConstraint.h"
#include "ParticleCableConstraint.h"

#include <vector>

class Cuboid : public ConstraintEntity
{
public:
	Cuboid() = default; // No arg constructor for loading
	Cuboid(const glm::vec3& min, const glm::vec3& max, const glm::vec3& position, const glm::vec3& orientation, float sphereRadius);
	virtual ~Cuboid();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnWorldAdd() override;

	virtual std::vector<Particle*> GetParticles() const override;
	virtual std::vector<ParticleConstraint*> GetConstraints() const override;

	inline virtual ConstraintEntityType GetEntityType() const override { return ConstraintEntityType::Cuboid; }

	virtual void Save(YAML::Emitter& emitter) const override;
	virtual void Load(const YAML::Node& node) override;
private:
	void Setup(const glm::vec3& min, const glm::vec3& max, const glm::vec3& position, const glm::vec3& orientation, float sphereRadius);

	Particle** particles;
	ParticleRodConstraint** constraints;

	glm::vec3 min, max;
	glm::vec3 position;
	glm::vec3 orientation;
	float radius;

	const int numOfParticles = 8;
	const int numOfConstraints = 16;
};