#pragma once

#include "Particle.h"
#include "ParticleConstraint.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

enum class ConstraintEntityType
{
	None = 0,
	WreckingBall,
	FerrisWheel,
	Cuboid,
	SoftBody
};

class ConstraintEntity
{
public:
	ConstraintEntity() = default;

	virtual void OnUpdate(float deltaTime) = 0;

	virtual std::vector<Particle*> GetParticles() const = 0;
	virtual std::vector<ParticleConstraint*> GetConstraints() const = 0;

	virtual void OnWorldAdd() = 0;

	virtual void Save(YAML::Emitter& emitter) const = 0;
	virtual void Load(const YAML::Node& node) = 0;
	virtual ConstraintEntityType GetEntityType() const = 0;

	static ConstraintEntityType StringToEntityType(const std::string& value)
	{
		if (value == "WreckingBall")		return ConstraintEntityType::WreckingBall;
		else if (value == "FerrisWheel")		return ConstraintEntityType::FerrisWheel;
		else if (value == "Cuboid")		return ConstraintEntityType::Cuboid;
		else if (value == "SoftBody")		return ConstraintEntityType::SoftBody;

		std::cout << "Could not convert entity type " << value << " to entity type.\n";
		return ConstraintEntityType::None;
	}

	static std::string EntityTypeToString(ConstraintEntityType type)
	{
		switch (type)
		{
		case ConstraintEntityType::WreckingBall:	return "WreckingBall";
		case ConstraintEntityType::FerrisWheel:		return "FerrisWheel";
		case ConstraintEntityType::Cuboid:			return "Cuboid";
		case ConstraintEntityType::SoftBody:		return "SoftBody";
		default:
			std::cout << "Could not convert entity type "<< (int)type << " to string.\n";
			return "";
		}
	}


};