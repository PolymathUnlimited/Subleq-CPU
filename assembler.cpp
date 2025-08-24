#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isInt(string str)
{
	if (str.length() == 0) return false;
	if (str[0] == '-') str.erase(0, 1);
	if (str.length() == 0) return false;

	if (str.length() == 1 && (str[0] < '0' || str[0] > '9')) return false;

	if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) // binary numbers
	{
		for (int i = 2; i < str.length(); ++i) if (str[i] != '0' && str[i] != '1') return false;
	}
	else if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) // hexadecimal numbers
	{
		for (int i = 2; i < str.length(); ++i)
		{
			if ((str[i] < '0' || str[i] > '9') && (str[i] < 'A' || str[i] > 'F') && (str[i] < 'a' || str[i] > 'f'))
				return false;
		}
	}
	else
	{
		for (int i = 0; i < str.length(); ++i)
			if (str[i] < '0' || str[i] > '9') return false;
	}

	return true;
}

int toInt(string num)
{
	if (!isInt(num)) return 0;

	int sign = 1;
	if (num[0] == '-')
	{
		num.erase(0, 1);
		sign = -1;
	}

	if (num.length() < 2) return sign * stoi(num);

	if (num[1] == 'b' || num[1] == 'B')
	{
		num.erase(0, 2);
		return sign * stoi(num, nullptr, 2);
	}
	else if (num[1] == 'x' || num[1] == 'X')
	{
		return sign * stoi(num, nullptr, 0);
	}
	else return sign * stoi(num);
}

string toHex(const string& str)
{
	if (!isInt(str)) return "";

	int num = toInt(str);

	if (num == 0) return "0";

	string result = "";
	while (num)
	{
		char digit = num % 16;
		num /= 16;

		if (digit > 9)
		{
			digit -= 10;
			digit += 'A';
		}
		else
		{
			digit += '0';
		}

		result.insert(result.begin(), digit);
	}

	return result;
}

