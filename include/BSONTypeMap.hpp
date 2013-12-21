/*!
 * \file BSONTypeMap.hpp
 * \brief BSON Type Lookup Class Definitions
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

#ifndef BSONTYPEMAP_HPP_
#define BSONTYPEMAP_HPP_

namespace mongotype {

//----------------------------------------------------------------------------

/*!
 * \class BSONTypeElement
 * \brief Store the name and description of a single BSON data type.
 *
 * Used as the elements of a lookup table.
 *
 * \see BSONTypeStringsMap
 */
class BSONTypeElement {
	/*!
	 * The BSON type name from the <a href="http://bsonspec.org/#/specification">BSON Non-terminals definition</a>.
	 */
	string name;
	/*!
	 * Additional comments and information regarding the BSON type.
	 */
	string description;

public:
	/*!
	 * Default Constructor
	 */
	BSONTypeElement(const char* pname = "", const char* pdescription = "") :
			name(pname), description(pdescription) {
	}

	/*!
	 * Fetch the name.
	 * \return The \ref name string.
	 */
	const string& getName() const {
		return name;
	}

	/*!
	 * Fetch the desciption.
	 * \return The \ref description string.
	 */
	const string& getDesc() const {
		return description;
	}
};

/*!
 *  Associative array used to look up BSON types containing \ref BSONTypeElement data elements.
 */
typedef map<BSONType, const BSONTypeElement> BSONTypeStringsMap;
/*!
 * Pair type for retrieving BSON type strings. \see BSONTypeStringsMap
 */
typedef BSONTypeStringsMap::value_type BSONTypeStringsMapPair;

//----------------------------------------------------------------------------

/*!
 * \brief BSON Element Type String Lookup
 *
 * Provides conversion from BSON integer element type codes to human readable strings.
 */

class BSONTypeMap {

	/*!
	 * The BSON element supplying the type information.
	 */
	const BSONElement& element;

public:

	/*!
	 * \brief Construct a type lookup instance.
	 * \param[in] e The BSONElement whose type string will be looked up.
	 */
	BSONTypeMap(const BSONElement& e) :
			element(e) {
	}

	/*!
	 * Lookup the given BSONType. Intended for repeated lookup calls without construction overhead.
	 */
	const BSONTypeElement& lookup(BSONType type);

	/*!
	 * Lookup the given BSONType for this instance.
	 */
	const BSONTypeElement& lookup() {
		return lookup(element.type());
	}

	/*!
	 * \brief BSON Object Type Strings Stream Output Operator.
	 * \param[in,out] out The std::ostream to write the BSON type strings.
	 * \param[in] bos The BSONTypeMap object to write to the output stream.
	 */
	string to_string() {
		const BSONTypeElement& bsts = lookup();
		string s("(");
		s += bsts.getName();
		s += "/";
		s += bsts.getDesc();
		s += ")";
		return s;
	}

	/*!
	 * \brief BSON Object Type Strings Stream Output Operator.
	 * \param[in,out] out The std::ostream to write the BSON type strings.
	 * \param[in] bos The BSONTypeMap object to write to the output stream.
	 */
	OSTREAM_FRIEND(BSONTypeMap& bstn){
		return out << bstn.to_string();
	}
};

//----------------------------------------------------------------------------

} /* namespace mongotype */
#endif /* BSONTYPEMAP_HPP_ */
