#ifndef INTERP_H
#define INTERP_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "object.h"
#include "exception.h"
#include "exception_types.h"
#include "base_type_names.h"
#include "shunting-yard.h"
#include "reserved_keywords.h"
#include "variable.h"

class Interpreter
{
public:
	Interpreter(std::string sourceCode);

	void exec();
private:
	std::map<std::string, ObjectPtr> objects;

	std::vector<std::string> srcLines;

	bool varCreated(std::string varName, ObjectPtr &outObject)
	{
		std::map<std::string, ObjectPtr>::iterator it = objects.find(varName);
		if (it != objects.end())
		{
			outObject = it->second;
			return true;
		}

		return false;
	}

	bool isLegalIdentifierName(const std::string &varName, std::string &outMessage)
	{
		// check all reserved keywords
		int numKeywords = sizeof(keywords) / sizeof(std::string);
		for (int i = 0; i < numKeywords; i++)
		{
			if (varName == keywords[i]) {
				outMessage = "Reserved keyword.";
				return false;
			}
		}

		for (int i = 0; i < varName.length() - 1; i++) // do not count terminator character
		{
			char c = varName[i];
			if (i == 0)
			{
				// do not allow a variable to start with a number
				if (!(c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 95)) { // only allow uppercase or lowercase letters and underscores
					outMessage = "Must begin with either a capital letter, lowercase letter, or underscore.";
					return false;
				}
			}
			else
			{
				if (!(c >= 48 && c <= 57 || c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 95)) {
					outMessage = "Contains invalid characters.";
					return false;
				}
			}
		}

		return true;
	}

	void debugAssert(bool condition, int line, const BaseException &exception)
	{
		if (!condition)
			error(line, exception);
	}

	void error(int line, const BaseException &exception)
	{
		int tmp;
		std::cout << "\nError on line " << line <<
			":\nException type: " << exception.getExceptionType() <<
			"\nMessage:\n" <<
			"\t" << exception.getMessage() << "\n\nPress the return key to exit.\n";
		std::cin >> tmp;
		exit(1);
	}

	template <class BaseType, class ObjectType>
	typename std::enable_if<std::is_base_of<Variable<BaseType>, ObjectType>::value, bool>::type

		createIntegralVariable(const std::string &name, const std::string &data, int lineNum)
	{
		BaseType baseVariable = 0; // default value

		if (data != "")
		{
			vector<string> dataSplit = split(data, ' ');
			if (dataSplit.size() > 0) // make sure the string list is not null
			{
				if (dataSplit[0][0] == '&') // check to see if variable reference
				{
					// now we make sure that the variable exists
					string refVariableName = dataSplit[0].substr(1); // to cut off the '&'

					if (dataSplit.size() != 1) {
						error(lineNum, BadSyntaxException("Variable \"" + name + "\" not correctly initialized as a reference to \"" + refVariableName + "\""));
						return false;
					}

					ObjectPtr outObject = NULL;
					if (!varCreated(refVariableName, outObject))
						error(lineNum, UndeclaredException("The referenced variable \"" + refVariableName + "\" does not exist!"));
					else
					{
						// make sure the object is compatible with the variable type.
						std::shared_ptr<ObjectType> otherObjectCast = std::dynamic_pointer_cast<ObjectType>(outObject);
						if (otherObjectCast != NULL)
						{
							// initialize the object from reference
							ObjectPtr &newVariable = outObject;
							objects.insert(std::pair<string, ObjectPtr>(name, newVariable));

							return true;
						}
						else
							error(lineNum, CastException("The variable \"" + refVariableName + "\" is not directly compatible with the variable \"" + name + "\" as a reference."));
					}
				}
			}
			else
				return false;

			double ans = 0;
			try {
				map<string, double> expressionMap;

				// break up the expression
				string newExpression = "";
				vector<string> expressionSplit = splitExpression(data);
				for (int i = 0; i < expressionSplit.size(); i++)
				{
					if (isNumber(expressionSplit[i]))
						newExpression += expressionSplit[i] + " ";
					else if (isOperator(expressionSplit[i]))
						newExpression += expressionSplit[i] + " ";
					else if (expressionSplit[i] == "true" || expressionSplit[i] == "false")
						newExpression += expressionSplit[i];
					else
					{
						// lastly, check if a variable copy was used in the equation

						// search all available variable names to see if it is creating a copy
						string refVariableName = expressionSplit[i];
						std::shared_ptr<ObjectType> otherObjectCast = NULL;

						ObjectPtr outObject = NULL;
						if (varCreated(refVariableName, outObject))
						{
							// make sure the object is compatible with the variable type.
							string objType = outObject->getTypeInfo().getTypeName();
							//BaseType copyVar = 0;
							double copyVar = 0.0;
							bool completeCopy = false;

							// try to detect object
							if (objType == TYPE_INT) {
								if (std::shared_ptr<IntVariable> otherObjectCastInt = std::dynamic_pointer_cast<IntVariable>(outObject)) {
									if (copyVar = static_cast<double>(otherObjectCastInt->getData()))
										otherObjectCast = std::make_shared<ObjectType>(copyVar);
								}
							}
							else if (objType == TYPE_DOUBLE) {
								if (std::shared_ptr<DoubleVariable> otherObjectCastDouble = std::dynamic_pointer_cast<DoubleVariable>(outObject)) {
									if (copyVar = static_cast<double>(otherObjectCastDouble->getData()))
										otherObjectCast = std::make_shared<ObjectType>(copyVar);
								}
							}
							else if (objType == TYPE_FLOAT) {
								if (std::shared_ptr<FloatVariable> otherObjectCastFloat = std::dynamic_pointer_cast<FloatVariable>(outObject)) {
									if (copyVar = static_cast<double>(otherObjectCastFloat->getData()))
										otherObjectCast = std::make_shared<ObjectType>(copyVar);
								}
							}
							else if (objType == TYPE_BOOL) {
								if (std::shared_ptr<BoolVariable> otherObjectCastBool = std::dynamic_pointer_cast<BoolVariable>(outObject)) {
									if (copyVar = static_cast<double>(otherObjectCastBool->getData()))
										otherObjectCast = std::make_shared<ObjectType>(copyVar);
								}
							}
							else {
								otherObjectCast = std::dynamic_pointer_cast<ObjectType>(outObject); // try to cast by object finally
							}

							if (otherObjectCast != NULL)
							{
								expressionMap[refVariableName] = copyVar;//otherObjectCast->getData();
								newExpression += refVariableName + " ";
							}
							else
								error(lineNum, CastException("The variable \"" + refVariableName + "\" is not directly compatible with the variable \"" + name + "\""));
						}
					}
				}
				ans = calculator::calculate(newExpression.c_str(), expressionMap.size() > 0 ? &expressionMap : 0);
				baseVariable = static_cast<BaseType>(ans);
				cout << name << ": " << baseVariable << "\n";
				cout << "EXPRESSION: " << newExpression << "\n";
				// initialize the object from expression
				ObjectPtr newVariable = std::make_shared<ObjectType>(baseVariable);
				objects.insert(std::pair<string, ObjectPtr>(name, newVariable));
			}
			catch (...) {
				return false;
			}
		}
		else if (data == "")
		{
			// create the object with default constructor
			ObjectPtr newVariable = std::make_shared<ObjectType>(baseVariable);
			objects.insert(std::pair<string, ObjectPtr>(name, newVariable));
		}

		return true;
	}
};

#endif