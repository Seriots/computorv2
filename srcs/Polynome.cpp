#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "Polynome.hpp"

Polynome::Polynome()
{
	this->_left.push_back((V){0, 2});
	this->_left.push_back((V){50, 1});
	this->_right.push_back((V){5, 2});
	this->_right.push_back((V){2, 987});
	this->_right.push_back((V){18, 2});

	this->_steps = 0;
	this->_solution_set = false;
	this->_error = false;
}

Polynome::Polynome(int argc, char **argv)
{
	std::string input;
	this->_steps = 0;
	this->_error = false;
	this->_solution_set = false;

	if (argc == 1)
		return;

	for (int i = 1; i < argc; i++)
		input += argv[i];

	this->parseInput(input);
}

Polynome::Polynome(std::string input)
{
	this->_steps = 0;
	this->_error = false;
	this->_solution_set = false;

	this->parseInput(input);
}

Polynome::~Polynome()
{
}

void Polynome::parseInput(std::string &input)
{
	float degree = 0;
	float value = 1;
	bool left = true;
	bool first = true;
	std::string::iterator it = input.begin();

	while (input.find(" ") != std::string::npos)
		input = input.replace(input.find(" "), 1, "");

	while (it != input.end())
	{
		if ((*it == '+' || *it == '-' || *it == '=' || it == input.end()) && first == false)
		{
			if (left == true)
				this->_left.push_back((V){value, degree});
			else
				this->_right.push_back((V){value, degree});
			if (*it == '=')
			{
				left = false;
				first = true;
			}
			value = (*it == '-' ? -1 : 1);
			degree = 0;
			it++;
		}
		else
		{
			first = false;
			if (*it == '-')
			{
				value = -1;
				it++;
			}
			if (parseNumber(it, input.end(), value, degree) == -1)
			{
				std::cout << "Error: Invalid input\n";
				this->_left.clear();
				this->_right.clear();
				this->_error = true;
				return;
			}
		}
	}
	if (left == true)
		this->_left.push_back((V){value, degree});
	else
		this->_right.push_back((V){value, degree});
}

int Polynome::parseNumber(std::string::iterator &it, std::string::iterator end, float &value, float &degree)
{
	std::string value_str;
	std::string degree_str;
	bool mul = false;
	bool x = false;
	bool exp = false;

	while (*it >= '0' && *it <= '9' && it != end)
	{
		value_str += *it;
		it++;
	}
	if (*it == '.')
	{
		value_str += *it;
		it++;
		while (*it >= '0' && *it <= '9' && it != end)
		{
			value_str += *it;
			it++;
		}
	}
	if (!(*it == '*' || *it == 'x' || *it == 'X' || *it == '^' || *it == '+' || *it == '-' || *it == '=' || it == end || (*it >= '0' && *it <= '9')))
		return (-1);

	if (*it == '*')
	{
		mul = true;
		it++;
		if (!(*it == 'x' || *it == 'X' || *it == '^' || *it == '+' || *it == '-' || *it == '=' || it == end || (*it >= '0' && *it <= '9')))
			return (-1);
	}
	if (*it == 'X' || *it == 'x')
	{
		x = true;
		it++;
		if (!(*it == '^' || *it == '+' || *it == '-' || *it == '=' || it == end || (*it >= '0' && *it <= '9')))
			return (-1);
	}
	if (*it == '^')
	{
		exp = true;
		it++;
		if (!(*it == '+' || *it == '-' || *it == '=' || it == end || (*it >= '0' && *it <= '9')))
			return (-1);
	}
	while (*it >= '0' && *it <= '9' && it != end)
	{
		degree_str += *it;
		it++;
	}
	if (!(*it == '+' || *it == '-' || *it == '=' || it == end))
		return (-1);
	value = (value_str.size() == 0 ? value : value * atof(value_str.c_str()));
	degree = (degree_str.size() == 0 ? degree : atof(degree_str.c_str()));

	if (x == false && (mul == true || exp == true || degree_str.size() != 0))
		return (-1);

	return (0);
}

void Polynome::display(std::string prompt = "")
{
	if (prompt.size() > 0)
		std::cout << prompt;
	else
		std::cout << "Step " << this->_steps << ": ";
	if (this->_left.size() == 0 && this->_equation.size() == 0)
		std::cout << "\033[35m0\033[0m ";
	else
	{
		for (std::map<float, float>::reverse_iterator it = this->_equation.rbegin(); it != this->_equation.rend(); it++)
			std::cout << "\033[32m" << (it->second >= 0 ? (it == this->_equation.rbegin() ? "+" : "+ ") : (it == this->_equation.rbegin() ? "-" : "- "))
					  << (it->second >= 0 ? it->second : -it->second)
					  << (it->first != 0 ? " * X^" : "") << (it->first != 0 ? to_string(it->first) : "\0") << "\033[0m ";

		for (std::vector<V>::iterator it = this->_left.begin(); it != this->_left.end(); it++)
			std::cout << "\033[31m" << (it->value >= 0 ? (it == this->_left.begin() && this->_equation.size() == 0 ? "+" : "+ ") : (it == this->_left.begin() && this->_equation.size() == 0 ? "-" : "- "))
					  << (it->value >= 0 ? it->value : -it->value)
					  << (it->degree != 0 ? " * X^" : "") << (it->degree != 0 ? to_string(it->degree) : "\0") << "\033[0m ";
	}

	std::cout << "\033[34m=\033[0m ";

	if (this->_right.size() == 0)
		std::cout << "\033[35m0\033[0m";
	else
	{
		for (std::vector<V>::iterator it = this->_right.begin(); it != this->_right.end(); it++)
			std::cout << "\033[33m" << (it->value >= 0 ? (it == this->_right.begin() ? "+" : "+ ") : (it == this->_right.begin() ? "-" : "- "))
					  << (it->value >= 0 ? it->value : -it->value)
					  << (it->degree != 0 ? " * X^" : "") << (it->degree != 0 ? to_string(it->degree) : "\0") << "\033[0m ";
	}

	std::cout << "\n";

	this->_steps += 1;
}

