#include "spch.h"
#include "CameraController.h"

#include "Sprout\Input\Input.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Sprout
{
	CameraController::CameraController(CameraType type, float aspectRatio)
		: CamType(type), AspectRatio(aspectRatio)
	{
		switch (type)
		{
		case CameraType::Orthographic:
			Cam = Camera(glm::ortho(-aspectRatio * ZoomLevel, aspectRatio * ZoomLevel, ZoomLevel, ZoomLevel));
			break;
		case CameraType::Perspective:
			Cam = Camera(glm::perspective(glm::radians(45.0f) * ZoomLevel, aspectRatio, .1f, 100.0f));
			break;
		}
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		float dt = ts;

		if (Input::IsMouseButtonPressed(Sprout::MouseCode::ButtonRight))
		{
			glm::vec2 newMousePos = Input::GetMousePos();
			glm::vec2 deltaMouse = newMousePos - LastMousePos;

			glm::vec3 camEuler = Rotation;
			if (Any(LockedRotation & RotationOptions::Pitch) == false)
			{
				camEuler.x += glm::radians(deltaMouse.y) * RotationSpeed;
			}
			if (Any(LockedRotation & RotationOptions::Yaw) == false)
			{
				camEuler.y += glm::radians(deltaMouse.x) * RotationSpeed;
			}
			camEuler.z = .0f;

			Rotation = camEuler;
		}
		LastMousePos = Input::GetMousePos();

		glm::vec3 moveVec(0);
		if (Any(LockedAxis & AxisOptions::Forward) == false)
		{
			if (Input::IsKeyPressed(Sprout::KeyCode::W) || Sprout::Input::IsKeyPressed(Sprout::KeyCode::Up))
			{
				moveVec.z = -1.0f;
			}
			else if (Input::IsKeyPressed(Sprout::KeyCode::S) || Sprout::Input::IsKeyPressed(Sprout::KeyCode::Down))
			{
				moveVec.z = 1.0f;
			}
		}

		if (Any(LockedAxis & AxisOptions::Right) == false)
		{
			if (Input::IsKeyPressed(Sprout::KeyCode::D) || Sprout::Input::IsKeyPressed(Sprout::KeyCode::Right))
			{
				moveVec.x = 1.0f;
			}
			else if (Input::IsKeyPressed(Sprout::KeyCode::A) || Sprout::Input::IsKeyPressed(Sprout::KeyCode::Left))
			{
				moveVec.x = -1.0f;
			}
		}

		if (Any(LockedAxis & AxisOptions::Up) == false)
		{
			if (Input::IsKeyPressed(Sprout::KeyCode::Q))
			{
				moveVec.y = -1.0f;
			}
			else if (Input::IsKeyPressed(Sprout::KeyCode::E))
			{
				moveVec.y = 1.0f;
			}
		}

		moveVec = glm::vec4(moveVec, 0) * glm::eulerAngleXYZ(Rotation.x, Rotation.y, Rotation.z);
		Position += moveVec * dt * TranslationSpeed;

		UpdateView();
	}

	void CameraController::OnEvent(Event& ev)
	{
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<MouseScrollEvent>([this](MouseScrollEvent& ev) { return OnMouseScrolled(ev); });
		dispatcher.Dispatch<WinResizeEvent>([this](WinResizeEvent& ev) { return OnWindowResized(ev); });
	}

	bool CameraController::OnMouseScrolled(MouseScrollEvent& ev)
	{
		ZoomLevel -= ev.GetScrollY();
		UpdateProjection();
		return false;
	}

	bool CameraController::OnWindowResized(WinResizeEvent& ev)
	{
		AspectRatio = (float) ev.GetWidth() / ev.GetHeight();
		UpdateProjection();
		return false;
	}

	void CameraController::UpdateView()
	{
		glm::mat4 result(1);
		result = glm::eulerAngleZXY(Rotation.z, Rotation.x, Rotation.y) * glm::translate(result, -Position);
		Cam.SetView(result);
	}

	void CameraController::UpdateProjection()
	{
		switch (CamType)
		{
		case CameraType::Orthographic:
			Cam.SetProjection(glm::ortho(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, ZoomLevel, ZoomLevel));
			break;
		case CameraType::Perspective:
			Cam.SetProjection(glm::perspective(glm::radians(45.0f) * ZoomLevel, AspectRatio, .1f, 100.0f));
			break;
		}
	}
}
