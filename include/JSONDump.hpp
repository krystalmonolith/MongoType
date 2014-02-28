/*!
 * \file JSONDump.hpp
 * \brief JSON Dump Definitions
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

#ifndef JSONDUMP_HPP_
#define JSONDUMP_HPP_

#include <mongotype.hpp>
#include <Parameters.hpp>
#include <IBSONRenderer.hpp>
#include <BSONObjectParser.hpp>

namespace mongotype {

/*!
 * \brief BSON -> JSON Dump
 *
 * Provides an std::ostream comparable output operator for dumping a JSON representation of the given BSON object. JSONDump implements interface IBSONObjectVisitor and
 * uses the BSON object parsing events to output the BSON object's JSON representation.
 *
 * \see IBSONObjectVisitor, BSONObjectParser, IBSONRenderer
 */

class JSONDump : virtual public IBSONRenderer, virtual protected IBSONObjectVisitor {
	Parameters& params;
	const BSONObj& object;
	string indentStr;
	int level;
	function<ostream&()> getOStream; // std::function required to store a closure.

	void tstr(const char* token) {
		getOStream() << token;
		getOStream().flush(); // TEST!
	}

	void tstr(string& token) {
		tstr(token.c_str());
	}

	void istr(const char* token) {
		const string cr("\n");
		string indent;
		indent.reserve(1024);
		for (int i=0; i < level; i++) {
			indent += indentStr;
		}
		string s(cr + indent + token);
		tstr(s);
	}

	void istr(string& token) {
		istr(token.c_str());
	}

	void comma(const BSONObjectVisitorParams& vp) {
		if (vp.getElementIndex() > 0 && vp.getArrayIndex() != 0) {
			getOStream() << ',';
		}
		if (vp.getParent() != NULL) {
			string s(vp.getKey());
			s += ": ";
			istr(s);
		}
	}

protected: // IBSONObjectVisitor overrides.

	virtual void onParseStart() {
		level = 1; // Reset the indent level to one, render() outputs level zero array square brackets.
	}

	virtual void onParseEnd() {
		level--;
		if (params.isDebug() && level != 0) {
			string s("ISE: Terminal Parse Level Error! : level=");
			s += to_string(level);
			throw std::logic_error(s);
		}
	}

	virtual void onObjectStart(const BSONObjectVisitorParams& vparams, const BSONObj& object) {
		comma(vparams);
		tstr("{"); // Output a newline and indent.
		level++; // Increase the indent for the object's BSON elements.
	}

	virtual void onObjectEnd(const BSONObjectVisitorParams& vparams, const BSONObj& object) {
		level--; // Decrease the indent level after the object's elements.
		istr("}"); // Output a newline, indent, and bracket closing the object.
		const BSONObjectVisitorParams vp;
		comma(vp);
	}

	virtual void onArrayStart(const BSONObjectVisitorParams& vparams, const BSONElement& element) {
		comma(vparams);
		tstr("["); // Output a newline, indent, and bracket closing the object.
		level++; // Increase the indent for the array's BSON elements.
	}

	virtual void onArrayEnd(const BSONObjectVisitorParams& vparams, const BSONElement& element) {
		level--; // Decrease the indent after the array's BSON elements.
		istr("]"); // Output a newline, indent, and bracket closing the object.
		const BSONObjectVisitorParams vp;
		comma(vp);
	}

	virtual void onElement(const BSONObjectVisitorParams& vparams, const BSONElement& element) {
		comma(vparams);
		string s(element.toString(false,false));
		tstr(s); // Output newline, indent, element text.
	}

public: // User Interface

	/*!
	 * \brief Construct a BSON object dumper.
	 * \param[in] pobject The BSON object to be dumped.
	 * \param[in] pindentStr The string used to indent the text output. The indent text is prepended to the output lines once for each indent level.
	 */
	JSONDump(Parameters& pparams, const BSONObj& pobject, const char *pindentStr = " ") :
		params(pparams), object(pobject), indentStr(pindentStr), level(0) {}
	virtual ~JSONDump() {};

	virtual std::ostream& render(std::ostream& os) {
		getOStream = [&] () -> ostream& { return os; }; // Wrap closure around ostream.
		BSONObjectParser objectParser(*this); // Construct a parser around this event handler.
		tstr("[\n");
		objectParser.parse(object);     // Parse the object and write the text output the the output stream.
		tstr("]");
		getOStream = NULL;
		os << "\n";
		return os;
	}

	/*!
	 * \brief BSON Object stream output operator.
	 * \param[in,out] out The std::ostream to write the BSON object dump.
	 * \param[in] bos The JSONDump object to write to the output stream.
	 */

	OSTREAM_FRIEND(JSONDump& bos) {
		return bos.render(out);
	}
};

} /* namespace mongotype */
#endif /* JSONDUMP_HPP_ */
