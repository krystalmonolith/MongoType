/*!
 * \file BSONObjectParser.hpp
 * \brief BSON Object Visitor-Parser Definitions
 *
 * \author Mark Deazley &lt;mdeazley@gmail.com&gt;
 *
 * \copyright Copyright &copy; 2013 by Mark Deazley<br/><br/>
 *
 * <b>License:</b> <i>Free Software Foundation’s GNU AGPL v3.0.</i><br/>
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

#ifndef BSONOBJECTPARSER_HPP_
#define BSONOBJECTPARSER_HPP_

//----------------------------------------------------------------------------

#include <mongotype.hpp>

//----------------------------------------------------------------------------

namespace mongotype {

//----------------------------------------------------------------------------

/**
 * \class BSONParserStackItem
 * \brief Container class for pointers to ::mongo::BSONObj and ::mongo::BSONElement.
 *
 * Implemented because the ::mongo::BSONObj and ::mongo::BSONElement do not have common base class. This class implements a tagged union to store the BSON object pointers.
 */

class BSONParserStackItem {
public:
	/**
	 * \enum ItemType The BSON type of the contained object.
	 */
	enum ItemType {
		OBJECT, 	/*!< The contained object is mongo::BSONObj. */
		ARRAY, 		/*!< The contained object is mongo::BSONElement array. */
		ELEMENT 	/*!< The contained object is mongo::BSONElement scalar. */
	};

private:
	/**
	 * \var type The contained BSON object''s type.
	 */
	ItemType type;

	/**
	 * \union Item Common BSON object pointer storage.
	 */

	union Item {
		/**
		 * \var object Reference to the ::mongo::BSONObj
		 */
		const BSONObj* object;
		/**
		 * \fn Item(const BSONObj* pobject)
		 * \brief Construct the enum with a ::mongo::BSONObj.
		 * \param pobject The pointer to the ::mongo::BSONObj.
		 */
		Item(const BSONObj* pobject) : object(pobject) {}
		/**
		 * \var element Reference to a mongo::BSONElement.
		 */
		const BSONElement* element;
		/**
		 * \fn Item(const BSONElement* pelement)
		 * \brief Construct the enum with a ::mongo::BSONElement.
		 * \param pelement The pointer to the ::mongo::BSONElement.
		 */
		Item(const BSONElement* pelement) : element(pelement) {}
	} item;

	/*
	 * \var Name of the BSON object/array/element, or the empty string if this is the root object.
	 */
	const string& key;

	/*
	 * \var The zero based index of the BSON object/array/element within the parent object.
	 */
	int elementIndex;

	/*
	 * \var The count of all the BSON object(s)/array(s)/element(s) within the parent object.
	 */
	int elementCount;

	/*
	 * \var The zero based index of BSON element within the containing BSON array, or -1 if the BSON element is not contained within a BSON array.
	 * <ul><li>arrayIndex == -1 if object is not contained in an array.</li><li>arrayIndex >= 0 if object is contained within an array.</li></ul>
	 */
	int arrayIndex;

	/*
	 * \var The count of all BSON element(s) within the containing BSON array, or 0 if the BSON element is not contained within a BSON array.
	 * <ul><li>arrayCount == 0 if object is not contained in an array.</li><li>arrayCount > 0 if object is contained within an array.</li></ul>
	 */
	int arrayCount;

private:
	/**
	 * \fn validate Throw an error if the wrong type of fetch from the union occurs.
	 * \param t The ItemType being fetched.
	 * \throws std::logic_error Thrown if the passed type does not match the stored type.
	 */
	void validate(ItemType t) const {
		if (t != type) {
			throw std::logic_error( string("Illegal Stack Item Type Access: ") + (t == OBJECT ? string("OBJECT") : string("ELEMENT")) );
		}
	}

public:
	/**
	 * \fn BSONParserStackItem(const BSONObj* object)
	 * \brief Construct a BSONParserStackItem containing a pointer to a mongo::BSONObj.
	 * \param object The pointer to the mongo::BSONObj.
	 */
	BSONParserStackItem(const BSONObj* object, const string& pkey, int pelementIndex, int pelementCount, int parrayIndex, int parrayCount)
		: type(OBJECT), item(object), key(pkey), elementIndex(pelementIndex), elementCount(pelementCount), arrayIndex(parrayIndex), arrayCount(parrayCount) {}
	/**
	 * \fn BSONParserStackItem(const BSONElement* element)
	 * \brief Construct a BSONParserStackItem containing a pointer to a mongo::BSONElement.
	 * \param element The pointer to the mongo::BSONElement.
	 */
	BSONParserStackItem(ItemType ptype, const BSONElement* element, const string& pkey, int pelementIndex, int pelementCount, int parrayIndex, int parrayCount)
		: type(ptype), item(element), key(pkey), elementIndex(pelementIndex), elementCount(pelementCount), arrayIndex(parrayIndex), arrayCount(parrayCount) {}

