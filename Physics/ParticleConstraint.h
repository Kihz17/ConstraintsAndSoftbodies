#pragma once

#include "pch.h"
#include "Shader.h"
#include "IParticleContactGenerator.h"

class ParticleConstraint : public IParticleContactGenerator
{
public:
	ParticleConstraint(Particle* p1, Particle* p2) : p1(p1), p2(p2) {}
	virtual ~ParticleConstraint() = default;

	float CurrentSeparation() const { return glm::distance(p1->GetPosition(), p2->GetPosition()); }

	inline Particle* GetParticleA() { return p1; }
	inline Particle* GetParticleB() { return p2; }

	virtual void Draw(Ref<Shader> shader) const
	{
		shader->Bind();
		shader->SetMat4x4("matModel", glm::mat4(1.0f));
		shader->SetMat4x4("matModelInverseTranspose", glm::inverse(glm::mat4(1.0f)));

		shader->SetFloat("isIgnoreLighting", (float)GL_TRUE);
		shader->SetFloat("isOverrideColor", (float)GL_TRUE);
		shader->SetFloat4("colorOverride", glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		glLineWidth(2.0f);
		glBegin(GL_LINES);

		glm::vec3 pos1 = p1->GetPosition();
		glm::vec3 pos2 = p2->GetPosition();
		glVertex3f(pos1.x, pos1.y, pos1.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);

		glEnd();

		shader->SetFloat("isOverrideColor", (float)GL_FALSE);
		shader->SetFloat("isIgnoreLighting", (float)GL_FALSE);
	}

protected:
	Particle* p1;
	Particle* p2;
};