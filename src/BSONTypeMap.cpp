/*!
 * \file BSONTypeMap.cpp
 * \brief BSON Type Lookup Class Implementation
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

#include "mongotype.hpp"
#include "BSONTypeMap.hpp"

//----------------------------------------------------------------------------

namespace mongotype {

//----------------------------------------------------------------------------

/*!
 * BSON Type lookup table initialization.
 */

static BSONTypeStringsMap BSONTypeLookupTable {
	make_pair(MinKey, BSONTypeElement("MinKey", "MinKey")),
	make_pair(EOO, BSONTypeElement("EOO", "EOO")),
	make_pair(NumberDouble, BSONTypeElement("NumberDouble", "Double")),
	make_pair(String, BSONTypeElement("String", "UTF8")),
	make_pair(Object, BSONTypeElement("Object", "BSON")),
	make_pair(Array, BSONTypeElement("Array", "BSON Array")),
	make_pair(BinData, BSONTypeElement("BinData", "Binary")),
	make_pair(Undefined, BSONTypeElement("Undefined", "Undefined")),
	make_pair(jstOID, BSONTypeElement("jstOID", "ObjectId")),
	make_pair(Bool, BSONTypeElement("Bool", "Boolean")),
	make_pair(Date, BSONTypeElement("Date", "Date")),
	make_pair(jstNULL, BSONTypeElement("jstNULL", "NULL")),
	make_pair(RegEx, BSONTypeElement("RegEx", "Regex")),
	make_pair(DBRef, BSONTypeElement("DBRef", "deprecated")),
	make_pair(Code, BSONTypeElement("Code", "deprecated")),
	make_pair(Symbol, BSONTypeElement("Symbol", "Symbol")),
	make_pair(CodeWScope, BSONTypeElement("CodeWScope", "Javascript")),
	make_pair(NumberInt, BSONTypeElement("NumberInt", "int32")),
	make_pair(Timestamp, BSONTypeElement("Timestamp", "Timestamp")),
	make_pair(NumberLong, BSONTypeElement("NumberLong", "int64")),
	make_pair(JSTypeMax, BSONTypeElement("JSTypeMax", "Max Object Value Used")),
	make_pair(MaxKey, BSONTypeElement("MaxKey", "MaxKey"))
};

//----------------------------------------------------------------------------

/*!
 * Look up the given type.
 * \param[in] type The BSON type to convert to string(s).
 * \return The BSONTypeElement or "UNKNOWN" if  not found.
 */

const BSONTypeElement& BSONTypeMap::lookup(BSONType type) {
	try {
		const BSONTypeElement& rv = BSONTypeLookupTable.at(type);
		return rv;
	} catch (out_of_range& oor) {
		static const BSONTypeElement& unk = BSONTypeElement("UNKNOWN", "UNKNOWN");
		return unk;
	}
}

//----------------------------------------------------------------------------

} /* namespace mongotype */
