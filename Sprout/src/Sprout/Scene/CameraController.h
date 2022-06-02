#pragma once

#include "Sprout\Renderer\Camera.h"

#include "Sprout\Core\Timestep.h"

#include "Sprout\Input\KeyCode.h"

#include "Sprout\CoreEvents\KeyEvent.h"
#include "Sprout\CoreEvents\MouseEvent.h"
#include "Sprout\CoreEvents\WinEvent.h"

#include "Sprout\Math\Enumerations.h"

namespace Sprout
{
	enum class CameraType : uint8_t
	{
		Perspective,
		Orthographic
	};

	class CameraController
	{
	public:
		CameraController(CameraType type, float aspectRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& ev);

		void LockPosition(AxisOptions options) { LockedAxis = options; }
		void LockRotation(RotationOptions options) { LockedRotation = options; }

		void SetPosition(glm::vec3 pos) { Position = pos; UpdateView(); }
		void SetRotation(glm::vec3 rot) { Rotation = rot; UpdateView(); }

		void SetTranslationSpeed(float speed) { TranslationSpeed = speed; }
		void SetRotationSpeed(float speed) { RotationSpeed = speed; }

		glm::vec3 GetPosition() const { return Position; }
		glm::vec3 GetRotation() const { return Rotation; }

		const Camera& GetCamera() const { return Cam; }

	private:
		bool OnMouseScrolled(MouseScrollEvent& ev);
		bool OnWindowResized(WinResizeEvent& ev);

		void UpdateView();
		void UpdateProjection();

	private:
		Camera Cam;

		CameraType CamType;

		float AspectRatio;
		float ZoomLevel = 1.0f;

		glm::vec2 LastMousePos = {.0f, .0f};

		AxisOptions LockedAxis = AxisOptions::None;
		RotationOptions LockedRotation = RotationOptions::None;

		glm::vec3 Position = {.0f, .0f, .0f};
		glm::vec3 Rotation = {.0f, .0f, .0f};

		float TranslationSpeed = 5.0f;
		float RotationSpeed = 0.1f;
	};
}