	ItemType getType() const {
		return type;
	}

	const BSONObj& getObject() const {
		validate(OBJECT);
		return *item.object;
	}

	const BSONElement& getElement() const {
		validate(ELEMENT);
		return *item.element;
	}

	const string& getKey() const {
		return key;
	}

	int getElementIndex() const {
		return elementIndex;
	}

	int getElementCount() const {
		return elementCount;
	}

	int getArrayIndex() const {
		return arrayIndex;
	}

	int getArrayCount() const {
		return arrayCount;
	}

	string toString() const {
		string s("{");
		switch (type) {
		case OBJECT:
			s += "OBJECT";
			break;
		case ARRAY:
			s += "ARRAY";
			break;
		case ELEMENT:
			s += "ELEMENT";
			break;
		default:
			throw std::logic_error(string("toString Undefined ItemType!"));
		}
		s += ",\"" + key + "\"",
		s += "," + to_string(elementIndex);
		s += "," + to_string(elementCount);
		s += "," + to_string(arrayIndex);
		s += "," + to_string(arrayCount);
		s += "}";
		return s;
	}
};

//----------------------------------------------------------------------------

/**
 * \class BSONParserStack
 * \brief Stack of BSONParserStackItem for storing the BSONObjectParser parse context.
 *
 * Stores the current state of the parsed BSON objects in a FILO data structure.
 *
 * \note "TOS" == "Top Of Stack"<br/>"FILO" == "First-In-Last-Out"
 */

class BSONParserStack {
	typedef std::deque<std::unique_ptr<const BSONParserStackItem>> Stack;

	Stack stack;

public:
	int depth() const { return stack.size(); }

private:
	void throwCount(int count) const {
		if (depth() < count) {
			throw std::logic_error( string("ISE: Insufficient BSONParserStack Stack Entries:(" + to_string(count)) + "," + to_string(depth()) + ")" );
		}
	}

protected:

	/**
	 * \brief Pop the TOS item.
	 * \return The pointer to the read-only stack item.
	 * \throws std::logic_error On stack underflow.
	 * \note Releases item ownership to caller, i.e., the called is now responsible for freeing the returned item.
	 */
	const BSONParserStackItem* pop() {
		throwCount(1);
		const BSONParserStackItem* rv = stack.back().release();
		stack.pop_back();
		return rv;
	}

	/**
	 * \brief Push the item.
	 * \param[in] item The bare pointer to the item to be stored on the stack.
	 * \note Takes item ownership from caller, i.e., if not first popped the contained item(s) will be destroyed and freed when the BSONParserStack instance is destroyed .
	 */
	void push(const BSONParserStackItem* item) {
		stack.push_back(unique_ptr<const BSONParserStackItem>(item));
	}

public:
	/**
	 * \param[in] index Zero based index of the stack item where zero is the first item pushed. If negative the items are referenced from the top of the stack, ie.:
	 * \li -1 item at top of stack.
	 * \li -2 1st item below top of stack.
	 * \li -3 2ed item below top of stack.
	 * \li etc...
	 * \return The pointer to the read-only stack item.
	 * \throws std::logic_error On stack underflow.
	 * \note Item ownership unaffected.
	 */
	const BSONParserStackItem& item(int index) const {
		if (index >= 0) {
			throwCount(index+1);
			return *(stack[index].get());
		} else {
			int i = depth() + index;
			return item(i);
		}
	}

	/**
	 * \brief Return the TOS item, leaving it in place.
	 * \return The pointer to the read-only stack item.
	 * \throws std::logic_error On stack underflow.
	 * \note Item ownership unaffected.
	 */
	const BSONParserStackItem& top() const { // Item ownership unaffected.
		return item(depth()-1);
	}

	/**
	 * \brief Push the BSON object.
	 * \param[in] object The bare pointer to the BSON object to be encapsulated in a BSONParserStackItem object and pushed on the stack.
	 * \see void BSONParserStack::push(const BSONParserStackItem* item)
	 */
	void push(const BSONObj& object, const string& key = string(), int elementIndex=0, int elementCount=1, int arrayIndex=-1, int arrayCount=0) {
		push(new BSONParserStackItem(&object, key, elementIndex, elementCount, arrayIndex, arrayCount));
	}

