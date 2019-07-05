#pragma once

#include "Event.h"

namespace BlueMarble {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: oWidth(width), oHeight(height) {}

		inline unsigned int GetWidth() const { return oWidth; }
		inline unsigned int GetHeight() const { return oHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << oWidth << ", " << oHeight;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowResize"; }
		virtual int GetCategoryFlags() const override { return (EventCategoryApplication); }
	private:
		unsigned int oWidth, oHeight;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowClose"; }
		virtual int GetCategoryFlags() const override { return (EventCategoryApplication); }
	};
}