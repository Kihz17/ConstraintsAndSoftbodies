#include "FerrisWheel.h"
#include "YAMLOverloads.h"

FerrisWheel::FerrisWheel(const glm::vec3& position, const glm::vec3& force, float rodLength, float cartRadius)
{
	Setup(position, force, rodLength, cartRadius);
}

FerrisWheel::~FerrisWheel()
{
	for (int i = 0; i < numOfGameObjects; i++)
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

void FerrisWheel::OnWorldAdd()
{
	particles[1]->ApplyConstantForce(force);
}


void FerrisWheel::OnUpdate(float deltaTime)
{
	//particles[1]->ApplyForce(glm::vec3(1.0f, 0.0f, 0.0f));
}

std::vector<Particle*> FerrisWheel::GetParticles() const
{
	std::vector<Particle*> objects;
	for (int i = 0; i < numOfGameObjects; i++)
	{
		objects.push_back(this->particles[i]);
	}

	return objects;
}

std::vector<ParticleConstraint*> FerrisWheel::GetConstraints() const
{
	std::vector<ParticleConstraint*> constraints;
	for (int i = 0; i < numOfConstraints; i++)
	{
		constraints.push_back(this->constraints[i]);
	}

	return constraints;
}

void FerrisWheel::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "CenterPos" << YAML::Value << center;
	emitter << YAML::Key << "ForceDirection" << YAML::Value << force;
	emitter << YAML::Key << "RodLength" << YAML::Value << rodLength;
	emitter << YAML::Key << "CartRadius" << YAML::Value << cartRadius;
	emitter << YAML::EndMap;
}

void FerrisWheel::Load(const YAML::Node& node)
{
	glm::vec3 center = node["CenterPos"].as<glm::vec3>();
	glm::vec3 force = node["ForceDirection"].as<glm::vec3>();
	float rodLength = node["RodLength"].as<float>();
	float radius = node["CartRadius"].as<float>();

	Setup(center, force, rodLength, radius);
}

void FerrisWheel::Setup(const glm::vec3& position, const glm::vec3& force, float rodLength, float cartRadius)
{
	this->center = position;
	this->force = force;
	this->rodLength = rodLength;
	this->cartRadius = cartRadius;

	this->particles = new Particle* [5];
	this->constraints = new ParticleRodConstraint* [10];

	// Center piece
	{
		particles[0] = new Particle(-1.0f, position, 1.0f);
	}

	{
		glm::vec3 pos = position;
		pos.x -= rodLength;
		particles[1] = new Particle(0, pos, cartRadius);
	}

	{
		glm::vec3 pos = position;
		pos.y += rodLength;
		particles[2] = new Particle(0, pos, cartRadius);
	}

	{
		glm::vec3 pos = position;
		pos.x += rodLength;
		particles[3] = new Particle(0, pos, cartRadius);
	}

	{
		glm::vec3 pos = position;
		pos.y -= rodLength;
		particles[4] = new Particle(0, pos, cartRadius);

	}

	// Constraints
	{
		constraints[0] = new ParticleRodConstraint(particles[0], particles[1]); // Center - pos x
		constraints[1] = new ParticleRodConstraint(particles[0], particles[2]); // Center - pos y
		constraints[2] = new ParticleRodConstraint(particles[0], particles[3]); // Center - neg x
		constraints[3] = new ParticleRodConstraint(particles[0], particles[4]); // Center - neg y

		constraints[4] = new ParticleRodConstraint(particles[1], particles[2]); // pos x - pos y
		constraints[5] = new ParticleRodConstraint(particles[2], particles[3]); // pos y - neg x
		constraints[6] = new ParticleRodConstraint(particles[3], particles[4]); // negx - neg y
		constraints[7] = new ParticleRodConstraint(particles[4], particles[1]); // neg y - pos x

		constraints[8] = new ParticleRodConstraint(particles[1], particles[3]); // pos x - neg x
		constraints[9] = new ParticleRodConstraint(particles[2], particles[4]); // pos y - neg y
	}
}