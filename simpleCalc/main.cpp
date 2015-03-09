#include <iostream>
#include <vector>
#include <string>

using namespace std;
const int sign_mod = INT_MAX;
const int sign_mult = INT_MAX - 1;
const int sign_div = INT_MAX - 2;
const int sign_minus = INT_MAX - 3;
const int sign_plus = INT_MAX - 4;

vector<int> inp;

//If input error, force to exit
void forceEnd(string s)
{
	cout << s;
	getchar();
	exit(0);
}


//Calculate exptression
void calc(int start, int end)
{
	for (int sign = sign_mod; sign >= sign_plus; sign--)
	{
		int i = start;
		while (i < end)
		{
			if (inp[i] == sign)
			{
				switch (sign)
				{
				case sign_mod:
					inp[i - 1] = inp[i - 1] % inp[i + 1];
					break;
				case sign_mult:
					inp[i - 1] = inp[i - 1] * inp[i + 1];
					break;
				case sign_div:
					if (inp[i + 1] == 0)
						forceEnd("Wrong input: division by zero");
					inp[i - 1] = inp[i - 1] / inp[i + 1];
					break;
				case sign_plus:
					inp[i - 1] = inp[i - 1] + inp[i + 1];
					break;
				case sign_minus:
					inp[i - 1] = inp[i - 1] - inp[i + 1];
					break;
				}
				for (int j = 0; j <= 1; j++) 
					inp.erase(inp.begin() + i);
				end -= 2;
			}
			else
				i++;
		}
	}
}

int main()
{
	bool lastIsSign = 1;
	string inps;
	getline(cin, inps);
	int start = 0;
	int i = 0;
	int end = 0;
	while (i < inps.length())
	{
		if ((inps[i] != ' ' ) && (i != inps.length() - 1))
		{
			i++;
			continue;
		}
		else
		{
			if (i != inps.length() - 1) end = i - 1;
			else end = 1;
			if (inps.substr(start, end - start + 1) == "+")
			{
				if (lastIsSign)
					forceEnd("Wrong input");
				inp.push_back(sign_plus);
				lastIsSign = 1;
			}
			else if (inps.substr(start, end - start + 1) == "-")
			{
				if (lastIsSign)
					forceEnd("Wrong input");
				inp.push_back(sign_minus);
				lastIsSign = 1;
			}
			else if (inps.substr(start, end - start + 1) == "*")
			{
				if (lastIsSign)
					forceEnd("Wrong input");
				inp.push_back(sign_mult);
				lastIsSign = 1;
			}
			else if (inps.substr(start, end - start + 1) == "/")
			{
				if (lastIsSign)
					forceEnd("Wrong input");
				inp.push_back(sign_div);
				lastIsSign = 1;
			}
			else if (inps.substr(start, end - start + 1) == "%")
			{
				if (lastIsSign)
					forceEnd("Wrong input");
				inp.push_back(sign_mod);
				lastIsSign = 1;
			}
			else
			{
				if (!lastIsSign)
					forceEnd("Wrong input");
				try
				{
					inp.push_back(stoi(inps.substr(start, end - start + 1)));
				}
				catch (...)
				{
					forceEnd("Wrong input");
				}
				lastIsSign = 0;
			}
			i++;
			start = i;
		}
	}
	calc(0, inp.size());
	for (int i = 0; i < inp.size(); i++)
		cout << inp[i] << " ";
	getchar();
	exit;
	return 0;
}