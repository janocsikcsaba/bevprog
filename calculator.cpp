
/*
	calculator08buggy.cpp

	Helpful comments removed.

	We have inserted 3 bugs that the compiler will catch and 3 that it won't.
*/

#include "std_lib_facilities.h"

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { } // initialize kind with ch
	Token(char ch, double val) :kind(ch), value(val) { } // initialize kind
								// and value
	Token(char ch, string n) :kind{ch}, name{n} { } // initialize kind
								// and name
};

class Token_stream {
private:
	bool full; // is there a Token in the buffer?
	Token buffer; // here is where we keep a Token put back using
public:
	Token_stream() :full(0), buffer(0) { }

	Token get(); // get a Token
	void unget(Token t) { buffer=t; full=true; }

	void ignore(char); // discard characters up to and including a c
};

const char let = 'L'; //let set a value for a variable
const char quit = 'Q'; //exit the program
const char print = ';'; //print the result
const char number = '8'; //the kind of the number values
const char name = 'a'; //the kind of the string names
const char square_r = 'S';
const char power_r = 'P';

Token Token_stream::get()
{
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '#': return Token{let}; //declaration keyword	
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case ',':
		return Token(ch); // let each character represent itself
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	cin.unget(); // put digit back into the input stream
		double val;
		cin >> val;  // read a floating-point number
		return Token(number,val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s+=ch;
			cin.unget(); 
			if (s == "pow") return Token{power_r};
			if (s == "sqrt") return Token{square_r};
			if (s == "exit") return Token{quit};
			return Token{name,s};
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c) // c represents the kind of Token
{
	// first look in buffer:
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	// now search input:
	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names;	

double get_value(string s)
	// return the value of the Variable named s
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ",s);
}

void set_value(string s, double d)
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}

bool is_declared(string s)
	// is var already in var_table?
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

double define_name(string var, double val)
// add (var,val) to var_table
	{
		if (is_declared(var)) error(var," declared twice");
		names.push_back(Variable(var,val));
		return val;
}

Token_stream ts;

double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
	}
	case '-':
		return - primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);
	case power_r:
		{
				
				Token t = ts.get();
				switch (t.kind) {
				case '(':
				{	double d = expression();
					t = ts.get();
					if (t.kind != ',') error("',' expected");
					int d2 = narrow_cast<int>(expression());
					t = ts.get();
					if (t.kind != ')') error("')' expected");
					return pow(d, d2);
				}
				default:
					error("'('expected");
				}
					
				
			}
		
	case square_r:
		{

				Token t = ts.get();
				switch (t.kind) {
				case '(':
				{	double d = expression();
					t = ts.get();
					if (t.kind != ')') error("')' expected");
					if (d < 0) error("negative expression");
					return sqrt(d);
				}
				default:
					error("'('expected");
				}
					
				
			}	
		
	default:
		error("primary expected");
	}
}

double term()
{
	double left = primary();
	while(true) {
		Token t = ts.get(); // get the next token from token stream
		switch(t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{	double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left = fmod(left,d);
			break;
		}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression()
{
	double left = term(); // read and evaluate a Term
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term(); // evaluate Term and add
			break;
		case '-':
			left -= term(); // evaluate Term and subtract
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration()
	// assume we have seen "let”
	// handle: name = expression
	// declare a variable called "name” with the initial value "expression”
{
	Token t = ts.get();
	if (t.kind != 'a') error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
	double d = expression();
	names.push_back(Variable(name,d));
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while(cin) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.unget(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()

	try {
		define_name("pi",3.1415926535);
		define_name("e",2.7182818284);
		define_name("k",1000);

		calculate();

		return 0;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		char c;
		while (cin >>c&& c!=';') ;
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		char c;
		while (cin>>c && c!=';');
		return 2;
	}
