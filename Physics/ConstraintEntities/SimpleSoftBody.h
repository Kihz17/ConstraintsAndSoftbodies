#pragma once

#include "ConstraintEntity.h"
#include "ParticleCableConstraint.h"

#include <vector>

class SimpleSoftBody : public ConstraintEntity
{
public:
	SimpleSoftBody() = default; // No arg constructor for loading
	SimpleSoftBody(int width, int height, const glm::vec3& position, const glm::vec3& orientation);
	virtual ~SimpleSoftBody();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnWorldAdd() override;

	inline virtual std::vector<Particle*> GetParticles() const override { return particles; }
	virtual std::vector<ParticleConstraint*> GetConstraints() const override;

	inline virtual ConstraintEntityType GetEntityType() const override { return ConstraintEntityType::SoftBody; }

	virtual void Save(YAML::Emitter& emitter) const override;
	virtual void Load(const YAML::Node& node) override;
private:
	void Setup(int width, int height, const glm::vec3& position, const glm::vec3& orientation);

	glm::vec3 position;
	glm::vec3 orientation;
	int width, height;

	std::vector<Particle*> particles;
	std::vector<ParticleCableConstraint*> contraints;
};