#ifndef MONGOTYPE_HPP_
#define MONGOTYPE_HPP_

/*!
 * \file mongotype.hpp
 * \brief MongoType Common Definitions File
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
 */

//----------------------------------------------------------------------------

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <time.h>
#include <mongo/client/dbclient.h>
#include <mongo/bson/bsonobj.h>

using namespace std;
using namespace mongo;

//----------------------------------------------------------------------------

/*!
 * \def OSTREAM_FRIEND
 * Used to standardize the definition of the overloaded output stream friend
 * functions. For example:
 * \code
 * class X {
 *   int i;
 * public:
 *   X(int pi) : i(pi) {}
 *   OSTREAM_FRIEND(X& x) {
 *   	out << "I:" << i;
 *   	return out;
 *   }
 * };
 *
 * X x(10);
 * cout << x; // Prints "I:10".
 * \endcode
 *
 */

#define OSTREAM_FRIEND(ref) friend std::ostream& operator<<(std::ostream& out, ref)

/*!
 * \note
 * <code>
 * class X {
 *   int i;
 * public:
 *   X(int pi) : i(pi) {}
 *   OSTREAM_FRIEND(X& x) {
 *   	out << "I:" << i;
 *   	return out;
 *   }
 *
 *   X x(10);
 *   cout << x; // Prints "I:10".
 *   </code>
 */

//----------------------------------------------------------------------------

#endif /* MONGOTYPE_HPP_ */
