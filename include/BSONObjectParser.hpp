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

/*
 * \class BSONObjectVisitorParams
 * \brief Immutable visitor interface parameter container class.
 *
 * Used to pass the parameters common to all parsing events.
 */

class BSONObjectVisitorParams {
	/*
	 * \var Name of the BSON object/array/element, or the empty string if this is the root object.
	 */
	const string& key;

	/*
	 * \var  The parent BSONObj that contains the object/array/element, or NULL if this is the root object.
	 */
	const BSONObj* parent;

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

public:

	BSONObjectVisitorParams(const string& pkey = string(), const BSONObj* pparent = NULL, int pelementIndex=0, int pelementCount=1, int parrayIndex=-1, int parrayCount=0) :
		key(pkey), parent(pparent), elementIndex(pelementIndex), elementCount(pelementCount), arrayIndex(parrayIndex), arrayCount(parrayCount) { }

	BSONObjectVisitorParams(const BSONObjectVisitorParams& p) :
		key(p.key), parent(p.parent), elementIndex(p.elementIndex), elementCount(p.elementCount), arrayIndex(p.arrayIndex), arrayCount(p.arrayCount) { }

	const string& getKey() const {
		return key;
	}

	const BSONObj* getParent() const {
		return parent;
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
	 * \fn void onObjectStart(const BSONObj& object, int arrayIndex)
	 * \brief BSON Object Precursor Event
	 * \param[in] vparams The \ref BSONObjectVisitorParams object containing the element properties.
	 * \param[in] object The object being parsed.
	 *
	 * Invoked once per each non-terminal BSON object before parsing the contained BSON elements.
	 */
	virtual void onObjectStart(const BSONObjectVisitorParams& vparams, const BSONObj& object) = 0;
	/*!
	 * \fn void onObjectEnd(const BSONObj& object, int arrayIndex)
	 * \brief BSON Object Successor Event
	 * \param[in] vparams The \ref BSONObjectVisitorParams object containing the element properties.
	 * \param[in] object The object being parsed.
	 *
	 * Invoked once per each non-terminal BSON object after parsing the contained BSON elements.
	 */
	virtual void onObjectEnd(const BSONObjectVisitorParams& vparams, const BSONObj& object) = 0;

	/*!
	 * \fn void onArrayStart(const BSONElement& element, int count)
	 * \brief BSON Array Precursor Event
	 * \param[in] vparams The \ref BSONObjectVisitorParams object containing the element properties.
	 * \param[in] element The non-terminal array element being parsed.
	 *
	 * Invoked once per each non-terminal BSON array before parsing the contained BSON elements.
	 */
	virtual void onArrayStart(const BSONObjectVisitorParams& vparams, const BSONElement& element) = 0;
	/*!
	 * \fn void onArrayEnd(const BSONElement& element, int count)
	 * \brief BSON Array Successor Event
	 * \param[in] vparams The \ref BSONObjectVisitorParams object containing the element properties.
	 * \param[in] element The non-terminal array element being parsed.
	 *
	 * Invoked once per each non-terminal BSON array after parsing the contained BSON elements.
	 */
	virtual void onArrayEnd(const BSONObjectVisitorParams& vparams, const BSONElement& element) = 0;

	/*!
	 * \fn void onElement(const BSONElement& element, int arrayIndex)
	 * \brief BSON Element Event
	 * \param[in] vparams The \ref BSONObjectVisitorParams object containing the element properties.
	 * \param[in] element The terminal element being parsed.
	 *
	 * Invoked once per each terminal BSON element that is not a BSON object or a BSON array.
	 */
	virtual void onElement(const BSONObjectVisitorParams& vparams, const BSONElement& element) = 0;
};

//----------------------------------------------------------------------------

/*!
 * \class BSONObjectParser
 * \brief Parser for BSON objects
 *
 * This class is is used to tear apart a given BSON object and process its data content.
 *
 * It is intended to be fully extensible as the internal methods are all declared protected.
 *
 * \b Usage:
 * \see IBSONObjectVisitor
 */

class BSONObjectParser {

protected:
	/*!
	 * Visitor implementation that receives the parse events.
	 */
	IBSONObjectVisitor& visitor;

