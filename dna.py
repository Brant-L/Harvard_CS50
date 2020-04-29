# Import needed modules
from csv import reader, DictReader
from sys import argv, exit


# Check for correct number of command-line arguments
if len(argv) != 3:
    print("Error: run script in format database, sequence")
    exit(1)

# Open database and sequence files and read into memory
else:
    # Create dictionary for STRs in database
    with open(argv[1]) as database:
        db = reader(database)
        for row in db:
            dbSTR = row
            dbSTR.pop(0)
            break

    # Create dictionary to store STR counts
    STRs = {}

    # Copy database list into a dictionary
    # Dictionary keys are the STRs
    for value in dbSTR:
        STRs[value] = 1

    # Create string for sequence file input
    with open(argv[2]) as sequence:
        seqReader = reader(sequence)
        for row in seqReader:
            seq = row[0]

# Iterate through sequence file and count repeated STR values
for key in STRs:
    STRlen = len(key)
    countMax = 0
    count = 0

    for i in range(len(seq)):
        # Reset counter if sequence terminates
        while count > 0:
            count -= 1
            continue

        # Count repeated STRs
        if seq[i:i + STRlen] == key:
            while seq[i - STRlen:i] == seq[i:i + STRlen]:
                count += 1
                i += STRlen

        # Track longest sequence
        if count > countMax:
            countMax = count

    # Add largest STR counts to STR count dictionary
    STRs[key] += countMax

# Open database file
with open(argv[1], newline='') as database:
    db = DictReader(database)
    for value in db:
        match = 0

        # Compare STR counts to each person in database and print name if match is found
        for seq in STRs:
            if STRs[seq] == int(value[seq]):
                match += 1

            if match == len(STRs):
                print(value['name'])
                exit()

    print('No match')