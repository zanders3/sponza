// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Observer
//	- Provides a safe way to notify others of changes.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <list>
#include <functional>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace game
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

template <class F> class Notifier;

template <class F>
class Observer
{
public:
	friend class Notifier<F>;

	typedef typename std::function<void (const F&)> ObserverCallback;

	Observer(
		ObserverCallback	callback
	);

	~Observer();

	void Notify(const F& args);

private:
	Notifier<F>*	 m_notifier;
	ObserverCallback m_callback;
};

// -----------------------------------------------------------------------------

template <class F>
class Notifier
{
public:
	friend class Observer<F>;

	Notifier() {}
	~Notifier() {}

	void
	Notify(
		const F& eventArgs
	)
	{
		for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
		{
			(*iter)->Notify(eventArgs);
		}
	}

	void
	Register(
		Observer<F>* observer
	)
	{
		observer->m_notifier = this;
		m_observers.push_back(observer);
	}

private:
	std::list<Observer<F>*> m_observers;
};

// -----------------------------------------------------------------------------

template <class F> 
Observer<F>::Observer(
	ObserverCallback	callback
) : m_notifier(nullptr),
	m_callback(callback)
{
}

// -----------------------------------------------------------------------------

template <class F> 
Observer<F>::~Observer()
{
	assert(m_notifier);
	m_notifier->m_observers.remove(this);
}

// -----------------------------------------------------------------------------

template <class F> void 
Observer<F>::Notify(const F& args)
{
	m_callback(args);
}

// -----------------------------------------------------------------------------

}//namespace game