	/**
	 * \brief Push the BSON element.
	 * \param[in] element The bare pointer to the BSON element to be encapsulated in a BSONParserStackItem object and pushed on the stack.
	 * \see void BSONParserStack::push(const BSONParserStackItem* item)
	 */
	void push(BSONParserStackItem::ItemType type, const BSONElement& element, const string& pkey = string(), int pelementIndex=0, int pelementCount=1, int parrayIndex=-1, int parrayCount=0) {
		push(new BSONParserStackItem(type, &element, pkey, pelementIndex, pelementCount, parrayIndex, parrayCount));
	}

	/**
	 * \brief Drop the TOS.
	 * \note TOS item is destroyed.
	 */
	void drop() {
		delete pop();
	}

	string toString() const {
		string s("<<");
		for (int i=0; i < depth(); i++) {
			if (i > 0) s += ",";
			s += item(i).toString();
		}
		s += ">>";
		return s;
	}
};

//----------------------------------------------------------------------------

/*!
 * \interface IBSONObjectVisitor
 * \brief Visitor interface for parsing nested BSON data objects.
 *
 * \b Usage:
 * <ol>
 * <li>Derive a class <i>V</i> from interface IBSONObjectVisitor and implement each onX() function.</li>
 * <li>Instantiate an instance of V called v.
 * <li>Construct a BSONObjectParser object <i>p</i> using the instance of the derived class <i>v</i> as the parameter to constructor BSONObjectParser::BSONObjectParser(IBSONObjectVisitor&).
 * <li>Parse a BSON object (mongo::BSONobj) <i>o</i> using virtual void BSONObjectParser::parse(const BSONObj& <i>o</i>), i.e., <i>p</i>.parse(<i>o</i>).</li>
 * </ol>
 */

class IBSONObjectVisitor {
public:
	virtual ~IBSONObjectVisitor() {}

	/*!
	 * \fn void onParseStart()
	 * \brief Parser Construction Event
	 *
	 * Invoked once each parse before all other events. Used to construct the parse state.
	 */
	virtual void onParseStart() = 0;
	/*!
	 * \fn void onParseEnd()
	 * \brief Parser Destruction Event
	 *
	 * Invoked once each parse after all other events. Used to destroy the parse state.
	 */
	virtual void onParseEnd() = 0;

	/*!
	 * \fn void onObjectStart(const BSONParserStack& stack)
	 * \brief BSON Object Precursor Event
	 * \param[in] stack The \ref BSONParserStack object containing the current parse context.
	 *
	 * Invoked once per each non-terminal BSON object before parsing the contained BSON elements.
	 */
	virtual void onObjectStart(const BSONParserStack& stack) = 0;
	/*!
	 * \fn void onObjectEnd(const BSONParserStack& stack)
	 * \brief BSON Object Successor Event
	 * \param[in] stack The \ref BSONParserStack object containing the current parse context.
	 *
	 * Invoked once per each non-terminal BSON object after parsing the contained BSON elements.
	 */
	virtual void onObjectEnd(const BSONParserStack& stack) = 0;

	/*!
	 * \fn void onArrayStart(const BSONParserStack& stack)
	 * \brief BSON Array Precursor Event
	 * \param[in] stack The \ref BSONParserStack object containing the current parse context.
	 *
	 * Invoked once per each non-terminal BSON array before parsing the contained BSON elements.
	 */
	virtual void onArrayStart(const BSONParserStack& stack) = 0;
	/*!
	 * \fn void onArrayEnd(const BSONParserStack& stack)
	 * \brief BSON Array Successor Event
	 * \param[in] stack The \ref BSONParserStack object containing the current parse context.
	 *
	 * Invoked once per each non-terminal BSON array after parsing the contained BSON elements.
	 */
	virtual void onArrayEnd(const BSONParserStack& stack) = 0;

	/*!
	 * \fn void onElement(const BSONParserStack& stack)
	 * \brief BSON Element Event
	 * \param[in] stack The \ref BSONParserStack object containing the current parse context.
	 *
	 * Invoked once per each terminal BSON element that is not a BSON object or a BSON array.
	 */
	virtual void onElement(const BSONParserStack& stack) = 0;
};

//----------------------------------------------------------------------------

/*!
 * \class BSONObjectParser
 * \brief The BSON Parser.
 *
 * This class is is used to tear apart a given BSON object and process its data content.
 *
 * It is intended to be fully extensible as the internal methods are all declared protected.
 *
 * \b Usage:
 * \see BSONObjectParser::parse, IBSONObjectVisitor, BSONParserStack, BSONParserStackItem
 */

class BSONObjectParser {

protected:
	/*!
	 * Visitor implementation that receives the parse events.
	 */
	IBSONObjectVisitor& visitor;
	BSONParserStack stack;

