#pragma once

#include <glm/glm.hpp>

namespace Sprout
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(glm::mat4 projectionMatrix) 
			: ProjectionMatrix(projectionMatrix), 
			ViewProjectionMatrix(glm::mat4(projectionMatrix))
		{}

		void SetProjection(glm::mat4 projectionMatrix) 
		{ 
			ProjectionMatrix = projectionMatrix; 
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix; 
		}

		void SetView(glm::mat4 viewMatrix) 
		{
			ViewMatrix = viewMatrix; 
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix; 
		}

		const glm::mat4& GetViewMatrix() const { return ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

	private:
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewProjectionMatrix;
	};
}