import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

#from helpers import apology, login_required, lookup, usd
from helpers import apology, login_required, lookup

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

# Custom filters
#app.jinja_env.filters["usd"] = usd
@app.template_filter('usd')
def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

@app.template_filter('operation_amount')
def operationAmount(shares, price):
    """Calculate operation amount"""
    return f"${shares * price:,.2f}"

@app.template_filter('format_operation')
def formatOperation(operation):
    """Format operation"""
    if operation == 0:
        return "Buy"
    else:
        return "Sell"

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

    # Query database for user portfolio
    rows = db.execute("SELECT * FROM portfolio WHERE user_id = :userId",
                          userId=session["user_id"])

    # Initialize list of user stocks
    stocks = []

    # Initialize total value os user stocks
    totalStocks = 0

    # For each user stock
    for row in rows:
        # Look up stock quote
        quote = lookup(row["stock_symbol"])

        # Put stock data in stocks list
        stock = {
            "symbol": row["stock_symbol"],
            "name": row["company_name"],
            "shares": row["number_of_shares"],
            "price": usd(quote["price"]),
            "total": usd(row["number_of_shares"] * quote["price"])
        }
        stocks.append(stock)

        # Uptate total value os user stocks
        totalStocks += row["number_of_shares"] * quote["price"]

    # Query database for user cash
    rows = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session["user_id"])

    # Calculate total user amount
    total = totalStocks + rows[0]["cash"]

    return render_template("index.html", stocks=stocks, cash=usd(rows[0]["cash"]), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Look up quote
        quote = lookup(request.form.get("symbol"))

        # Ensure quote was looked up
        if not quote:
            return apology("stock symbol does not exist")

        # Ensure number of shares was submitted and is positive
        try:
            num = int(request.form["shares"])
            is_num = True
        except:
            is_num = False

        if not is_num or num < 1:
            return apology("must provide positive number of shares")

        totalPrice = quote["price"] * int(request.form.get("shares"))

        # Query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])

        # Ensure cash is enough
        if rows[0]["cash"] < totalPrice:
            return apology("not enough cash")

        # Update database for user cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                          cash = rows[0]["cash"] - totalPrice,
                          id=session["user_id"])

        # Query database for user portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :userId AND stock_symbol = :stockSymbol",
                          userId=session["user_id"], stockSymbol = quote["symbol"])

        # Update database for user portfolio
        if not rows:
            db.execute("INSERT INTO portfolio (user_id, stock_symbol, company_name, number_of_shares) VALUES (:id, :symbol, :name, :shares)",
                        id=session["user_id"], symbol=quote["symbol"], name=quote["name"], shares=int(request.form.get("shares")))
        else:
            db.execute("UPDATE portfolio SET number_of_shares=:shares WHERE user_id=:id AND stock_symbol=:symbol",
                        id=session["user_id"], symbol=quote["symbol"],
                        shares=int(request.form.get("shares")) + rows[0]["number_of_shares"])

        # Update database for user history
        db.execute("INSERT INTO history (user_id, stock_symbol, company_name, number_of_shares, share_price, operation) VALUES (:id, :symbol, :name, :shares, :price, :operation)",
                    id=session["user_id"], symbol=quote["symbol"], name=quote["name"], shares=int(request.form.get("shares")),
                    price=quote["price"], operation=0)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of history"""

    # Query database for user history
    rows = db.execute("SELECT * FROM history WHERE user_id = :userId ORDER BY date DESC, time DESC",
                          userId=session["user_id"])

    if not rows:
        return apology("no hystory")

    return render_template("history.html", history=rows)


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
        session["user_name"] = rows[0]["username"]

        # Report message
        flash('You were successfully logged in')
        flash(request.form.get("username"))

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Look up quote
        quote = lookup(request.form.get("symbol"))

        # Ensure quote was looked up
        if not quote:
            return apology("stock symbol does not exist")

        return render_template("quote.html", quote=quote)


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

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

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("password confirmation does not match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username does not exists
        if rows:
            return apology("username already exists", 403)

        # Insert user into database
        user_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username=request.form.get("username"),
                            hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))

        # Ensure user was inserted
        if not user_id:
            return apology("internal server error", 500)

        # Remember which user has logged in
        session["user_id"] = user_id
        session["user_name"] = request.form.get("username")

        # Report message
        flash('You were successfully logged in')
        flash(request.form.get("username"))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Query database for user portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :userId AND stock_symbol = :stockSymbol",
                          userId=session["user_id"], stockSymbol = request.form.get("symbol"))

        # Look up quote
        quote = lookup(request.form.get("symbol"))

        # Ensure quote was looked up
        if not quote:
            return apology("stock symbol does not exist")

        # Ensure user has stock
        if not rows:
            return apology("no such stock")

        # Ensure number of shares was submitted and is positive
        try:
            num = int(request.form["shares"])
            is_num = True
        except:
            is_num = False

        if not is_num or num < 1:
            return apology("must provide positive number of shares")

        # Ensure user has enough shares
        if rows[0]["number_of_shares"] < int(request.form.get("shares")):
            return apology("not enough shares")

        totalPrice = quote["price"] * int(request.form.get("shares"))

        # Query database for user cash
        cashes = db.execute("SELECT cash FROM users WHERE id = :id",
                            id=session["user_id"])

        # Update database for user cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                    cash = cashes[0]["cash"] + totalPrice,
                    id=session["user_id"])

        # Update database for user portfolio
        numberOfShares = rows[0]["number_of_shares"] - int(request.form.get("shares"))
        if numberOfShares < 1:
            db.execute("DELETE FROM portfolio WHERE user_id=:id AND stock_symbol=:symbol",
                        id=session["user_id"], symbol=request.form.get("symbol"))
        else:
            db.execute("UPDATE portfolio SET number_of_shares=:shares WHERE user_id=:id AND stock_symbol=:symbol",
                        id=session["user_id"], symbol=request.form.get("symbol"),
                        shares=numberOfShares)

        # Update database for user history
        db.execute("INSERT INTO history (user_id, stock_symbol, company_name, number_of_shares, share_price, operation) VALUES (:id, :symbol, :name, :shares, :price, :operation)",
                    id=session["user_id"], symbol=request.form.get("symbol"), name=quote["name"], shares=int(request.form.get("shares")),
                    price=quote["price"], operation=1)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Query database for user portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :userId",
                          userId=session["user_id"])

        # Ensure user has stocks
        if not rows:
            return apology("no stocks")

        # Show page
        symbols = []
        for row in rows:
            symbols.append(row["stock_symbol"])

        return render_template("sell.html", symbols=symbols)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Change cash"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure change cash and is positive
        try:
            num = int(float(request.form["change"]) * 100) / 100
            is_num = True
        except:
            is_num = False

        if not is_num or num <= 0:
            return apology("must provide positive change cash")

        # Query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])

        # Update database for user cash
# Aqui tem que se form.get e nao form["add"] por que o browser so envia o valor do botao clicado,
# nao envia o do outro, gerando uma excessao no flask quando o outro botao e clicado
#        if request.form.get('add'):
        if request.form['operation'] == "add":
            db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                          cash = rows[0]["cash"] + num,
                          id=session["user_id"])
#        elif request.form.get('subtract'):
        elif request.form.get('operation') == "subtract":
            # Ensure cash is enough
            if rows[0]["cash"] < num:
                return apology("not enough cash")

            db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                          cash = rows[0]["cash"] - num,
                          id=session["user_id"])
        else:
            return render_template("cash.html", cash=rows[0]["cash"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])

        return render_template("cash.html", cash=rows[0]["cash"])


@app.route("/password", methods=["GET", "POST"])
def password():
    """Change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("password confirmation does not match", 403)

        # Update user password into database
        db.execute("UPDATE users SET hash = :hash WHERE id = :id",
                    hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8),
                    id=session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("password.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
