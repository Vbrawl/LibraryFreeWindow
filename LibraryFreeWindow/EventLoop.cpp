#include "EventLoop.h"
#include "utils.h"




EventLoop::EventLoop() : running(false) {}
EventLoop::~EventLoop() {}

list<weak_ptr<EventProcessor>>::iterator EventLoop::FindEventProcessor(weak_ptr<EventProcessor> processor) {
	// Simple linear-search for checking if a checker is inserted in m_pollCheckers
	auto itEnd = this->m_pollCheckers.end();
	for (auto itStart = this->m_pollCheckers.begin(); itStart != itEnd; itStart++) {
		if (wp_equals(*itStart, processor)) {
			return itStart;
		}
	}
	return itEnd;
}

list<weak_ptr<EventProcessor>>::iterator EventLoop::FindEventProcessor(EventProcessor* processor) {
	// Simple linear-search for checking if a checker is inserted in m_pollCheckers
	auto itEnd = this->m_pollCheckers.end();
	for (auto itStart = this->m_pollCheckers.begin(); itStart != itEnd; itStart++) {
		if (itStart->lock().get() == processor) {
			return itStart;
		}
	}
	return itEnd;
}


void EventLoop::AddEventProcessor(weak_ptr<EventProcessor> processor) {
	if (this->FindEventProcessor(processor) == this->m_pollCheckers.end()) {
		this->m_pollCheckers.push_back(processor);
	}
}

void EventLoop::RunOnce() {
	Event evt;
	for (auto itStart = this->m_pollCheckers.begin(); itStart != this->m_pollCheckers.end(); itStart++) {
		if (auto processor = itStart->lock()) {
			while ((evt = processor->findNextEvent()).code != NoEvent) {
				if (evt.code == NoMemoryEvent) {
					this->Stop();
					return; // Immediately exit if there is no memory left.
				}
				processor->processEvent(evt);
			}
		}
		else {
			itStart = this->m_pollCheckers.erase(itStart);
		}
	}

	this->m_toRemoveLater_Lock.lock();
	for (auto itStart = this->m_toRemoveLater.begin(); itStart != this->m_toRemoveLater.end(); itStart++) {
		this->RemoveEventProcessor(*itStart);
	}
	this->m_toRemoveLater.clear();
	this->m_toRemoveLater_Lock.unlock();
}


void EventLoop::Start() {
	this->running = true;
	m_MainLoop();
}

void EventLoop::Stop() {
	this->running = false;
}


void EventLoop::m_MainLoop() {
	while (this->running) {
		this->RunOnce();
		if (this->m_pollCheckers.size() == 0) {
			this->running = false;
		}
	}
}


void EventLoop::RemoveEventProcessor(weak_ptr<EventProcessor> processor) {
	auto it = this->FindEventProcessor(processor);
	if (it != this->m_pollCheckers.end()) {
		this->m_pollCheckers.erase(it);
	}
}

void EventLoop::RemoveEventProcessor(EventProcessor* processor) {
	auto it = this->FindEventProcessor(processor);
	if (it != this->m_pollCheckers.end()) {
		this->m_pollCheckers.erase(it);
	}
}


void EventLoop::RemoveEventProcessorLater(EventProcessor* processor) {
	this->m_toRemoveLater_Lock.lock();
	this->m_toRemoveLater.push_back(processor);
	this->m_toRemoveLater_Lock.unlock();
}