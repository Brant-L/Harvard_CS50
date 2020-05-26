from cs50 import get_int

# Prompt for integer between 1 and 8 inclusive
x = 0
while x < 1 or x > 8:
    x = get_int("Enter pyramid height:\n")

# Loop through each row to print correct number of spaces and hashes
for i in range(1, x + 1, 1):
    print(" " * (x - i) + "#" * i)
