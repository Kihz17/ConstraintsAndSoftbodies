#pragma once

#include "Particle.h"
#include "ParticleContact.h"
#include "IParticleContactGenerator.h"
#include "ParticleContactResolver.h"
#include "ConstraintEntity.h"

#include <vector>

class World
{
public:
	World(size_t maxContacts, size_t iterations = 0);
	virtual ~World();

	bool AddParticle(Particle* particle);
	bool RemoveParticle(Particle* particle);

	bool AddConstraintEntity(ConstraintEntity* entity);
	bool RemoveConstraintEntity(ConstraintEntity* entity);

	void Update(float deltaTime);

	void SetGravity(const glm::vec3& gravity);

	bool AddContactGenerator(IParticleContactGenerator* generator);
	bool RemoveContactGenerator(IParticleContactGenerator* generator);

	const std::vector<Particle*>& GetParticles() const { return particles; }
	const std::vector<ConstraintEntity*>& GetConstraintEntities() const { return contraintEntities; }
private:
	void UpdateParticles(float deltaTime);

	size_t GenerateContacts();

	std::vector<Particle*> particles;
	std::vector<ConstraintEntity*> contraintEntities;

	ParticleContact* particleContacts; // An array of contacts (length = maxContacts)
	size_t maxContacts;

	std::vector<IParticleContactGenerator*> contactGenerators; // A list of the actual things creation contacts (AKA: The logic behind the collisions)

	ParticleContactResolver contactResolver;
	bool calculateIterations;

	glm::vec3 gravity;
};