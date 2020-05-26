from cs50 import SQL
from sys import argv, exit

db = SQL("sqlite:///students.db")

# Check command line arguments
if len(argv) != 2:
    print('Error: check command line arguments')
    exit(1)

# Query database for all students in house
else:
    query = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first;", argv[1])

    # Extract values from dictionaries contained in SQL output list
    for i in range(len(query)):
        result = [value for key, value in query[i].items()]

        # Print out each student's full name and birth year
        if result[1] != None:
            print(result[0] + ' ' + result[1] + ' ' + result[2] + ', born' + ' ' + str(result[3]))
        else:
            print(result[0] + ' ' + result[2] + ', born' + ' ' + str(result[3]))
