#!/usr/bin/python

import operator

def min1(seq, op=operator.lt):
    seq = iter(seq)
    try:
        result = next(seq)
    except StopIteration:
        return None
    for x in seq:
        if op(x, result):
            result = x
    return result

def add_to_counter(counter, op, zero, carry):
    for i, c in enumerate(counter):
        if c == zero:
            counter[i] = carry
            return zero
        carry = op(c, carry)
        counter[i] = zero
    return carry

def reduce_counter(counter, op, zero):
    it = iter(counter)
    result = None
    for c in it:
        if c != zero:
            result = c
            break

    if result is None:
        return zero

    for c in it:
        if c != zero:
            result = op(c, result)
    return result

class BinaryCounter:
    def __init__(self, op, zero):
        self.op = op
        self.zero = zero
        self.counter = [zero]*16

    def add(self, x):
        x = add_to_counter(self.counter, self.op, self.zero, x)
        if x != self.zero: self.counter.append(x)

    def reduce(self):
        return reduce_counter(self.counter, self.op, self.zero)

class Min2Op:
    def __init__(self, op=operator.lt):
        self.op = op

    def __call__(self, x, y):
        if self.op(y[0], x[0]):
            y[1].insert(0, x[0])
            return y
        else:
            x[1].append(y[0])
            return x

def min2(seq):
    op = Min2Op()
    counter = BinaryCounter(op, (None, []))
    for x in seq:
        counter.add((x, []))
    m = counter.reduce()
    return m[0], min1(m[1])

def main():
    # data = [7, 15, 45, 3, 14, 12, 9]
    data = range(1000*1000*10)
    print min2(data)

if __name__ == '__main__':
    main()