import string

def spin_words(sentence):
    words = sentence.split(' ');
    for i,word in enumerate( words ):
        if len( word ) >= 5:
            words[i] = word[::-1]
    return string.join(words, ' ');
