#ifndef STACK
#define STACK

#include <stack>
#include <cstdint>

class Stack {
    public:
    void push(uint16_t value);
    uint16_t pop();
    bool empty() const;
    size_t size() const;

    private:
        std::stack<uint16_t> _stack;
};

#endif