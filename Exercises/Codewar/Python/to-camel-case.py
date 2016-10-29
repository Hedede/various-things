import re
def to_camel_case(text):
    if not text:
        return ''
    words = re.split(r'[_-]+', text)
    words = [ words[0] ] + [word.title() for word in words[1:]]
    return ''.join(words)
