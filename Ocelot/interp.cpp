#include "interp.h"
#include "strutil.h"
#include "variable.h"
#include "base_type_names.h"
#include "exception_types.h"

#include <iostream>
#include <memory>

Interpreter::Interpreter(std::string sourceCode)
{
	// Split code by semi-colons
	this->srcLines = codeSplit(sourceCode);
}

void Interpreter::exec()
{
	for (int i = 0; i < srcLines.size(); i++)
	{
		std::string line = srcLines[i];

		// split by spaces
		std::vector<std::string> tokens = splitQuotes(line, ' ');

		if (tokens.size() > 0)
		{
			// scan to see if it is creating an instance of a base integral type
			if (tokens[0] == TYPE_INT)
			{
				std::string varName = tokens.at(1);

				std::string reason = "";
				bool legalName = isLegalIdentifierName(varName, reason);
				debugAssert(legalName, i, BadSyntaxException("Illegal name for identifier: \"" + varName + "\": " + reason));

				ObjectPtr tmp = NULL;
				debugAssert(!varCreated(varName, tmp), i, AlreadyDeclaredException("The variable \"" + varName + "\" has already been declared!"));

				std::string valueSubString;

				if (tokens.size() > 2)
				{
					debugAssert(tokens.at(2) == "=", i, BadSyntaxException("Syntax error: \"" + line + "\""));

					// evaluate answer
					int indexOfValue = line.find(tokens.at(3));
					valueSubString = line.substr(indexOfValue);
				}
				else
					valueSubString = "";

				if (!createIntegralVariable<int, IntVariable>(varName, valueSubString, i))
				{
					FormattingException exception("The parser encountered an error parsing the line: \"" + line + "\"");
					this->error(i, exception);
				}

			}
			else if(tokens[0] == TYPE_DOUBLE)
			{
				std::string varName = tokens.at(1);

				std::string reason = "";
				bool legalName = isLegalIdentifierName(varName, reason);
				debugAssert(legalName, i, BadSyntaxException("Illegal name for identifier: \"" + varName + "\": " + reason));

				ObjectPtr tmp = NULL;
				debugAssert(!varCreated(varName, tmp), i, AlreadyDeclaredException("The variable \"" + varName + "\" has already been declared!"));

				std::string valueSubString;

				if (tokens.size() > 2)
				{
					debugAssert(tokens.at(2) == "=", i, BadSyntaxException("Syntax error: \"" + line + "\""));
					
					// evaluate answer
					int indexOfValue = line.find(tokens.at(3));
					valueSubString = line.substr(indexOfValue);
				}
				else
					valueSubString = "";

				if (!createIntegralVariable<double, DoubleVariable>(varName, valueSubString, i))
				{
					FormattingException exception("The parser encountered an error parsing the line: \"" + line + "\"");
					this->error(i, exception);
				}

			}
			else if (tokens[0] == TYPE_FLOAT)
			{
				std::string varName = tokens.at(1);

				std::string reason = "";
				bool legalName = isLegalIdentifierName(varName, reason);
				debugAssert(legalName, i, BadSyntaxException("Illegal name for identifier: \"" + varName + "\": " + reason));

				ObjectPtr tmp = NULL;
				debugAssert(!varCreated(varName, tmp), i, AlreadyDeclaredException("The variable \"" + varName + "\" has already been declared!"));

				std::string valueSubString;

				if (tokens.size() > 2)
				{
					debugAssert(tokens.at(2) == "=", i, BadSyntaxException("Syntax error: \"" + line + "\""));

					// evaluate answer
					int indexOfValue = line.find(tokens.at(3));
					valueSubString = line.substr(indexOfValue);
				}
				else
					valueSubString = "";

				if (!createIntegralVariable<float, FloatVariable>(varName, valueSubString, i))
				{
					FormattingException exception("The parser encountered an error parsing the line: \"" + line + "\"");
					this->error(i, exception);
				}

			}
			else if (tokens[0] == TYPE_BOOL)
			{
				std::string varName = tokens.at(1);

				std::string reason = "";
				bool legalName = isLegalIdentifierName(varName, reason);
				debugAssert(legalName, i, BadSyntaxException("Illegal name for identifier: \"" + varName + "\": " + reason));

				ObjectPtr tmp = NULL;
				debugAssert(!varCreated(varName, tmp), i, AlreadyDeclaredException("The variable \"" + varName + "\" has already been declared!"));

				std::string valueSubString;

				if (tokens.size() > 2)
				{
					debugAssert(tokens.at(2) == "=", i, BadSyntaxException("Syntax error: \"" + line + "\""));

					// evaluate answer
					int indexOfValue = line.find(tokens.at(3));
					valueSubString = line.substr(indexOfValue);
				}
				else
					valueSubString = "";

				if (!createIntegralVariable<bool, BoolVariable>(varName, valueSubString, i))
				{
					FormattingException exception("The parser encountered an error parsing the line: \"" + line + "\"");
					this->error(i, exception);
				}
				
			}
		}
	}


	// clean up
	cout << "\nCleaning up...\n";
	for (auto &i : objects)
	{
		ObjectPtr &obj = objects[i.first];

		if (obj != NULL)
		{
			cout << "Found variable:\n";
			cout << "\tName: " << i.first << "\n";
			cout << "\tType: " << i.second->getTypeInfo().getTypeName() << "\n";
			cout << "\tSize: " << sizeof(i.second) << "\n";
			cout << "\tUse Count: " << i.second.use_count() << "\n";

			cout << "\n";

			obj.reset();
			obj = NULL;
		}
	}
	objects.clear();
}