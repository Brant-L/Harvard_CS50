from sys import argv, exit
from csv import reader, DictReader
from cs50 import SQL

db = SQL("sqlite:///students.db")

# Check for the correct number of command line arguments
if len(argv) != 2:
    print('Error: check command line arguments')
    exit(1)

# Open csv file and read contents into list
else:
    with open(argv[1]) as characters:
        c = reader(characters)
        cData = list(c)

        # Iterate through list skipping first row values
        for i in range(1, len(cData), 1):

            # Parse names into first, middle, last
            # Insert None if middle name is not listed
            cName = cData[i][0].split()
            if len(cName) < 3:
                cName.insert(1, None)

            # Store house data for each character
            cHouse = cData[i][1]

            # Store birth year data for each character
            cBirth = cData[i][2]

            # Import data into SQL database
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       cName[0], cName[1], cName[2], cHouse, cBirth)