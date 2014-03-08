/*!
 * \file BSONObjectTypeDump.hpp
 * \brief BSON Object Type Stream Definitions
 *
 * \author Mark Deazley &lt;mdeazley@gmail.com&gt;
 * \copyright Copyright &copy; 2013 by Mark Deazley<br/><br/>
 *
 *   <b>License:</b> <i>Free Software Foundation’s GNU AGPL v3.0.</i><br/>
 *
 * This program is free software: you can redistribute it and/or modify<br/>
 * it under the terms of the GNU Affero General Public License as<br/>
 * published by the Free Software Foundation, either version 3 of the<br/>
 * License, or (at your option) any later version.<br/>
 *
 * This program is distributed in the hope that it will be useful,<br/>
 * but WITHOUT ANY WARRANTY; without even the implied warranty of<br/>
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br/>
 * GNU Affero General Public License for more details.<br/>
 *
 * You should have received a copy of the GNU Affero General Public License<br/>
 * along with this program.  If not, see http://www.gnu.org/licenses/ .<br/>
 * 
 * Creation Date: Oct 30, 2013
 * Eclipse Project: MongoType
 * 
 */

//----------------------------------------------------------------------------

#ifndef BSONOBJECTTYPEDUMP_HPP_
#define BSONOBJECTTYPEDUMP_HPP_

#include <mongotype.hpp>
#include <Parameters.hpp>
#include <IBSONRenderer.hpp>
#include <BSONTypeFormatter.hpp>
#include <BSONObjectParser.hpp>

namespace mongotype {

/*!
 * \brief Human readable BSON Object Dump
 *
 * Provides an std::ostream comparable output operator for dumping a human readable text
 * version of the given BSON object. BSONObjectTypeDump implements interface IBSONObjectVisitor and
 * uses the BSON object parsing events to output the BSON object's text representation.
 *
 * \see IBSONObjectVisitor, BSONObjectParser
 */

class BSONObjectTypeDump : virtual public IBSONRenderer, virtual protected IBSONObjectVisitor {
	Parameters& params;
	string indentStr;
	string initialToken;
	int level;
	function<ostream&()> getOStream; // std::function required to store a closure.

	string istr() {
		string s;
		for (int i=0; i<level; i++) {
			s += indentStr;
		}
		return s;
	}

protected: // IBSONObjectVisitor overrides.

	virtual void onParseStart() {
		level = 0; // Reset the indent level to zero.
	}

	virtual void onParseEnd() {	}

	virtual void onObjectStart(const BSONParserStack& stack) {
		getOStream() << "\n" << istr(); // Output a newline and indent.
		if (stack.top().getArrayIndex() >= 0) { // If the object is an element of an array...
			getOStream() << "[" << stack.top().getArrayIndex() << "]: "; // Output an array index first.
		}
		getOStream() << "{"; // Output the opening bracket for the object.
		level++; // Increase the indent for the object's BSON elements.
	}

	virtual void onObjectEnd(const BSONParserStack& stack) {
		level--; // Decrease the indent level after the object's elements.
		getOStream() << "\n" << istr() << "}"; // Output a newline, indent, and bracket closing the object.

	}

	virtual void onArrayStart(const BSONParserStack& stack) {
		getOStream() << " {ARRAY[" << stack.top().getArrayCount() << "]}"; // Output the count of array elements.
		level++; // Increase the indent for the array's BSON elements.
	}

	virtual void onArrayEnd(const BSONParserStack& stack) {
		level--; // Decrease the indent after the array's BSON elements.
	}

	virtual void onElement(const BSONParserStack& stack) {
		const BSONElement& element = stack.top().getElement();
		BSONTypeFormatter type(params, element);
		getOStream() << "\n" << istr() << element << " " << type; // Output newline, indent, element text, element type text.
	}

public: // User Interface

	/*!
	 * \brief Construct a BSON object dumper.
	 * \param[in] pparams The command line parameters.
	 * \param[in] pinitialToken The string the prefixes all output lines.
	 * \param[in] pindentStr The string used to indent the text output. The indent text is prepended to the output lines once for each indent level.
	 */
	BSONObjectTypeDump(Parameters& pparams, string& pinitialToken, const char *pindentStr = " ") :
		params(pparams), indentStr(pindentStr), initialToken(pinitialToken), level(0) {}

	virtual ~BSONObjectTypeDump() {};

	/*
	 * \param[in] os The output stream to which the object(s) are rendered.
	 */
	virtual void setOutputStream(std::ostream& os) {
		getOStream = [&] () -> ostream& { return os; }; // Wrap closure around ostream.
	}

	/*
	 * \param[in] prefix The string to be output before the object is rendered, or NULL.
	 */
	virtual void begin(const char* prefix) {
		if (prefix != NULL) {
			getOStream() << prefix;
		}
	}

	/*
	 * \param[in] suffix The string to be output after the object is rendered, or NULL.
	 */
	virtual void end(const char* suffix) {
		if (suffix != NULL) {
			getOStream() << suffix;
		}
	}

	virtual void render(const BSONObj& object, int docIndex, int docCount) {
		getOStream() << "\n" << initialToken << " =>";
		BSONObjectParser objectParser(*this); // Construct a parser around this event handler.
		objectParser.parse(object);     // Parse the object and write the text output the the output stream.
	}
};

} /* namespace mongotype */
#endif /* BSONOBJECTTYPEDUMP_HPP_ */
