#pragma once

#include "event.h"

namespace BlueMarble {

	class BLUEMARBLE_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y)
			: oMouseX(x), oMouseY(y) {}

		inline float GetX() const { return oMouseX; }
		inline float GetY() const { return oMouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << oMouseX << ", " << oMouseY;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseMove; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseMove"; }			
		virtual int GetCategoryFlags() const override 
		{
			return (EventCategoryMouse | EventCategoryInput);
		}
	private:
		float oMouseX, oMouseY;
	};

	class BLUEMARBLE_API MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: oXOffset(xOffset), oYOffset(yOffset) {}

		inline float GetXOffset() const { return oXOffset; }
		inline float GetYOffset() const { return oYOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseScroll; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseScroll"; }
		virtual int GetCategoryFlags() const override
		{
			return (EventCategoryMouse | EventCategoryInput);
		}
	private:
		float oXOffset, oYOffset;
	};

	class BLUEMARBLE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return oButton; }

		virtual int GetCategoryFlags() const override
		{
			return (EventCategoryMouse | EventCategoryInput);
		}
	protected:
		MouseButtonEvent(int button)
			: oButton(button) {}

		int oButton;
	};

	class BLUEMARBLE_API MousePressEvent : public MouseButtonEvent
	{
	public:
		MousePressEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressEvent: " << oButton;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MousePress; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MousePress"; }
	};

	class BLUEMARBLE_API MouseReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseReleaseEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleaseEvent: " << oButton;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseRelease; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseRelease"; }
	};

}