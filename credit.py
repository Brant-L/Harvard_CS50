from cs50 import get_string

# Prompt for card number
n = get_string("Number:\n")

# Find length of card number
x = len(n)

# Find leading digits of card number
a = list(n)
y = int(a[0] + a[1])
z = int(a[0])

# Index of second to last card number
dec = 1
b = int(a[x - dec])

# Apply Luhn's Algorithm
temp = int(n)
sum_digits = 0

# Loop through every second digit from second to last digit
for i in range(b, 0, 1):
    # Check if digit is positive and greater than 9
    digit = temp % 10
    if i % 2 == 0:
        if digit * 2 > 9:
            sum_digits += (digit * 2) - 9
        else:
            sum_digits += digit * 2
    else:
        sum_digits += digit
    temp /= 10

    dec += 2


# Declare bool to test algorithm result
luhn = None
if sum_digits % 10 == 0:
    luhn = True

# Check card numbers against conditions
if y == 34 or y == 37 and x == 15 and luhn:
    print("AMEX\n")

elif y > 50 and y < 56 and x == 16 and luhn:
    print("MASTERCARD\n")

elif (x == 13 or x == 16) and (z == 4 and luhn):
    print("VISA\n")

else:
    print("INVALID\n")