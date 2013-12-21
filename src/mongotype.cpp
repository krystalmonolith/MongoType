/*!
"\file mongotype.cpp
"\brief MongoType Main Implementation File.
 *
"\mainpage MongoType Utility
 *
"MongoType is a command line utility designed to dump
"out the contents of a MongoDB collection with the associated
"BSON data types.<br/>
 *
"<strong>BSON Definition: </strong><a href="http://bsonspec.org/">BSON Website</a><br>
"<strong>JSON Definition: </strong><a href="http://json.org/">JSON Website</a>
 *
"<h3>Usage:</h3>
 *
"<b>mongotype</b> <i>&lt;collectionName&gt;</i> [<i>&lt;host>&gt;</i>[:<i>&lt;port&gt;</i>]]<br/>
 *
"<h3>Where:</h3>
"<ul>
"  <li><i>&lt;collectionName&gt;</i> is the required name of the MongoDB collection to dump. Collection names follow the format &quot;<i>&lt;db&gt;</i>.<i>&lt;collection&gt;</i>&quot;.</li>
"  <li><i>&lt;host&gt;</i> or <i>&lt;host&gt;</i>:<i>&lt;port&gt;</i> are the optional MongoDB host/port.</li>
"  <li>The default &quot;<i>&lt;host&gt;</i>:<i>&lt;port&gt;</i>&quot; is &quot;localhost:27017&quot;.</li>
"</ul>
 *
"\author Mark Deazley &lt;mdeazley@gmail.com&gt;
"\version 2.0.0
"\copyright Copyright &copy; 2013 by Mark Deazley<br/><br/>
 *
"<b>License:</b> <i>Free Software Foundation’s GNU AGPL v3.0.</i><br/>
 *
"This program is free software: you can redistribute it and/or modify<br/>
"it under the terms of the GNU Affero General Public License as<br/>
"published by the Free Software Foundation, either version 3 of the<br/>
"License, or (at your option) any later version.<br/>
 *
"This program is distributed in the hope that it will be useful,<br/>
"but WITHOUT ANY WARRANTY; without even the implied warranty of<br/>
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br/>
"GNU Affero General Public License for more details.<br/>
 *
"You should have received a copy of the GNU Affero General Public License<br/>
"along with this program.  If not, see http://www.gnu.org/licenses/ .<br/>
 */

//----------------------------------------------------------------------------

#include <mongotype.hpp>
#include <BSONTypeMap.hpp>
#include <BSONObjectTypeDump.hpp>
#include <BSONDotNotationDump.hpp>

//----------------------------------------------------------------------------

/*!
"\namespace mongotype
"All classes defined in the MongoType project are in C++ namespace \b mongotype.
 */

namespace mongotype {

//----------------------------------------------------------------------------

static const string VERSION("2.1");
static const string COPYRIGHT("Copyright (c) 2013 by Mark Deazley");
static const string LICENSE(
	"Free Software Foundation’s GNU AGPL v3.0.\n"
	"This program is free software: you can redistribute it and/or modify\n"
	"it under the terms of the GNU Affero General Public License as\n"
	"published by the Free Software Foundation, either version 3 of the\n"
	"License, or (at your option) any later version.\n"
	"\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU Affero General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU Affero General Public License\n"
	"along with this program.  If not, see http://www.gnu.org/licenses/ .\n"
);



//----------------------------------------------------------------------------

void dumpCollection(string& collectionName, string& hostPort) {
	DBClientConnection c;
	c.connect(hostPort);
	time_t t;
	time(&t);

	cout << collectionName << ".count:" << c.count(collectionName) << "\n";

	unique_ptr<DBClientCursor> cursor = c.query(collectionName, BSONObj());
	unsigned int i = 0;
	while (cursor->more()) {
		const BSONObj& o = cursor->next(); // Get the BSON
//		BSONObjectTypeDump typeDump(o);
//		cout << typeDump << endl;
		string dotBase(collectionName);
		dotBase += "{";
		dotBase += to_string(i++);
		dotBase += "}";
		BSONDotNotationDump dotDump(o, dotBase);
		cout << dotDump;
	}
}

}

//----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	try {
		// Remember argv[0] is image name, so first parameter is argv[1].
		if (argc < 2) {
			cerr
				<< "MongoType v" << mongotype::VERSION << " by Mark Deazley" << "\n" << "\n"
				<< "Usage:" << "\n"
				<< "\tmongotype <collectionName> [<host>[:<port>]]" << "\n" << "\n"
				<< "Where:" << "\n"
				<< "\t\t<collectionName> is required. Format: <db>.<collection>" << "\n"
				<< "\t\t<host> and <host>:<port> are optional." << "\n"
				<< "\t\tDefault <host>:<port> is localhost:27017." << "\n" << "\n"
				<< mongotype::COPYRIGHT << "\n" << "\n"
				<< mongotype::LICENSE << endl;
			exit(1);
		}
		string collectionName(argv[1]);
		string hostPort("localhost");
		if (argc > 2) {
			hostPort = argv[2];
		}
		mongotype::dumpCollection(collectionName, hostPort);
	} catch (const mongo::DBException &e) {
		cerr << "mongotype Error: \"" << e.what() << "\"" << endl;
		exit(2);
	}
	return EXIT_SUCCESS;
}


