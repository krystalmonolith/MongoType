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
	string indentStr;

	function<ostream&()> getOStream; // std::function required to store a closure.

	// Output Helper Functions
	// tstr functions centralize token output primarily for debugging purposes.
	// istr emit a carriage return, indentation whitespace, and a token

	void tstr(const char* token) {
		getOStream() << token;
		if (params.isDebug()) getOStream().flush(); // "Token Buffering" on debug.
	}

	void tstr(string& token) {
		tstr(token.c_str());
	}

	void istr(const char* token, int level) {
		const string cr("\n");
		string indent;
		indent.reserve(1024);
		for (int i=0; i < level; i++) {
			indent += indentStr;
		}
		string s(cr + indent + token);
		tstr(s);
	}

	void istr(string& token, int level) {
		istr(token.c_str(), level);
	}

	/**
	 * Emit a comma.
	 */
	void emitComma(const BSONParserStack& stack) {
		const int ARRAY_PARENT_OFFSET = 2;
		if (stack.depth() >= ARRAY_PARENT_OFFSET) {
			const BSONParserStackItem& parent = stack.item(-ARRAY_PARENT_OFFSET);
			bool parentIsNotArray = parent.getType() != BSONParserStackItem::ItemType::ARRAY;
			//if (stack.top().getElementIndex() > 0 && parentIsNotArray) {
			if (stack.top().getElementIndex() > 0 && (parentIsNotArray || stack.top().getArrayIndex() > 0)) {
				tstr(",");
			}
		}
	}

	void emitKey(const BSONParserStack& stack) {
		const int ARRAY_PARENT_OFFSET = 2;
		bool parentIsNotArray = true;
		if (stack.depth() >= ARRAY_PARENT_OFFSET) {
			const BSONParserStackItem& parent = stack.item(-ARRAY_PARENT_OFFSET);
			parentIsNotArray = parent.getType() != BSONParserStackItem::ItemType::ARRAY;
		}
		string s;
		if (stack.depth() > 1 && parentIsNotArray) {
			s += "\"";
			s += stack.top().getKey();
			s += "\" : ";
		}
		istr(s, stack.depth());
	}

	/**
	 * Emit a comma and/or object label based on output state.
	 */
	void nextLine(const BSONParserStack& stack) {
		emitComma(stack);
		if (params.isStackDebug()) {
			tstr(string(("  ") + stack.toString()).c_str());
		}
		emitKey(stack);
	}

protected: // IBSONObjectVisitor overrides. ---------------------------------------------------------------------------

	virtual void onParseStart() {
	}

	virtual void onParseEnd() {
	}

	virtual void onObjectStart(const BSONParserStack& stack) {
		nextLine(stack);
		tstr("{"); // Begin the JSON object
	}

	virtual void onObjectEnd(const BSONParserStack& stack) {
		istr("}", stack.depth()); // End the JSON object.
	}

	virtual void onArrayStart(const BSONParserStack& stack) {
		nextLine(stack);
		tstr("["); // Begin the JSON array.
	}

	virtual void onArrayEnd(const BSONParserStack& stack) {
		istr("]", stack.depth()); // End the JSON array.
	}

	virtual void onElement(const BSONParserStack& stack) {
		nextLine(stack);
		// TODO: Some element values produced by element.toString(..) need quoting for JSON.
		string s(stack.top().getElement().toString(false,false));
		tstr(s); // Output element value.
	}

public: // User Interface ---------------------------------------------------------------------------------------------

	/*!
	 * \brief Construct a BSON object dumper.
	 * \param[in] pparams The command line parameters object.
	 * \param[in] pindentStr The string used to indent the text output. The indent text is prepended to the output lines once for each indent level.
	 */
	JSONDump(Parameters& pparams, const char *pindentStr = " ") :
		params(pparams), indentStr(pindentStr) {}

	virtual ~JSONDump() {};

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
		tstr("[");
	}

	/*
	 * \param[in] suffix The string to be output after the object is rendered, or NULL.
	 */
	virtual void end(const char* suffix) {
		tstr("\n]");
	}

	/*
	 * \param[in] pobject The BSON object to be dumped.
	 * \param[in] pobject The output stream.
	 */
	virtual void render(const BSONObj& object, int docIndex, int docCount) {
		if (docIndex > 0) {
			tstr(",");
		}
		BSONObjectParser objectParser(*this); // Construct a parser around this event handler.
		objectParser.parse(object);     // !!! MAJOR ACTION HERE !!! >>> Parse the object and write to the output stream.
	}
};

} /* namespace mongotype */
#endif /* JSONDUMP_HPP_ */
