#ifndef _EVENT_STACK_H
#define _EVENT_STACK_H

#include <deque>
#include <vector>
#include <string>
/*
 * eventually make this class generic.
 * I attempted to make it so, but messing
 * with iterators and templates just is not fun
 */

class EventStack {
private:
	std::deque<std::string> eventStack;
	std::deque<int> timeOnStack;
public:
	void addEvent(std::string x, int time) {
		eventStack.push_back(x);
		timeOnStack.push_back(time);
	}

	std::string getNextEvent() {
		return eventStack.front();
	}

	bool deleteEvent() {
		if (!eventStack.empty()) {
			eventStack.pop_front();
			timeOnStack.pop_front();
			return true;
		}
		else
			return false;
	}

	void clearEventStack() {
		for (int i = 0; i < eventStack.size(); i++) {
			eventStack.pop_front();
			timeOnStack.pop_front();
		}
	}

	int getTimeOnStack() {
		return timeOnStack.front();
	}

	std::deque<std::string>::const_iterator getFront() {
		return eventStack.begin();
	}

	std::deque<std::string>::const_iterator getBack() {
		return eventStack.end();
	}

	bool isEmpty() {
		return eventStack.empty();
	}
};

#endif
