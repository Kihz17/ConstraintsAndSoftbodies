#include "PlayerController.h"
#include "Input.h"
#include "GLCommon.h"

#include <iostream>

static const float cameraYOffset = 5.0f;
static const float MoveSpeed = 1.0f;
static const float JumpForce = 50.0f;
static const glm::vec3 playerGravity(0.0f, -100.0f, 0.0f);

PlayerController::PlayerController(Ref<Camera> camera, const glm::vec3& position)
	: Particle(1.0f, position, 2.0f),
	camera(camera),
	handleInput(true),
	lastSpacePress(10000.0f),
	jumpCount(0)
{
	Input::SetCursorMode(CursorMode::Locked);
	SetDamping(0.3f);
	SetBounciness(0.01f);
}

void PlayerController::OnUpdate(float deltaTime)
{
	lastSpacePress += deltaTime;

	if (handleInput)
	{
		camera->MoveCamera(Input::GetMouseX(), Input::GetMouseY());

		glm::vec3 movementDirection = camera->direction;
		movementDirection.y = 0.0f;
		movementDirection = glm::normalize(movementDirection);

		glm::vec3 force(0.0f, 0.0f, 0.0f);

		glm::vec3 newPosition = GetPosition();
		if (Input::IsKeyPressed(Key::W))
		{
			force += movementDirection * MoveSpeed;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			force += movementDirection * -MoveSpeed;
		}

		if (Input::IsKeyPressed(Key::A))
		{
			constexpr float theta = glm::radians(90.0f);
			glm::vec3 left(movementDirection.x * cos(theta) + movementDirection.z * sin(theta), 0.0f, -movementDirection.x * sin(theta) + movementDirection.z * cos(theta));
			force += left * MoveSpeed;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			constexpr float theta = glm::radians(-90.0f);
			glm::vec3 right(movementDirection.x * cos(theta) + movementDirection.z * sin(theta), 0.0f, -movementDirection.x * sin(theta) + movementDirection.z * cos(theta));
			force += right * MoveSpeed;
		}

		bool spacePressed = Input::IsKeyPressed(Key::Space);

		if (Input::IsKeyPressed(Key::Space) && lastSpacePress >= 0.3f)
		{
			lastSpacePress = 0.0f;
			force.y += JumpForce;
		}

		if (Input::IsKeyPressed(Key::Equal))
		{
			SetBounciness(GetBounciness() + 0.01f);
		}
		else if (Input::IsKeyPressed(Key::Minus))
		{
			SetBounciness(std::max(0.0f, GetBounciness() - 0.01f));
		}

		ApplyConstantForce(force); // Apply force from player controls
	}

	ApplyForce(playerGravity); // Apply force from player controls
}

void PlayerController::ToggleHandleInput(bool state)
{
	handleInput = state;
	if (handleInput)
	{
		Input::SetCursorMode(CursorMode::Locked);
	}
	else
	{
		Input::SetCursorMode(CursorMode::Normal);
	}
}