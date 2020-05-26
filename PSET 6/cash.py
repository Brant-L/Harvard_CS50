from cs50 import get_float

c = get_float("Change owed:\n")

while c < 0.00:
    c = get_float("Change owed:\n")

rem = round(c * 100)
count = 0

while rem >= 25:
    count += 1
    rem -= 25

while rem >= 10:
    count += 1
    rem -= 10

while rem >= 5:
    count += 1
    rem -= 5

while rem >= 1:
    count += 1
    rem -= 1

print(count)
