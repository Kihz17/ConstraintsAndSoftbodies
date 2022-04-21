#include "World.h"

#include <iostream>

World::World(size_t maxContacts, size_t iterations)
	: gravity(0.0f, -9.81, 0.0f), contactResolver(iterations)
{
	this->calculateIterations = (iterations == 0);
	this->particleContacts = new ParticleContact[maxContacts];
	this->maxContacts = maxContacts;
}

World::~World()
{
	for (IParticleContactGenerator* generator : this->contactGenerators)
	{
		delete generator;
	}

	delete[] this->particleContacts;
}

bool World::AddParticle(Particle* particle)
{
	if (!particle)
	{
		return false;
	}

	std::vector<Particle*>::iterator itParticle = std::find(this->particles.begin(), this->particles.end(), particle);
	if (itParticle == this->particles.end())
	{
		this->particles.push_back(particle);
		return true;
	}

	return false; 
}

bool World::RemoveParticle(Particle* particle)
{
	if (!particle)
	{
		return false; 
	}

	std::vector<Particle*>::iterator itParticle = std::find(this->particles.begin(), this->particles.end(), particle);
	if (itParticle == this->particles.end())
	{
		return false; // nothing was removed.
	}
	this->particles.erase(itParticle);
	return true; // found it. removed it.
}

void World::Update(float deltaTime)
{
	UpdateParticles(deltaTime);

	size_t contactsGenerated = GenerateContacts();

	if (contactsGenerated)
	{
		if (this->calculateIterations)
		{
			this->contactResolver.SetIterations(contactsGenerated * 2);
		}

		this->contactResolver.ResolveContacts(this->particleContacts, contactsGenerated, deltaTime);
	}
}

void World::UpdateParticles(float deltaTime)
{
	std::vector<ConstraintEntity*>::iterator it = this->contraintEntities.begin();
	while (it != contraintEntities.end())
	{
		(*it)->OnUpdate(deltaTime);
		it++;
	}

	std::vector<Particle*>::iterator itParticle = this->particles.begin();
	while (itParticle != this->particles.end())
	{
		Particle* particle = *itParticle;
		particle->ApplyForce(this->gravity * particle->GetMass());
		particle->Update(deltaTime);
		itParticle++;
	}
}

void World::SetGravity(const glm::vec3 & gravity)
{
	this->gravity = gravity;
}

size_t World::GenerateContacts()
{
	size_t contactLimit = this->maxContacts;
	ParticleContact* currentContact = this->particleContacts;

	for (IParticleContactGenerator* generator : this->contactGenerators)
	{
		size_t contactsGenerated = generator->AddContact(this->particles, currentContact, contactLimit); // Creates contacts, and returns the amount of contacts generated
		contactLimit -= contactsGenerated;
		currentContact += contactsGenerated; // Move to our next available position in the contacts array

		if (contactLimit <= 0)
		{
			std::cout << "WARNING: We have created too many contacts to process!" << std::endl;
			break;
		}
	}

	return this->maxContacts - contactLimit; // Return the amount of contacts generated
}

bool World::AddContactGenerator(IParticleContactGenerator * generator)
{
	if (!generator)
	{
		return false;
	}

	std::vector<IParticleContactGenerator*>::iterator it = std::find(this->contactGenerators.begin(), this->contactGenerators.end(), generator);
	if (it == this->contactGenerators.end())
	{
		this->contactGenerators.push_back(generator);
		return true;
	}

	return false;
}

bool World::RemoveContactGenerator(IParticleContactGenerator* generator)
{
	if (!generator)
	{
		return false;
	}

	std::vector<IParticleContactGenerator*>::iterator it = std::find(this->contactGenerators.begin(), this->contactGenerators.end(), generator);
	if (it == this->contactGenerators.end())
	{
		return false;
	}

	this->contactGenerators.erase(it);
	return true;
}

bool World::AddConstraintEntity(ConstraintEntity* entity)
{
	if (!entity)
	{
		return false;
	}

	std::vector<ConstraintEntity*>::iterator itEntity = std::find(this->contraintEntities.begin(), this->contraintEntities.end(), entity);
	if (itEntity == this->contraintEntities.end())
	{
		this->contraintEntities.push_back(entity);

		// Add componenets
		for (Particle* p : entity->GetParticles()) AddParticle(p);
		for (ParticleConstraint* c : entity->GetConstraints()) AddContactGenerator(c);

		entity->OnWorldAdd();

		return true;
	}

	return false;
}

bool World::RemoveConstraintEntity(ConstraintEntity* entity)
{
	if (!entity)
	{
		return false;
	}

	std::vector<ConstraintEntity*>::iterator itEntity = std::find(this->contraintEntities.begin(), this->contraintEntities.end(), entity);
	if (itEntity == this->contraintEntities.end())
	{
		return false; // nothing was removed.
	}

	ConstraintEntity* removed = (*itEntity);
	this->contraintEntities.erase(itEntity);

	// Remove componenets
	for (Particle* p : entity->GetParticles()) RemoveParticle(p);
	for (ParticleConstraint* c : entity->GetConstraints()) RemoveContactGenerator(c);

	delete removed;

	return true; // found it. removed it.
}