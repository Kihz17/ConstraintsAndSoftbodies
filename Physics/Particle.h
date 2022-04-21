#pragma once

#include <glm/glm.hpp>

class Particle
{
public:
	Particle(float mass, const glm::vec3& position, float radius);
	virtual ~Particle();

	Particle() = delete;
	Particle(Particle& other) = delete;
	Particle& operator=(Particle& other) = delete;

	glm::vec3 GetPosition() const;
	void GetPosition(glm::vec3& position);
	void SetPosition(const glm::vec3& position);

	glm::vec3 GetVelocity() const;
	void GetVelocity(glm::vec3& velocity);
	void SetVelocity(const glm::vec3& velocity);

	glm::vec3 GetAcceleration() const;
	void GetAcceleration(glm::vec3& acceleration);
	void SetAcceleration(const glm::vec3& acceleration);

	void SetMass(float mass);
	float GetMass() const;
	float GetInverseMass() const;

	void SetDamping(float damping);
	float GetDamping() const;

	void ApplyForce(const glm::vec3& force);
	inline void ApplyConstantForce(const glm::vec3& force) { constantAppliedForce = force; }
	void ApplyImpulse(const glm::vec3& impulse);

	void ClearAppliedForces();

	virtual void Update(float deltaTime);

	void SetIsValid(bool valid);
	bool IsValid() const;

	void SetRadius(float radius);
	float GetRadius() const;

	inline void SetBounciness(float bounciness) { this->bounciness = bounciness; }
	inline float GetBounciness() const { return bounciness; }
private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 appliedForce;
	glm::vec3 constantAppliedForce;

	float inverseMass;
	float damping;
	bool valid;
	float radius;
	float bounciness;
};