
#pragma once

#include <map>
#include <vector>
#include <sstream>

typedef struct Value
{
	float value;
	float degree;
} V;

class Polynome
{
public:
	Polynome();
	Polynome(int argc, char **argv);
	Polynome(std::string input);
	~Polynome();

	// Display the current reduction of the equation
	void display(std::string prompt);

	// Simplify the equation to get an unique value for each degree
	void simplify(bool display);

	// Resolve the equation
	void resolve(bool display);

	// Print the solution
	void printSolution();

	// Return the solution
	std::vector<float> getSolution(bool display);

private:
	void parseInput(std::string &input);
	int parseNumber(std::string::iterator &it, std::string::iterator end, float &value, float &degree);
	void resolutionDegreeTwo();
	void applyOperation(float value, float degree, bool display, bool force_display);

	std::map<float, float> _equation;

	std::vector<V> _left;
	std::vector<V> _right;

	std::vector<float> _solution;
	bool _solution_set;
	bool _error;

	int _steps;
};

template <typename T>
std::string to_string(T Number)
{
	std::ostringstream ss;

	ss << Number;
	return ss.str();
}