#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>

struct Variable {
	std::string name;
	std::string value;
	std::string type;
};

bool isVarName(std::vector<Variable>& var,std::string& name)
{
	for (size_t i = 0; i < var.size(); ++i)
	{
		if (var[i].name == name) return true;
	}
	return false;
}

bool isNum(char ch)
{
	for (size_t k = 48; k <= 57; ++k)
	{
		if (ch == k)
			return true;
	}
	return false;
}

void Type(std::string& varValue, std::string& varType)
{
	size_t count = 0;
	for (size_t j = 0; j < varValue.size(); ++j)
	{
		if (!isNum(varValue[j]) && varValue[j] != '.')
		{
			varType = "std::string";
			return;
		}
		if (varValue[j] == '.') ++count;
	}
	if (count == 1) varType = "double";
	if (count == 0) varType = "int";
}

int main(int argc, char** argv)
{
	std::vector<Variable> variables;
	std::vector<std::pair<size_t, std::string>> ids;
	//file open
	std::ifstream fin;
	std::string file;

	fin.open(argv[1]);

	if (!fin.is_open())
	{
		std::cout << "Errorrr!" << std::endl << argc;
	}
	else
	{
		char ch;
		while (fin.get(ch))
		{
			file += ch;
		}
	}
	fin.close();

	//delete spaces and next lines
	file.erase(std::remove(file.begin(), file.end(), '\n'),
		file.end());
	std::regex space_re(R"(\s+)");

	file = std::regex_replace(file, space_re, " ");

	//split strings in vector
	std::vector<std::string> strings;

	std::string tmp;
	for (size_t i = 0; i < file.size(); ++i)
	{
		tmp += file[i];
		if (file[i] == ';')
		{
			strings.push_back(tmp);
			tmp = "";
		}
	}

	//lexer
	for (size_t i = 0; i < strings.size(); ++i)
	{
		//variable
		std::string tmp = "";
		for (auto j = 0; j <= 2; ++j)
		{
			tmp += strings[i][j];
		}
		if (tmp == "var")
		{
			std::string varName;
			std::string varValue;
			std::string varType;

			Variable var;


			size_t j = 4;
			while (strings[i][j] != ' ' && strings[i][j] != ';')
			{
				varName += strings[i][j];
				++j;
			}
			var.name = varName;
			for (size_t j = 48; j <= 57; ++j)
			{
				if (varName[0] == j) std::cout << "Variable line cann't begin from number!!!" << std::endl;
			}
			if (varName[varName.size() - 1] == ' ') varName.erase(varName.size() - 1, 1);
			size_t ix = j + 3;
			
			if (strings[i].size() >= (ix + 1))
			{
				for (size_t j = ix; j < strings[i].size() - 1; ++j)
				{
					varValue += strings[i][j];
				}
				var.value = varValue;

				Type(varValue, varType);

				var.type = varType;

				if (varName.size() && varValue.size() && varType.size())
				{
					strings[i].replace(0, 3, varType);
				}
			}
			else 
			{
				std::pair<size_t, std::string> p(i, varName);
				ids.push_back(p);
			}
			variables.push_back(var);
		}

		//print
		tmp = "";
		for (size_t j = 0; j <= 4; ++j)
		{
			tmp += strings[i][j];
		}
		if (tmp == "print")
		{
			std::string printValue;

			strings[i].replace(0, 5, "std::cout << ");
			size_t k = 14;

			while (strings[i][k] != ';')
			{
				printValue += strings[i][k];
				++k;
			}
			if (!isVarName(variables, printValue))
			{
				printValue = "\"" + printValue + "\"";
				strings[i].replace(13, printValue.size() - 1, printValue);
			}
		}

		//heto veragrelu hamar
		size_t val = 0;
		std::string temp;
		while (strings[i][val] != ' ')
		{
			temp += strings[i][val];
			++val;
		}
		if (isVarName(variables, temp))
		{
			val += 3;
			std::string varValue;
			std::string varType;
			while (strings[i][val] != ';')
			{
				varValue += strings[i][val];
				++val;
			}
			for (size_t l = 0; l < variables.size(); ++l)
			{
				if (variables[l].name == temp)
				{
					variables[l].value = varValue;
					Type(varValue, varType);
					variables[l].type = varType;
				}
			}
		}

	}
	for (size_t ix = 0; ix < ids.size(); ++ix)
	{
		for (size_t jx = 0; jx < variables.size(); ++jx)
		{
			if (variables[jx].name == ids[ix].second)
			{
				strings[ids[ix].first].replace(0, 3, variables[jx].type);
			}
		}
	}

	for (size_t i = 0; i < strings.size(); ++i)
	{
		std::cout << strings[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << file;

	return 0;
}