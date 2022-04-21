#pragma once

#include "pch.h"
#include "Camera.h"
#include "Particle.h"

#include <glm/glm.hpp>

class PlayerController : public Particle
{
public:
	PlayerController(Ref<Camera> camera, const glm::vec3& position);
	virtual ~PlayerController() = default;

	void OnUpdate(float deltaTime);
	void ToggleHandleInput(bool state);

	inline glm::vec3& GetLastNormalizedVelocity() { return lastNormalizedVelocity; }

	inline virtual void ResetJump() { jumpCount = 0; }

private:
	friend class Client;

	Ref<Camera> camera;
	bool handleInput;
	glm::vec3 lastNormalizedVelocity;

	float lastSpacePress;
	int jumpCount;
};