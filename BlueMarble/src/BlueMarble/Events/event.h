#pragma once

#include "bmpch.h"
#include "BlueMarble/core.h"

namespace BlueMarble {

	enum class EventType
	{
		None = 0,
		MousePress,
		MouseRelease,
		MouseMove,
		MouseScroll,
		KeyPress,
		KeyRelease,
		KeyType,
		WindowClose,
		WindowFocus,
		WindowResize,
		WindowLostFocus,
		WindowMove
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class Event
	{
	public:
		bool Handled = false;

		virtual std::string ToString() const { return GetName(); }
		virtual const char* GetName() const = 0;

		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