	//----------------------------------------------------------------------------

	/*!
	 * \brief Recursively parse a BSONElement
	 * \param[in] element The element to recursively dump.
	 *
	 * <ul>
	 * <li>If it is a BSON object (BSONObj) dump it via indirect recursion by calling parseObjectRecursive().</li>
	 * <li>If it is a BSON array iterate through the contained BSONElement(s) and dump via direct recursion by calling parseElementRecursive().</li>
	 * <li>If it is neither an object nor an array simply dump the BSONElement by printing its key, value, and BSON data type.</li>
	 * </ul>
	 */

	virtual void parseElementRecursive(const BSONElement& element, string& key, int elementIndex=0, int elementCount=1, int arrayIndex = -1, int arrayCount = 0) {
		BSONType btype = element.type();
		switch (btype) {
		case BSONType::Object:
			{
				const BSONObj& bobj = element.Obj();
				string k(element.fieldName());
				parseObjectRecursive(bobj, k, elementIndex, elementCount, arrayIndex, arrayCount);
			}
			break;
		case BSONType::Array:
			{
				stack.push(BSONParserStackItem::ItemType::ARRAY, element, key, elementIndex, elementCount, arrayIndex, arrayCount);
				std::vector<BSONElement> elementArray = element.Array();
				int elementArrayCount = elementArray.size();
				visitor.onArrayStart(stack);
				int elementArrayIndex = 0;
				for (BSONElement e : elementArray) {
					string k(e.fieldName());
					parseElementRecursive(e, k, elementIndex, elementCount, elementArrayIndex++, elementArrayCount);
				}
				visitor.onArrayEnd(stack);
				stack.drop();
			}
			break;
		default:
			{
				stack.push(BSONParserStackItem::ItemType::ELEMENT, element, key, elementIndex, elementCount, arrayIndex, arrayCount);
				visitor.onElement(stack);
				stack.drop();
			}
			break;
		}
	}

	//----------------------------------------------------------------------------

	/*!
	 * \brief Recursively parse a BSON Object.
	 * \param[in] key The key string of the object to dump.
	 * \param[in] object The BSON object to dump.
	 * \param[in] elementIndex The index of the object's BSONElement within its parent object.
	 * \param[in] elementCount The count of BSONElement(s) within the parent object.
	 * \param[in] arrayIndex The array index of the BSON object being parsed.
	 * <ul><li>arrayIndex == -1 if the element is not contained in an array.</li><li>arrayIndex >= 0 if the element is contained within an array.</li></ul>
	 *
	 * Iterate through all the BSONElement(s) contained in the BSONObj and dump them via indirect recursion by calling parseElementRecursive().
	 */

	virtual void parseObjectRecursive(const BSONObj& object, string& key, int elementIndex=0, int elementCount=1, int arrayIndex = -1, int arrayCount = 0) {
		stack.push(object, key, elementIndex, elementCount, arrayIndex, arrayCount);
		visitor.onObjectStart(stack);
		set<string> keys;
		object.getFieldNames(keys); // Get the key names of the BSON object.
		int ei = 0;
		int ec = keys.size();
		for (string key : keys) {
			BSONElement e = object.getField(key);
			parseElementRecursive(e, key, ei++, ec, arrayIndex);
		}
		visitor.onObjectEnd(stack);
		stack.drop();
	}

public:
	//----------------------------------------------------------------------------

	/*!
	 * \brief Construct a BSON Object parser
	 * \param[in] pvisitor The instance of the IBSONObjectVisitor visitor subclass that will receive the parse events.
	 *
	 * Construct a parser and register the parsing event handler/visitor.
	 */

	BSONObjectParser(IBSONObjectVisitor& pvisitor) : visitor(pvisitor) {}
	virtual ~BSONObjectParser() {}

	//----------------------------------------------------------------------------

	/*!
	 * \brief Parse a BSON Object (BSONObj)
	 * \param[in] object The BSON object to parse.
	 *
	 * Parse the given BSON object and invoke the event handlers of the IBSONObjectVisitor visitor \i subclass registered by constructor BSONObjectParser::BSONObjectParser(IBSONObjectVisitor& pvisitor).
	 */

	virtual void parse(const BSONObj& object) {
		visitor.onParseStart();
		string emptyString("");
		parseObjectRecursive(object, emptyString);
		visitor.onParseEnd();
	}

	//----------------------------------------------------------------------------
};

//----------------------------------------------------------------------------

} /* namespace mongotype */

//----------------------------------------------------------------------------

#endif /* BSONOBJECTPARSER_HPP_ */
