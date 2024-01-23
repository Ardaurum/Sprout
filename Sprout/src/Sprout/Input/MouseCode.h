#pragma once

namespace Sprout
{
	// From SDL_mouse.h
	enum class MouseCode
	{
		Button0 = 1,
		Button1 = 2,
		Button2 = 3,
		Button3 = 4,
		Button4 = 5,
		Button5 = 6,
		Button6 = 7,
		Button7 = 8,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button2,
		ButtonMiddle = Button1
	};
}