#pragma once

namespace Sprout
{
	class Timestep
	{
	public:
		Timestep(float time = .0f) 
			: Time(time)
		{
		}

		operator float() const { return Time; }

		float GetSeconds() const { return Time; }
		float GetMilliseconds() const { return Time * 1000.0f; }

	private:
		float Time;
	};
}