#pragma once
#include <memory>

using std::weak_ptr;

template<typename T>
bool wp_equals(weak_ptr<T> p1, weak_ptr<T> p2) {
	return !p1.owner_before(p2) && !p2.owner_before(p1);
}