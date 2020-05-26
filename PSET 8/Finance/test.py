from cs50 import SQL
from helpers import lookup

db = SQL("sqlite:///finance.db")

rows = db.execute("SELECT symbol, shares, price, date_time FROM ledger WHERE user_id = 2")
