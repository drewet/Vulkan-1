/**
 * VKTS Examples - Examples for Vulkan using VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Example.hpp"

Example::Example(const int32_t displayIndex, const int32_t windowIndex) :
		IUpdateThread(), displayIndex(displayIndex), windowIndex(windowIndex)
{
}

Example::~Example()
{
}

//
// Vulkan initialization.
//
VkBool32 Example::init(const vkts::IUpdateThreadContext& updateContext)
{
	if (!updateContext.isDisplayAttached(displayIndex))
	{
		return VK_FALSE;
	}

	if (!updateContext.isWindowAttached(windowIndex))
	{
		return VK_FALSE;
	}

	return VK_TRUE;
}

//
// Vulkan update.
//
VkBool32 Example::update(const vkts::IUpdateThreadContext& updateContext)
{
	//
	// Mouse.
	//

	static glm::ivec2 lastMouseLocation = updateContext.getMouseLocation(
			windowIndex);
	static VkBool32 lastMouseButtons[3] =
	{ VK_FALSE, VK_FALSE, VK_FALSE };
	static int32_t lastMouseWheel = updateContext.getMouseWheel(windowIndex);

	if (updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_LEFT)
			&& !lastMouseButtons[VKTS_MOUSE_BUTTON_LEFT])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Left mouse button pressed.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_LEFT] = VK_TRUE;
	}
	if (updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_RIGHT)
			&& !lastMouseButtons[VKTS_MOUSE_BUTTON_RIGHT])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Right mouse button pressed.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_RIGHT] = VK_TRUE;
	}
	if (updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_MIDDLE)
			&& !lastMouseButtons[VKTS_MOUSE_BUTTON_MIDDLE])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Middle mouse button pressed.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_MIDDLE] = VK_TRUE;
	}

	if (!updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_LEFT)
			&& lastMouseButtons[VKTS_MOUSE_BUTTON_LEFT])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Left mouse button released.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_LEFT] = VK_FALSE;
	}
	if (!updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_RIGHT)
			&& lastMouseButtons[VKTS_MOUSE_BUTTON_RIGHT])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Right mouse button released.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_RIGHT] = VK_FALSE;
	}
	if (!updateContext.getMouseButton(windowIndex, VKTS_MOUSE_BUTTON_MIDDLE)
			&& lastMouseButtons[VKTS_MOUSE_BUTTON_MIDDLE])
	{
		vkts::logPrint(VKTS_LOG_INFO, "Middle mouse button released.");

		lastMouseButtons[VKTS_MOUSE_BUTTON_MIDDLE] = VK_FALSE;
	}

	if (lastMouseLocation.x != updateContext.getMouseLocation(windowIndex).x
			|| lastMouseLocation.y
					!= updateContext.getMouseLocation(windowIndex).y)
	{
		vkts::logPrint(VKTS_LOG_INFO, "Mouse location changed: %d %d",
				updateContext.getMouseLocation(windowIndex).x,
				updateContext.getMouseLocation(windowIndex).y);

		lastMouseLocation = updateContext.getMouseLocation(windowIndex);
	}

	if (lastMouseWheel != updateContext.getMouseWheel(windowIndex))
	{
		vkts::logPrint(VKTS_LOG_INFO, "Mouse wheel changed: %d",
				updateContext.getMouseWheel(windowIndex));

		lastMouseWheel = updateContext.getMouseWheel(windowIndex);
	}

	//
	// Keyboard.
	//

	static VkBool32 lastKeys[1024 + 1] =
	{ VK_FALSE };

	for (int32_t keyCode = 0; keyCode <= 1024; keyCode++)
	{
		if (updateContext.getKey(windowIndex, keyCode) && !lastKeys[keyCode])
		{
			vkts::logPrint(VKTS_LOG_INFO, "Key pressed: %d", keyCode);

			if (keyCode < 256)
			{
				vkts::logPrint(VKTS_LOG_INFO, "           : '%c'", keyCode);
			}

			lastKeys[keyCode] = VK_TRUE;
		}
		if (!updateContext.getKey(windowIndex, keyCode) && lastKeys[keyCode])
		{
			vkts::logPrint(VKTS_LOG_INFO, "Key released: %d", keyCode);

			if (keyCode < 256)
			{
				vkts::logPrint(VKTS_LOG_INFO, "           : '%c'", keyCode);
			}

			lastKeys[keyCode] = VK_FALSE;
		}
	}

	//
	// Gamepad.
	//

	static VkBool32 lastGamepadButtons[VKTS_MAX_GAMEPAD_BUTTONS] =
	{ VK_FALSE };

	static float lastJoystickAxis[VKTS_MAX_GAMEPAD_AXIS] =
	{ 0.0f };

	for (int32_t buttonIndex = 0; buttonIndex < VKTS_MAX_GAMEPAD_BUTTONS;
			buttonIndex++)
	{
		if (updateContext.getGamepadButton(windowIndex, 0, buttonIndex)
				&& !lastGamepadButtons[buttonIndex])
		{
			vkts::logPrint(VKTS_LOG_INFO, "Gamepad button pressed: %d",
					buttonIndex);

			lastGamepadButtons[buttonIndex] = VK_TRUE;
		}
		if (!updateContext.getGamepadButton(windowIndex, 0, buttonIndex)
				&& lastGamepadButtons[buttonIndex])
		{
			vkts::logPrint(VKTS_LOG_INFO, "Gamepad button released: %d",
					buttonIndex);

			lastGamepadButtons[buttonIndex] = VK_FALSE;
		}
	}

	for (int32_t axisIndex = 0; axisIndex < VKTS_MAX_GAMEPAD_AXIS; axisIndex++)
	{
		if (updateContext.getGamepadAxis(windowIndex, 0, axisIndex)
				!= lastJoystickAxis[axisIndex])
		{
			vkts::logPrint(
			VKTS_LOG_INFO, "Gamepad axis changed: %d %f", axisIndex,
					updateContext.getGamepadAxis(windowIndex, 0, axisIndex));

			lastJoystickAxis[axisIndex] = updateContext.getGamepadAxis(
					windowIndex, 0, axisIndex);
		}
	}

	return VK_TRUE;
}

//
// Vulkan termination.
//
void Example::terminate(const vkts::IUpdateThreadContext& updateContext)
{
}
