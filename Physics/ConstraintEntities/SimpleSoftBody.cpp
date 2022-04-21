#include "SimpleSoftBody.h"
#include "YAMLOverloads.h"

#include <glm/gtc/matrix_transform.hpp>

static const float sphereRadius = 1.0f;
static const float sphereOffset = 3.0f;

SimpleSoftBody::SimpleSoftBody(int width, int height, const glm::vec3& position, const glm::vec3& orientation)
{
	Setup(width, height, position, orientation);
}

SimpleSoftBody::~SimpleSoftBody()
{
	for (Particle* p : particles)
	{
		delete p;
	}

	for (ParticleCableConstraint* c : contraints)
	{
		delete c;
	}
}

void SimpleSoftBody::OnUpdate(float deltaTime)
{

}

void SimpleSoftBody::OnWorldAdd()
{

}

std::vector<ParticleConstraint*> SimpleSoftBody::GetConstraints() const
{
	std::vector<ParticleConstraint*> vec;
	for (ParticleConstraint* c : contraints)
	{
		vec.push_back(c);
	}

	return vec;
}

void SimpleSoftBody::SimpleSoftBody::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "Width" << YAML::Value << width;
	emitter << YAML::Key << "Height" << YAML::Value << height;
	emitter << YAML::Key << "Position" << YAML::Value << position;
	emitter << YAML::Key << "Orientation" << YAML::Value << orientation;
	emitter << YAML::EndMap;
}

void SimpleSoftBody::SimpleSoftBody::Load(const YAML::Node& node)
{
	int width = node["Width"].as<float>();
	int height = node["Height"].as<float>();
	glm::vec3 pos = node["Position"].as<glm::vec3>();
	glm::vec3 orientation = node["Orientation"].as<glm::vec3>();
	Setup(width, height, pos, orientation);
}

void SimpleSoftBody::SimpleSoftBody::Setup(int width, int height, const glm::vec3& position, const glm::vec3& orientation)
{
	this->width = width;
	this->height = height;
	this->position = position;
	this->orientation = orientation;

	glm::mat4 transform(1.0f);
	transform *= glm::translate(glm::mat4(1.0f), position);
	transform *= glm::rotate(glm::mat4(1.0f), orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 direction = glm::normalize(orientation);
	particles.resize(width * height);
	contraints.resize(((width - 1) * height) + ((height - 1) * width));

	// Particles are constructed like:
	// O		O		O		O O			O O
	//    ->	O	->	O	->	O		->	O O		-> etc...
	//					O		O			O

	int particleIndex = 0;
	for (int w = 0; w < width; w++)
	{
		glm::vec3 pos = position + direction * sphereOffset * (w + 1.0f);
		for (int h = 0; h < height; h++)
		{
			Particle* p = new Particle((h == 0 ? 0.0f : 1.0f), pos, 1.0f); // Top row of particles must have no mass so that it will stay in place;
			p->SetBounciness(0.0f);
			particles[particleIndex] = p;
			pos.y -= sphereOffset;
			particleIndex++;
		}
	}

	// Setup constraints
	int constraintIndex = 0;
	for (int i = 0; i < particles.size(); i++)
	{
		int particleBelowIndex = i + 1;
		int row = i / height;
		if (particleBelowIndex < (height * (row + 1)))
		{
			contraints[constraintIndex++] = new ParticleCableConstraint(particles[i], particles[particleBelowIndex], sphereOffset);
		}

		int particleToRightIndex = i + height;
		if (particleToRightIndex < particles.size())
		{
			contraints[constraintIndex++] = new ParticleCableConstraint(particles[i], particles[particleToRightIndex], sphereOffset);
		}
	}

	std::cout << std::endl;
}