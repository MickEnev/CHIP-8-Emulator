#include <stdexcept>
#include "stack.h"

void Stack::push(uint16_t value) {
    _stack.push(value);
}

uint16_t Stack::pop() {
    if (_stack.empty()) {
        throw std::runtime_error("Stack underflow!");
    }
    uint16_t value = _stack.top();
    _stack.pop();
    return value;
}

bool Stack::empty() const {
    return _stack.empty();
}

size_t Stack::size() const {
    return _stack.size();
}