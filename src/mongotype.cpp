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
#include <Parameters.hpp>
#include <BSONTypeMap.hpp>
#include <BSONObjectTypeDump.hpp>
#include <BSONDotNotationDump.hpp>
#include <JSONDump.hpp>

//----------------------------------------------------------------------------

/*!
 "\namespace mongotype
 "All classes defined in the MongoType project are in C++ namespace \b mongotype.
 */

namespace mongotype {

//----------------------------------------------------------------------------

const string VERSION("2.3");
const string COPYRIGHT("Copyright (c) 2013 by Mark Deazley");
const string LICENSE(
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
				"along with this program.  If not, see http://www.gnu.org/licenses/ .\n");

//----------------------------------------------------------------------------

void dumpCollection(Parameters& params) {
	DBClientConnection c;
	string hostPort(params.getHost());
	if (hostPort.find(':') == string::npos) {
		hostPort += ":";
		hostPort += to_string(params.getPort());
	}
	c.connect(hostPort);
	time_t t;
	time(&t);

	int documentCount = c.count(params.getDbCollection());

	if (params.isDebug()) {
		cout << "{ " << params.getDbCollection() << ".count: "
			<< documentCount << " }";
	}

	string docPrefixString(params.getDbCollection());
//	docIndex += "{";
//	docIndex += to_string(i++);
//	docIndex += "}";

	unique_ptr<DBClientCursor> cursor = c.query(params.getDbCollection(), BSONObj());

	unique_ptr<IBSONRenderer> renderer;
	switch (params.getStyle()) {
	case STYLE_DOTTED:
		renderer = unique_ptr<IBSONRenderer>(new BSONDotNotationDump(params, docPrefixString));
		break;
	case STYLE_TREE:
		renderer = unique_ptr<IBSONRenderer>(new BSONObjectTypeDump(params, docPrefixString));
		break;
	case STYLE_JSON:
	case STYLE_JSONPACKED:
		renderer = unique_ptr<IBSONRenderer>(new JSONDump(params, "  "));
		break;
	default:
		throw std::logic_error("ISE: Undefined STYLE!");
		break;
	}
	if (renderer) {
		renderer->setOutputStream(cout);
		renderer->begin(NULL);
		int documentIndex = 0;
		while (cursor->more()) {
			const BSONObj& o = cursor->next(); // Get the BSON Object
			renderer->render(o, documentIndex++, documentCount);
		}
		renderer->end(NULL);
	} else {
		throw std::logic_error("ISE: Undefined renderer!");
	}
}

//----------------------------------------------------------------------------

} // End of namespace mongotype

//----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	try {
		mongotype::Parameters params;
		params.parse(argc, argv);
		mongotype::dumpCollection(params);
	} catch (const mongo::DBException &e) {
		cerr << "mongotype MongoDB Error: \"" << e.what() << "\"" << endl;
		exit(2);
	} catch (std::logic_error &e) {
		cerr << "mongotype Generic Error: \"" << e.what() << "\"" << endl;
		exit(2);
	}
	return EXIT_SUCCESS;
}