void Polynome::applyOperation(float value, float degree, bool display = false, bool force_display = false)
{
	std::map<float, float>::iterator key;

	key = this->_equation.find(degree);

	if (key != this->_equation.end())
	{
		key->second += value;
		if (display)
			this->display();
	}
	else
	{
		this->_equation.insert(std::make_pair(degree, value));
		if (display && force_display)
			this->display();
	}
}

void Polynome::simplify(bool display = false)
{
	std::vector<V>::iterator it;
	std::map<float, float>::iterator itm;
	float value;
	float degree;

	if (this->_error == true)
		return;

	if (display)
		this->display();

	while (this->_left.begin() != this->_left.end())
	{
		it = this->_left.begin();
		value = it->value;
		degree = it->degree;
		this->_left.erase(it);
		applyOperation(value, degree, display);
	}
	while (this->_right.begin() != this->_right.end())
	{
		it = this->_right.begin();
		value = -it->value;
		degree = it->degree;
		this->_right.erase(it);
		applyOperation(value, degree, display, true);
	}

	// Remove all 0 multiplication
	itm = this->_equation.begin();
	while (itm != this->_equation.end())
	{
		if (itm->second == 0)
		{
			this->_equation.erase(itm);
			itm = this->_equation.begin();
		}
		else
			itm++;
	}
}

void Polynome::resolutionDegreeTwo()
{
	float delta;
	float a;
	float b;
	float c;

	a = (this->_equation.find(2) != this->_equation.end() ? this->_equation.find(2)->second : 0);
	b = (this->_equation.find(1) != this->_equation.end() ? this->_equation.find(1)->second : 0);
	c = (this->_equation.find(0) != this->_equation.end() ? this->_equation.find(0)->second : 0);

	delta = b * b - 4 * a * c;

	if (a == 0)
	{
		this->_solution.insert(this->_solution.end(), -c / b);
	}
	else if (delta == 0)
	{
		this->_solution.insert(this->_solution.end(), -b / (2 * a));
	}
	else if (delta > 0)
	{
		this->_solution.insert(this->_solution.end(), (-b - sqrt(delta)) / (2 * a));
		this->_solution.insert(this->_solution.end(), (-b + sqrt(delta)) / (2 * a));
	}
}

void Polynome::resolve(bool display = false)
{
	float polynomialDegree = 0;
	std::map<float, float>::iterator key;

	if (this->_solution_set == true || this->_error == true)
		return;

	this->_solution_set = true;
	this->simplify(display);
	if (display)
		this->display("Reduced form: ");
	for (std::map<float, float>::iterator it = this->_equation.begin(); it != this->_equation.end(); it++)
	{
		if (it->first < 0)
		{
			if (display)
				std::cout << "Degree inferior to 0, can't resolve\n";
			return;
		}
		if (polynomialDegree < it->first)
			polynomialDegree = it->first;
	}

	if (display)
		std::cout << "Polynomial degree: " << polynomialDegree << "\n";
	if (polynomialDegree > 2)
	{
		if (display)
			std::cout << "The polynomial degree is strictly greater than 2, I can't solve.\n";
		return;
	}

	// Check if x is useful for the resolution
	key = this->_equation.find(2);
	if (key == this->_equation.end())
	{
		key = this->_equation.find(1);
		if (key == this->_equation.end())
		{
			key = this->_equation.find(0);
			if (display)
			{
				if (key == this->_equation.end())
					std::cout << "X ∈ R(i,Z,D,Q,C) everything in the universe\n";
				else
					std::cout << "This equation has 0 solution\n";
			}
			return;
		}
	}

	this->resolutionDegreeTwo();

	if (display)
		this->printSolution();
}

/******************************Solution management*******************************/
void Polynome::printSolution()
{
	if (this->_solution.size() == 0)
		std::cout << "No solution\n";
	else
	{
		std::cout << "Solutions: ";
		for (std::vector<float>::iterator it = this->_solution.begin(); it != this->_solution.end(); it++)
			std::cout << (*it == 0 ? 0 : *it) << (it + 1 == this->_solution.end() ? "\n" : ", ");
	}
}

std::vector<float> Polynome::getSolution(bool display = false)
{
	if (this->_solution_set == false)
		this->resolve(display);
	if (display)
		this->printSolution();
	return (this->_solution);
}
