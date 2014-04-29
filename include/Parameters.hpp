/*!
 * \file Parameters.hpp
 * \brief MongoType Parameter Parsing and Access.
 *
 * - Uses boost::program_options for parsing of command line options.
 * - Provides a read-only interface to parsed parameters.
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
 * Creation Date: January 21, 2014
 * Eclipse Project: MongoType
 *
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <mongotype.hpp>

#define DEFAULT_CONFIGURATION_FILE "~/.mongotype"
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 27017

namespace mongotype {

/**
 * Enumeration of --style options.
 */

enum StyleParam {
	STYLE_UNDEF      = -1,	/**< UNDEFINED: Used to signal parsing errors */
	STYLE_DOTTED     = 0,	/**< Dotted Output: see \ref BSONDotNotationDump */
	STYLE_TREE       = 1,	/**< Tree Output: see \ref BSONObjectTypeDump  */
	STYLE_JSON       = 2,	/**< Pretty JSON Output: see \ref JSONDump */
	STYLE_JSONPACKED = 3	/**< Packed JSON Output: see \ref JSONDump */
};

enum TypeParamMask {
	TYPE_UNDEF = -1,
	TYPE_NONE  = 0,
	TYPE_NAME  = 1,
	TYPE_DESC  = 2,
	TYPE_CODE  = 4,
	TYPE_ALL   = 7
};

/**
 * Map enumeration integers to their string equivalents.
 */

template <class E> class EnumMapper {
	typedef map<string, E> EnumMap;
	typedef typename EnumMap::value_type EnumMapValue;

	EnumMap enumMap;

public:
	EnumMapper() {}
	/**
	 * Insert an string key and its corresponding enumeration value to the internal associative array.
	 * \param[in] key The key string.
	 * \param[in] value The enumeration value.
	 */
	void insert(const char* key, E value) { EnumMapValue v(string(key), value); enumMap.insert(v); }
	/**
	 * Look up the enumeration value corresponding to the input string in the associative array. and return its enumeration value, or the default value if it is not found.
	 * \param[in] s The key string.
	 * \param[in] defValue The default enumeration value.
	 * \return The enumeration value if the string is found, or the default value if it is not found.
	 */
	E find(string& s, E defValue) { typename EnumMap::iterator i = enumMap.find(s); if (i != enumMap.end()) { return i->second; } else { return defValue; } }
};

/**
 * Command Line Parameter parser and read-only access member functions.
 *
 * - Uses <a href="http://www.boost.org/index.html">boost::program_options</a> for parsing of command line options.
 * - Provides a read-only interface to parsed parameters.
 */

class Parameters {
	bool valid;
    string config_file;
    string host;
    int port;
    bool scalarFirst;
    StyleParam style;
    TypeParamMask typeMask;
    string dbCollection;
//    string query;
//    string projection;

    boost::program_options::variables_map vm;

public:

    Parameters();
	virtual ~Parameters();

	int parse(int ac, char* av[]);

	bool isValid() { return valid; }

	bool isDebug() { return vm.count("debug") > 0; }

	bool isStackDebug() { return vm.count("stack") > 0; }

	const string& getConfigFile() const {
		return config_file;
	}

	const string& getDbCollection() const {
		return dbCollection;
	}

	const string& getHost() const {
		return host;
	}

	int getPort() const {
		return port;
	}

//	const string& getProjection() const {
//		return projection;
//	}
//
//	const string& getQuery() const {
//		return query;
//	}

	bool isScalarFirst() const {
		return scalarFirst;
	}

	StyleParam getStyle() const {
		return style;
	}

	TypeParamMask getTypeMask() const {
		return typeMask;
	}

	friend ostream& operator <<(ostream& os, Parameters& p);
};

} /* namespace mongotype */

#endif /* PARAMETERS_HPP_ */