	//----------------------------------------------------------------------------

	/*!
	 * \brief Recursively parse a BSONElement
	 * \param[in] element The BSON element to dump.
	 * \param[in] element The zero based index of the current BSON element within the current element's parent object.
	 * \param[in] element The count of BSON elements within the current element's parent object.
	 * \param[in] arrayIndex The array index of the BSON element being parsed.
	 * <ul><li>arrayIndex == -1 if the element is not contained in an array.</li><li>arrayIndex >= 0 if the element is contained within an array.</li></ul>
	 *
	 * <ul>
	 * <li>If it is a BSON object (BSONObj) dump it via indirect recursion by calling parseObjectRecursive().</li>
	 * <li>If it is a BSON array iterate through the contained BSONElement(s) and dump via direct recursion by calling parseElementRecursive().</li>
	 * <li>If it is neither an object nor an array simply dump the BSONElement by printing its key, value, and BSON data type.</li>
	 * </ul>
	 */

	virtual void parseElementRecursive(const BSONObjectVisitorParams& vparams, BSONElement& element) {
		BSONType btype = element.type();
		switch (btype) {
		case BSONType::Object:
			{
				const BSONObj& bobj = element.Obj();
				string k(element.fieldName());
				parseObjectRecursive(k, bobj, vparams.getParent(), vparams.getElementIndex(), vparams.getElementCount(), vparams.getArrayIndex());
			}
			break;
		case BSONType::Array:
			{
				std::vector<BSONElement> elementArray = element.Array();
				int elementArrayCount = elementArray.size();
				BSONObjectVisitorParams vp(vparams.getKey(), vparams.getParent(), vparams.getElementIndex(), vparams.getElementCount(), -1, elementArrayCount);
				visitor.onArrayStart(vp, element);
				int elementArrayIndex = 0;
				for (BSONElement e : elementArray) {
					string k(e.fieldName());
					BSONObjectVisitorParams ve(k, vparams.getParent(), vparams.getElementIndex(), vparams.getElementCount(), elementArrayIndex++, elementArrayCount);
					parseElementRecursive(ve, e);
				}
				visitor.onArrayEnd(vp, element);
			}
			break;
		default:
			visitor.onElement(vparams, element);
			break;
		}
	}

	//----------------------------------------------------------------------------

	/*!
	 * \brief Recursively parse a BSON Object (BSONObj)
	 * \param[in] object The BSON object to dump.
	 * \param[in] arrayIndex The array index of the BSON object being parsed.
	 * <ul><li>arrayIndex == -1 if the element is not contained in an array.</li><li>arrayIndex >= 0 if the element is contained within an array.</li></ul>
	 *
	 * Iterate through all the BSONElement(s) contained in the BSONObj and dump them via indirect recursion by calling parseElementRecursive().
	 */

	virtual void parseObjectRecursive(string& key, const BSONObj& object, const BSONObj* parent=NULL, int elementIndex=0, int elementCount=1, int arrayIndex = -1) {
		BSONObjectVisitorParams vparams(key, parent);
		visitor.onObjectStart(vparams, object);
		set<string> keys;
		object.getFieldNames(keys); // Get the key names of the BSON object.
		int ei = 0;
		int ec = keys.size();
		for (string key : keys) {
			BSONElement e = object.getField(key);
			BSONObjectVisitorParams vp(key, &object, ei++, ec, arrayIndex);
			parseElementRecursive(vp, e);
		}
		visitor.onObjectEnd(vparams, object);
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
	 * Parse the given BSON object and invoke the event handlers of the registered IBSONObjectVisitor visitor subclass.
	 */

	virtual void parse(const BSONObj& object) {
		visitor.onParseStart();
		string emptyString("");
		parseObjectRecursive(emptyString, object);
		visitor.onParseEnd();
	}

	//----------------------------------------------------------------------------
};

//----------------------------------------------------------------------------

} /* namespace mongotype */

//----------------------------------------------------------------------------

#endif /* BSONOBJECTPARSER_HPP_ */
