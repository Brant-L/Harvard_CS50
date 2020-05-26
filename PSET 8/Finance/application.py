import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, integer, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query ledger table to for homepage table attributes
    data = db.execute("SELECT DISTINCT(symbol), SUM(shares) AS shares, name, total FROM ledger WHERE user_id = :id GROUP BY symbol",
                        id=session.get("user_id"))

    total_holdings = 0

    for row in data:
            symbol = row["symbol"]
            shares = row["shares"]
            name = row["name"]
            quote = lookup(symbol)
            row.update({"price":float(quote["price"])})
            row.update({"total":row["price"] * row["shares"]})
            total_holdings += row["total"]

    # Query users table for current user's cash balance
    balance = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session.get("user_id"))

    cash = balance[0]["cash"]
    cash_usd = usd(balance[0]["cash"])

    grand_total = usd(cash + total_holdings)

    return render_template("index.html", cash_usd=cash_usd, data=data, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        shares = request.form.get("shares")

        # Return apology if symbol isn't entered
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)

        # Return apology if shares aren't entered
        elif not request.form.get("shares"):
            return apology("must enter number of shares", 403)

        # Return apology if shares input is not an integer
        elif integer(shares) == None:
            return apology("enter shares as whole number", 403)

        # Return apology if shares input is not > 0
        elif int(shares) < 1:
            return apology("must buy at least one share", 403)

        # Return apology is stock does not exist
        elif lookup(request.form.get("symbol")) == None:
            return apology("stock does not exist", 403)

        # Update cash attribute in database with successful purchase
        else:

            # Query database to find cash attribute for current user
            # Cast as float
            rows = db.execute("SELECT cash FROM users WHERE id = :id",
                              id=session.get("user_id"))

            cash = (rows[0]["cash"])

            # Update price variable if lookup function doesn't return None value
            stock = lookup(request.form.get("symbol"))
            for key, value in stock.items():
                if key == "price":
                    price = float(value)
                elif key == "name":
                    name = value
                else:
                    symbol = value

            balance = float(cash) - int(shares) * price

            # Return apology if user can't afford shares
            if int(shares) * price > cash:
                return apology("insufficient funds for purchase", 403)

            else:
                db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                           cash=balance,
                           id=session.get("user_id"))


                db.execute("INSERT INTO ledger (user_id, name, symbol, price, shares, total) VALUES (:user_id, :name, :symbol, :price, :shares, :total)",
                           user_id=session.get("user_id"),
                           name=name,
                           symbol=symbol,
                           price=price,
                           shares=int(shares),
                           total=int(shares) * price)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query ledger table for history table attributes
    rows = db.execute("SELECT symbol, shares, price, date_time FROM ledger WHERE user_id = :id",
                      id=session.get("user_id"))

    for row in rows:
        price = row["price"]
        price_usd = usd(price)
        time = row["date_time"]
        transacted = str(time)
        row.update({"price_usd":price_usd})
        row.update({"transacted":transacted})


    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Run lookup function against passed in symbol
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        # Return error if stock doesn't exist
        if quote == None:
            return apology("stock does not exist", 403)

        # Return quote if stock does exist
        else:
            for key, value in quote.items():
                if key == "name":
                    name = value
                elif key == "symbol":
                    symbol = value
                elif key == "price":
                    price = value

        # Navigate to quote information page
        return render_template("quoted.html", name=name, symbol=symbol, price=price)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        # Ensure confirmation matches password
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("password entries do not match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username doesn't already exist
        if len(rows) != 0:
            return apology("username already taken", 403)

        # Insert new user into users table of finance.db
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :pwhash)",
                       username=request.form.get("username"),
                       pwhash=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/reset", methods=["GET", "POST"])
def reset():
    """Reset password"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure old password was submitted
        elif not request.form.get("old_password"):
            return apology("must provide old password", 403)

        # Ensure new password was submitted
        elif not request.form.get("new_password"):
            return apology("must provide new password", 403)

        # Ensure new password confirmation was entered
        elif not request.form.get("confirmation"):
            return apology("must confirm new password", 403)

        # Ensure new password matches confirmation
        elif request.form.get("confirmation") != request.form.get("new_password"):
            return apology("password entries do not match", 403)

        # Ensure new password doesn't match old password
        elif request.form.get("new_password") == request.form.get("old_password"):
            return apology("new password may not match old password", 403)

        # Query users table for username entered
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username & old password match existing user
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("invalid username and/or password", 403)

        # Update users table with new password for user
        else:
            db.execute("UPDATE users SET hash = :pwhash WHERE username = :username",
                       username=request.form.get("username"),
                       pwhash=generate_password_hash(request.form.get("new_password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("reset.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        shares = request.form.get("shares")

        # Return apology if symbol isn't entered
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)

        # Return apology if shares aren't entered
        elif not request.form.get("shares"):
            return apology("must enter number of shares", 403)

        # Return apology if shares input is not an integer
        elif integer(shares) == None:
            return apology("enter shares as whole number", 403)

        # Return apology if shares input is not > 0
        elif int(shares) < 1:
            return apology("must buy at least one share", 403)

        # Update cash attribute in database with successful purchase
        else:

            # Query ledger table to determine what shares are currently owned by user
            rows = db.execute("SELECT DISTINCT(symbol), SUM(shares) AS shares FROM ledger WHERE user_id = :id AND symbol = :symbol",
                              id=session.get("user_id"),
                              symbol=request.form.get("symbol"))

            for row in rows:
                # Return apology if user doesn't own any shares in specified company
                if row["symbol"] == None:
                    return apology("do not own any shares in this company", 403)

                available = row["shares"]
                quote = lookup(row["symbol"])

                # Return apology if user owns fewer shares than trying to sell
                if int(shares) > available:
                    return apology("do not own enough shares", 403)

                else:
                    # Update cash balance for user following sale
                    rows = db.execute("SELECT cash FROM users WHERE id = :id",
                                      id=session.get("user_id"))

                    cash = (rows[0]["cash"])
                    balance = cash + int(shares) * quote["price"]
                    db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                               cash=balance,
                               id=session.get("user_id"))

                    # Update ledger table to reflect sale
                    db.execute("INSERT INTO ledger (user_id, name, symbol, price, shares, total) VALUES (:user_id, :name, :symbol, :price, :shares, :total)",
                           user_id=session.get("user_id"),
                           name=quote["name"],
                           symbol=quote["symbol"],
                           price=quote["price"],
                           shares=-int(shares),
                           total=-int(shares) * quote["price"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
