from cs50 import get_string

# Prompt user for string of text
s = get_string("Text:")

letter_count = 0
word_count = 1
sentence_count = 0

# Use else if statements to identify and count string components

for c in s:
    if str.isalpha(c):
        letter_count += 1
    elif c is ' ':
        word_count += 1
    elif c is '.' or c is '?' or c is '!':
        sentence_count += 1

g = round(0.0588 * (100 * letter_count) / word_count - 0.296 * (100 * sentence_count) / word_count - 15.8)

if g > 0 and g < 16:
    print(f"Grade: {g}")
elif g >= 16:
    print("Grade: 16+")
else:
    print("Before Grade 1")