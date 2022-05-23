#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Parsers to allow _slightly_ more complex expressions in
    formats and the configuration.
"""

import math


def sgn(x):
    r"The sign function."
    if x > 0.0:
        return 1.0
    elif x < 0.0:
        return -1.0
    return 0.0


def theta(x):
    r"""
    Basic mathematical function sometimes refered to as the step function.

    Allows us to capture discontinuties in the parametric function definitions.
    """
    if x < 0.0:
        return 0.0
    return 1.0


def identify_leaf(exp):
    """
    Find an expression with no parentheses within it that
    can be resolved.
    """
    for i in range(len(exp)):
        if exp[i] == "(":
            for j in range(i+1, len(exp)):
                if exp[j] == "(":
                    break
                if exp[j] == ")":
                    return exp[i:j+1]
    return exp


def resolve_expression(exp):
    r"Solve an expression with no subexpressions."
    # Parse the expression into an operation and numbers.
    exp = exp[1:-1]
    exp = exp.split(" ")
    operation = exp[0]
    numbers = [float(i) for i in exp[1:]]

    result = 0
    if operation == "+":
        for i in numbers:
            result += i

    elif operation == "-":
        result = numbers[0]
        for i in numbers[1:]:
            result -= i

    elif operation == "*":
        result = 1
        for i in numbers:
            result *= i

    elif operation == "/":
        result = numbers[0]
        for i in numbers[1:]:
            result /= i

    elif operation == "^":
        result = numbers[0]
        for i in numbers[1:]:
            result = result**i

    elif operation == "sin":
        result = math.sin(numbers[0])

    elif operation == "cos":
        result = math.cos(numbers[0])

    elif operation == "tan":
        result = math.tan(numbers[0])

    elif operation == "abs":
        result = abs(numbers[0])

    elif operation == "sgn":
        result = sgn(numbers[0])

    elif operation == "theta":
        result = theta(numbers[0])

    return str(result)


def list_processor(exp):
    """
    Our lisp expression processor.

    A lisp-style expression processor for our design primatives
    could resolve our data/code conundrum here.

    We need a safe way to express a function that only
    returns one value, given one input value. So a lisp expression
    without any applications of functions would be enough.

    For example:

    (* (/ 8 11) (sin (- (/ 10 9) (* 56 t))))

    represents

    8/11*math.sin(10/9-56*t)

    For now we'll need to deal with replacing using .replace()
    the t for the input value before calling this.
    """
    exp = exp.replace("\n", "")

    leaf = identify_leaf(exp)
    while "(" in exp:
        exp = exp.replace(leaf, resolve_expression(leaf))
        leaf = identify_leaf(exp)

    return exp