int main()
{
	// read assembly code from a file
	string filename = "code.asm";
	ifstream fin(filename);

	string line;
	string code;
	while (getline(fin, line)) code += line + "\n";

	fin.close();

	// add whitespace between mathematical operators
	for (int i = 0; i < code.length(); ++i)
	{
		if (code[i] == '=' || code[i] == '(' || code[i] == ')' || code[i] == '^' || code[i] == '*' || code[i] == '/' || code[i] == '%' || code[i] == '+' || code[i] == '-')
		{
			code.insert(code.begin() + i + 1, ' ');
			code.insert(code.begin() + i, ' ');
			++i;
		}
	}

	cout << "Code:" << endl << endl;
	cout << code << endl;

	// parse the assembly code into symbols
	vector<string> symbols;
	string symbol = "";
	for (int i = 0; i < code.length(); ++i)
	{
		char c = code[i];

		if (c == ' ' || c == '\n' || c == '\t')
		{
			if (symbol != "") symbols.push_back(symbol);
			symbol = "";
		}
		else if (c == ';')
		{
			while (i < code.length() && code[i] != '\n') code.erase(i, 1);
		}
		else
		{
			symbol.push_back(c);
		}
	}

	cout << "Raw symbols: " << endl << endl;
	for (string symbol : symbols)
		cout << "\"" << symbol << "\"" << endl;
	cout << endl;

	// add parentheses to mathematical expressions to enforce order of operations
	for (int i = 0; i < symbols.size(); ++i) // exponents
	{
		if (symbols[i] == "^")
		{
			int openIndex = 0, closeIndex = symbols[i].length() - 1;

			int parnum = 0;
			for (int j = i + 1; j < symbols.size(); ++j)
			{
				if (symbols[j] == "(") ++parnum;
				if (symbols[j] == ")") --parnum;
				if (parnum == 0)
				{
					closeIndex = j + 1;
					break;
				}
			}

			parnum = 0;
			for (int j = i - 1; j >= 0; --j)
			{
				if (symbols[j] == ")") ++parnum;
				if (symbols[j] == "(") --parnum;
				if (parnum == 0)
				{
					openIndex = j;
					break;
				}
			}

			symbols.insert(symbols.begin() + closeIndex, ")");
			symbols.insert(symbols.begin() + openIndex, "(");
			++i;
		}
	}

	for (int i = 0; i < symbols.size(); ++i) // multiplication, division, and mod
	{
		if (symbols[i] == "*" || symbols[i] == "/" || symbols[i] == "%")
		{
			int openIndex = 0, closeIndex = symbols[i].length() - 1;

			int parnum = 0;
			for (int j = i + 1; j < symbols.size(); ++j)
			{
				if (symbols[j] == "(") ++parnum;
				if (symbols[j] == ")") --parnum;
				if (parnum == 0)
				{
					closeIndex = j + 1;
					break;
				}
			}

			parnum = 0;
			for (int j = i - 1; j >= 0; --j)
			{
				if (symbols[j] == ")") ++parnum;
				if (symbols[j] == "(") --parnum;
				if (parnum == 0)
				{
					openIndex = j;
					break;
				}
			}

			symbols.insert(symbols.begin() + closeIndex, ")");
			symbols.insert(symbols.begin() + openIndex, "(");
			++i;
		}
	}

	for (int i = 0; i < symbols.size(); ++i) // addition and subtraction
	{
		if (symbols[i] == "+" || symbols[i] == "-")
		{
			int openIndex = 0, closeIndex = symbols[i].length() - 1;

			int parnum = 0;
			for (int j = i + 1; j < symbols.size(); ++j)
			{
				if (symbols[j] == "(") ++parnum;
				if (symbols[j] == ")") --parnum;
				if (parnum == 0)
				{
					closeIndex = j + 1;
					break;
				}
			}

			parnum = 0;
			for (int j = i - 1; j >= 0; --j)
			{
				if (symbols[j] == ")") ++parnum;
				if (symbols[j] == "(") --parnum;
				if (parnum == 0)
				{
					openIndex = j;
					break;
				}
			}

			symbols.insert(symbols.begin() + closeIndex, ")");
			symbols.insert(symbols.begin() + openIndex, "(");
			++i;
		}
	}

	cout << "Symbols (after parentheses added):" << endl << endl;
	for (string symbol : symbols)
	{
		cout << "\"" << symbol << "\"" << endl;
	}
	cout << endl;

	// generate tags
	vector<string> tags;
	vector<int> addresses;
	int currentAddress = 0;

	for (int i = 0; i < symbols.size(); ++i)
	{
		string symbol = symbols[i];

		if (symbol == "subleq") currentAddress += 3;
		if (symbol == ".byte") currentAddress += 1;
		if (symbol.back() == ':')
		{
			// define a tag
			symbol.pop_back();
			tags.push_back(symbol);
			addresses.push_back(currentAddress);
			symbols.erase(symbols.begin() + i);
			--i;
		}
	}

	// replace tags in code
	bool progress = true;
	while (progress)
	{
		progress = false;
		for (int i = 0; i < symbols.size(); ++i)
		{
			string symbol = symbols[i];

			for (int j = 0; j < tags.size(); ++j)
			{
				if (symbol == tags[j])
				{
					symbols[i] = to_string(addresses[j]);
					progress = true;
				}
				if (symbol == "+" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(toInt(symbols[i - 1]) + toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "-" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(toInt(symbols[i - 1]) - toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "*" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(toInt(symbols[i - 1]) * toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "/" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(toInt(symbols[i - 1]) / toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "%" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(toInt(symbols[i - 1]) % toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "^" && i + 1 < symbols.size() && i - 1 >= 0 && isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					symbols[i] = to_string(pow(toInt(symbols[i - 1]), toInt(symbols[i + 1])));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (isInt(symbol) && i + 1 < symbols.size() && i - 1 >= 0 && symbols[i - 1] == "(" && symbols[i + 1] == ")")
				{
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
				if (symbol == "=" && i + 1 < symbols.size() && i - 1 >= 0 && !isInt(symbols[i - 1]) && isInt(symbols[i + 1]))
				{
					// define a tag
					tags.push_back(symbols[i - 1]);
					addresses.push_back(toInt(symbols[i + 1]));
					symbols.erase(symbols.begin() + i + 1);
					symbols.erase(symbols.begin() + i);
					symbols.erase(symbols.begin() + i - 1);
					--i;
					progress = true;
				}
			}
		}
	}

	cout << "symbols (after evaluation):" << endl << endl;
	for (string symbol : symbols)
		cout << "\"" << symbol << "\"" << endl;
	cout << endl;

	cout << "Tags:" << endl << endl;
	for (int i = 0; i < tags.size(); ++i)
		cout << tags[i] << ": " << addresses[i] << endl;

	// add third argument for two-argument subleq's
	currentAddress = 0;
	for (int i = 0; i < symbols.size(); ++i)
	{
		if (symbols[i] == "subleq") currentAddress += 3;
		if (symbols[i] == ".byte") currentAddress += 1;
		if (symbols[i] == "subleq" && (i + 3 >= symbols.size() || symbols[i + 3] == "subleq"))
		{
			symbols.insert(symbols.begin() + i + 3, to_string(currentAddress));
		}
	}

	cout << "symbols (after adding arguments)" << endl << endl;
	for (string symbol : symbols)
		cout << "\"" << symbol << "\"" << endl;
	cout << endl;

	// remove anything that is not a number and translate to hex
	for (int i = 0; i < symbols.size(); ++i)
	{
		if (!isInt(symbols[i]))
		{
			symbols.erase(symbols.begin() + i);
			--i;
		}
		else
		{
			symbols[i] = toHex(symbols[i]);
		}
	}

	// hexdump
	cout << "Hexdump: " << endl << endl;
	for (string symbol : symbols)
		cout << ((symbol.length() < 2) ? "0" : "") << symbol << " ";
	cout << endl << endl;

	// end program
	return 0;
}