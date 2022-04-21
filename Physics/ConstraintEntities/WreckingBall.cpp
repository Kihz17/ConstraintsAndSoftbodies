#include "WreckingBall.h"
#include "YAMLOverloads.h"

WreckingBall::WreckingBall(const glm::vec3& position, const glm::vec3& forceDirection, float cableLength, float ballRadius)
{
	Setup(position, forceDirection, cableLength, ballRadius);
}

WreckingBall::~WreckingBall()
{
	delete anchor;
	delete ball;
	delete cable;
}

void WreckingBall::OnWorldAdd()
{
	ball->ApplyConstantForce(forceDirection);
}

void WreckingBall::OnUpdate(float deltaTime)
{

}

std::vector<Particle*> WreckingBall::GetParticles() const
{
	std::vector<Particle*> objects;
	objects.push_back(anchor);
	objects.push_back(ball);
	return objects;
}

std::vector<ParticleConstraint*> WreckingBall::GetConstraints() const
{
	std::vector<ParticleConstraint*> constraints;
	constraints.push_back(cable);
	return constraints;
}

void WreckingBall::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "AnchorPos" << YAML::Value << anchorPos;
	emitter << YAML::Key << "ForceDirection" << YAML::Value << forceDirection;
	emitter << YAML::Key << "CableLength" << YAML::Value << cableLength;
	emitter << YAML::Key << "BallRadius" << YAML::Value << ballRadius;
	emitter << YAML::EndMap;
}

void WreckingBall::Load(const YAML::Node& node)
{
	glm::vec3 anchorPos = node["AnchorPos"].as<glm::vec3>();
	glm::vec3 direction = node["ForceDirection"].as<glm::vec3>();
	float cableLength = node["CableLength"].as<float>();
	float radius = node["BallRadius"].as<float>();

	Setup(anchorPos, direction, cableLength, radius);
}

void WreckingBall::Setup(const glm::vec3& position, const glm::vec3& forceDirection, float cableLength, float ballRadius)
{
	this->anchorPos = position;
	this->forceDirection = forceDirection;
	this->cableLength = cableLength;
	this->ballRadius = ballRadius;

	// Anchor
	this->anchor = new Particle(-2.0f, position, 1.0f);
	
	// Wrecking ball
	glm::vec3 pos = position;
	pos.y -= cableLength;
	this->ball = new Particle(ballRadius * 1.0f, pos, ballRadius);

	// Cable connection
	this->cable = new ParticleCableConstraint(ball, anchor, cableLength);
}