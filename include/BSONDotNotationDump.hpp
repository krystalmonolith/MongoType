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

#ifndef BSONDOTNOTATIONDUMP_HPP_
#define BSONDOTNOTATIONDUMP_HPP_

#include <mongotype.hpp>
#include <Parameters.hpp>
#include <IBSONRenderer.hpp>
#include <BSONTypeMap.hpp>
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

class BSONDotNotationDump : virtual public IBSONRenderer, virtual protected IBSONObjectVisitor {
	Parameters& params;
	const BSONObj& object;
	deque<string> dotStack;
	function<ostream&()> getOStream; // std::function required to store a closure.

protected: // IBSONObjectVisitor overrides.

	virtual void onParseStart() { }

	virtual void onParseEnd() {	}

	virtual void onObjectStart(const BSONObj& object, int arrayIndex) {
		if (arrayIndex >= 0) { // If the object is an element of an array...
			 string s("[");
			 s += to_string(arrayIndex);
			 s += "]"; // Output an array index first.
			 dotStack.push_back(s);
		}
	}

	virtual void onObjectEnd(const BSONObj& object, int arrayIndex) {
		if (arrayIndex >= 0) { // If the object is an element of an array...
			dotStack.pop_back();
		}
	}

	virtual void onArrayStart(const BSONElement& element, int count) {
		string s(".");
		s += element.fieldName();
		 dotStack.push_back(s);
	}

	virtual void onArrayEnd(const BSONElement& element) {
		dotStack.pop_back();
	}

	virtual void onElement(const BSONElement& element, int arrayIndex) {
		BSONTypeMap type(element);
		string acc;
		for (string s: dotStack) {
			acc += s;
		}
		acc += '.';
		acc += string(element);
		acc += " ";
		acc += type.to_string();
		getOStream() << acc << "\n";
	}

public: // User Interface

	/*!
	 * \brief Construct a BSON object dumper.
	 * \param[in] pobject The BSON object to be dumped.
	 * \param[in] pindentStr The string used to indent the text output. The indent text is prepended to the output lines once for each indent level.
	 */
	BSONDotNotationDump(Parameters& pparams, const BSONObj& pobject, string& initialToken) : params(pparams), object(pobject) {
		dotStack.clear();
		dotStack.push_back(initialToken);
	};
	virtual ~BSONDotNotationDump() {};

	virtual std::ostream& render(std::ostream& os) {
		getOStream = [&] () -> ostream& { return os; }; // Wrap closure around ostream.
		BSONObjectParser objectParser(*this); // Construct a parser around this event handler.
		objectParser.parse(object);     // Parse the object and write the text output the the output stream.
		getOStream = NULL;
		return os;
	}

	/*!
	 * \brief BSON Object stream output operator.
	 * \param[in,out] out The std::ostream to write the BSON object dump.
	 * \param[in] bos The BSONObjectTypeDump object to write to the output stream.
	 */

	OSTREAM_FRIEND(BSONDotNotationDump& bos) {
		bos.render(out);
		return out;							// Required for stream chaining.
	}
};

} /* namespace mongotype */
#endif /* BSONDOTNOTATIONDUMP_HPP_ */
