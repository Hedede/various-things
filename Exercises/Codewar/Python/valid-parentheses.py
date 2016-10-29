def valid_parentheses(string):
    stack = []
    for c in string:
        if c == '(':
            stack.append(')')
        elif c == ')':
            if not stack:
                return False
            stack.pop()
    return not stack
