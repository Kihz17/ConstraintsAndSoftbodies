#include "Cuboid.h"
#include "YAMLOverloads.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Cuboid::Cuboid(const glm::vec3& min, const glm::vec3& max, const glm::vec3& position, const glm::vec3& orientation, float sphereRadius)
{
	Setup(min, max, position, orientation, sphereRadius);
}

Cuboid::~Cuboid()
{
	for (int i = 0; i < numOfParticles; i++)
	{
		delete particles[i];
	}

	delete[] particles;

	for (int i = 0; i < numOfConstraints; i++)
	{
		delete constraints[i];
	}

	delete[] constraints;
}

void Cuboid::OnWorldAdd()
{

}


void Cuboid::OnUpdate(float deltaTime)
{
	
}

std::vector<Particle*> Cuboid::GetParticles() const
{
	std::vector<Particle*> objects;
	for (int i = 0; i < numOfParticles; i++)
	{
		objects.push_back(this->particles[i]);
	}

	return objects;
}

std::vector<ParticleConstraint*> Cuboid::GetConstraints() const
{
	std::vector<ParticleConstraint*> constraints;
	for (int i = 0; i < numOfConstraints; i++)
	{
		constraints.push_back(this->constraints[i]);
	}

	return constraints;
}

void Cuboid::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "Min" << YAML::Value << min;
	emitter << YAML::Key << "Max" << YAML::Value << max;
	emitter << YAML::Key << "Position" << YAML::Value << position;
	emitter << YAML::Key << "Orientation" << YAML::Value << orientation;
	emitter << YAML::Key << "SphereRadius" << YAML::Value << radius;
	emitter << YAML::EndMap;
}

void Cuboid::Load(const YAML::Node& node)
{
	glm::vec3 min = node["Min"].as<glm::vec3>();
	glm::vec3 max = node["Max"].as<glm::vec3>();
	glm::vec3 pos = node["Position"].as<glm::vec3>();
	glm::vec3 orientation = node["Orientation"].as<glm::vec3>();
	float radius = node["SphereRadius"].as<float>();

	Setup(min, max, pos, orientation, radius);
}

void Cuboid::Setup(const glm::vec3& min, const glm::vec3& max, const glm::vec3& position, const glm::vec3& orientation, float sphereRadius)
{
	this->min = min;
	this->max = max;
	this->position = position;
	this->orientation = orientation;
	this->radius = sphereRadius;

	glm::mat4 transform(1.0f);
	transform *= glm::translate(glm::mat4(1.0f), position);
	transform *= glm::rotate(glm::mat4(1.0f), orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	particles = new Particle* [8];
	constraints = new ParticleRodConstraint* [16];

	{
		particles[0] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(min, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(min.x, min.y, max.z);
		particles[1] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(max.x, min.y, min.z);
		particles[2] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(max.x, min.y, max.z);
		particles[3] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(min.x, max.y, min.z);
		particles[4] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(min.x, max.y, max.z);
		particles[5] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		glm::vec3 pos(max.x, max.y, min.z);
		particles[6] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(pos, 1.0f)), sphereRadius);
	}
	{
		particles[7] = new Particle(sphereRadius * 0.8f, glm::vec3(transform * glm::vec4(max, 1.0f)), sphereRadius);
	}

	for (int i = 0; i < numOfParticles; i++)
	{
		particles[i]->SetDamping(0.2f);
	}

	// Constraints

	// Bottom square
	constraints[0] = new ParticleRodConstraint(particles[0], particles[1]);
	constraints[1] = new ParticleRodConstraint(particles[1], particles[3]);
	constraints[2] = new ParticleRodConstraint(particles[0], particles[2]);
	constraints[3] = new ParticleRodConstraint(particles[2], particles[3]);

	// Bot - top connector
	constraints[4] = new ParticleRodConstraint(particles[0], particles[4]);
	constraints[5] = new ParticleRodConstraint(particles[1], particles[5]);
	constraints[6] = new ParticleRodConstraint(particles[2], particles[6]);
	constraints[7] = new ParticleRodConstraint(particles[3], particles[7]);

	// Top square
	constraints[8] = new ParticleRodConstraint(particles[4], particles[5]);
	constraints[9] = new ParticleRodConstraint(particles[4], particles[6]);
	constraints[10] = new ParticleRodConstraint(particles[5], particles[7]);
	constraints[11] = new ParticleRodConstraint(particles[7], particles[6]);

	// Criss cross
	constraints[12] = new ParticleRodConstraint(particles[0], particles[7]);
	constraints[13] = new ParticleRodConstraint(particles[2], particles[5]);
	constraints[14] = new ParticleRodConstraint(particles[1], particles[6]);
	constraints[15] = new ParticleRodConstraint(particles[3], particles[4]);
}