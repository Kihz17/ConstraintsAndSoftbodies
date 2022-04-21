#include "Particle.h"

Particle::Particle(float mass, const glm::vec3& position, float radius)
	: position(position)
	, velocity(0.f)
	, acceleration(0.f)
	, damping(0.995f)
	, appliedForce(0.f)
	, valid(true)
	, radius(radius)
	, bounciness(1.0f)
	, constantAppliedForce(0.0f)
{
	SetMass(mass);
}
Particle::~Particle()
{

}

glm::vec3 Particle::GetPosition() const
{
	return this->position;
}

void Particle::GetPosition(glm::vec3& position)
{
	position = this->position;
}

void Particle::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

glm::vec3 Particle::GetVelocity() const
{
	return this->velocity;
}

void Particle::GetVelocity(glm::vec3& velocity)
{
	velocity = this->velocity;
}

void Particle::SetVelocity(const glm::vec3& velocity)
{
	this->velocity = velocity;
}

glm::vec3 Particle::GetAcceleration() const
{
	return this->acceleration;
}

void Particle::GetAcceleration(glm::vec3& acceleration)
{
	acceleration = this->acceleration;
}

void Particle::SetAcceleration(const glm::vec3& acceleration)
{
	this->acceleration = acceleration;
}

void Particle::ApplyForce(const glm::vec3& force)
{
	this->appliedForce += force;
}

void Particle::ApplyImpulse(const glm::vec3& impulse)
{
	this->velocity += impulse * this->inverseMass;
}

void Particle::ClearAppliedForces()
{
	this->appliedForce.x = 0.0f;
	this->appliedForce.y = 0.0f;
	this->appliedForce.z = 0.0f;

	this->constantAppliedForce.x = 0.0f;
	this->constantAppliedForce.y = 0.0f;
	this->constantAppliedForce.z = 0.0f;
}

void Particle::Update(float deltaTime)
{
	if (this->inverseMass == 0.f)
	{
		return;
	}

	this->velocity += constantAppliedForce;
	this->position += this->velocity * deltaTime;
	this->velocity += (this->acceleration + this->appliedForce * this->inverseMass) * deltaTime;
	this->velocity *= glm::pow(this->damping, deltaTime);

	ClearAppliedForces();
}

void Particle::SetMass(float mass)
{
	if (mass <= 0.f)
	{
		this->inverseMass = 0.f;
	}
	else
	{
		this->inverseMass = 1.f / mass;
	}
}

float Particle::GetMass() const
{
	if (this->inverseMass == 0.f)
	{
		return this->inverseMass;
	}
	return 1.0f / this->inverseMass;
}

float Particle::GetInverseMass() const
{
	return this->inverseMass;
}

void Particle::SetDamping(float damping)
{
	this->damping = glm::clamp(damping, 0.f, 1.f);
}

float Particle::GetDamping() const
{
	return this->damping;
}

void Particle::SetIsValid(bool valid)
{
	this->valid = valid;
}

bool Particle::IsValid() const
{
	return this->valid;
}

void Particle::SetRadius(float radius)
{
	this->radius = radius;
}

float Particle::GetRadius() const
{
	return this->radius;
}