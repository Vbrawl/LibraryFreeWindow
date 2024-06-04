#pragma once

#include <list>
#include <memory>
#include <mutex>
using std::list;
using std::weak_ptr;
using std::shared_ptr;
using std::atomic_bool;
using std::mutex;


/* Event Codes */
#define NoMemoryEvent		-1
#define UndefinedEvent		0
#define NoEvent				1
#define WindowEvent			2

/* Event Sub Codes */
#define UndefinedSubEvent	0


/* Event Sub Codes: WindowEvent */
#define CloseWindow			0


/* Event structures */
struct Event {
	int code = UndefinedEvent;
	int subCode = UndefinedSubEvent;
	shared_ptr<void> data;		// ACTS AS A UNIQUE_PTR: The reason for it being shared is for type-erasing
								// TODO: Need to find a way to make a unique_ptr accept type-erased values.
};

/* Event processor */
class EventProcessor {
public:
	virtual Event findNextEvent() = 0;
	virtual bool processEvent(const Event& evt) = 0;
};

/* Actual event loop */
class EventLoop {
private:
	list<weak_ptr<EventProcessor>> m_pollCheckers;
	atomic_bool running = false;

	list<EventProcessor*> m_toRemoveLater;
	mutex m_toRemoveLater_Lock;
public:
	EventLoop();
	~EventLoop();

	list<weak_ptr<EventProcessor>>::iterator FindEventProcessor(EventProcessor* processor);
	list<weak_ptr<EventProcessor>>::iterator FindEventProcessor(weak_ptr<EventProcessor> processor);
	void AddEventProcessor(weak_ptr<EventProcessor> processor);
	void RunOnce();
	void Start();
	void Stop();


	void RemoveEventProcessor(weak_ptr<EventProcessor> processor);
	void RemoveEventProcessor(EventProcessor* processor);
	void RemoveEventProcessorLater(EventProcessor* processor);


private:
	void m_MainLoop();
};

