/*
 * Parameters.h
 *
 *  Created on: Jan 21, 2014
 *      Author: mdeazley
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <mongotype.hpp>

#define DEFAULT_CONFIGURATION_FILE "~/.mongotype"
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 27017

namespace mongotype {

enum StyleParam {
	STYLE_UNDEF,
	STYLE_DOTTED,
	STYLE_TREE,
	STYLE_JSON
};

enum TypeParamMask {
	TYPE_UNDEF = -1,
	TYPE_NONE = 0,
	TYPE_NAME = 1,
	TYPE_DESC = 2,
	TYPE_CODE = 4,
	TYPE_ALL  = 7
};

template <class E> class EnumMapper {
	typedef map<string, E> EnumMap;
	typedef typename EnumMap::value_type EnumMapValue;

	EnumMap enumMap;

public:
	EnumMapper() {}
	void insert(const char* key, E value) { EnumMapValue v(string(key), value); enumMap.insert(v); }
	E find(string& s, E defValue) { typename EnumMap::iterator i = enumMap.find(s); if (i != enumMap.end()) { return i->second; } else { return defValue; } }
};

class Parameters {
	bool valid;
    string config_file;
    string host;
    int port;
    StyleParam style;
    TypeParamMask typeMask;
    bool scalarFirst;
    string dbCollection;
    string query;
    string projection;

public:

    Parameters();
	virtual ~Parameters();

	int parse(int ac, char* av[]);

	bool isValid() { return valid; }

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

	const string& getProjection() const {
		return projection;
	}

	const string& getQuery() const {
		return query;
	}

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

#endif /* PARAMETERS_H_ */
