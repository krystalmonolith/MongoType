/*!
 * \file mongotype.cpp
 * \brief MongoType Main Implementation File.
 *
 * \mainpage MongoType Utility
 *
 * MongoType is a command line utility designed to dump
 * out the contents of a MongoDB collection with the associated
 * BSON data types.
 *
 * ---
 *
 * \author Mark Deazley &lt;mdeazley@gmail.com&gt;
 * \version 2.4.1
 * \copyright Copyright &copy; 2013-2014 by Mark Deazley
 *
 * #### License
 *
 * ##### Free Software Foundation’s GNU AGPL v3.0.
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
 * ---
 *
 * #### Hints:
 *
 * - This documentation is heavily hyperlinked: Navigate around to get a feel for the different views, .i.e., files, namespaces, classes, etc are all different views of the programs structure.
 * - The colorized and hyperlinked program source code matching this version of the documentation is accessed by clicking on the line number links or the links on the file and class pages.
 *
 * ---
 *
 * ### Summary of Operation:
 * #### main() Function:
 *
 * The main() functionality is relatively simple:
 *
 * - Instantiate a mongotype::Parameters object.
 * - Parse the command line options and positional parameters with call to mongotype::Parameters::parse.
 * - Fetch the collection and output it in mongotype::dumpCollection per the parsed command line.
 *
 * #### mongotype NameSpace:
 *
 * The \ref mongotype namespace is where the majority of the code resides to avoid conflicts.
 *
 * ##### The mongotype::dumpCollection function:
 *
 * This is a catch-all function that:
 *
 *  - Opens a connection to the MongoDB database and gets a cursor to the collection specified by the Parameters object.
 *  - Constructs an object that sub-classes mongotype::IBSONRenderer that corresponds to the current mongotype::StyleParam.
 *  - Invokes the object's mongotype::IBSONRenderer::begin function once to initialize rendering.
 *  - Invokes the object's mongotype::IBSONRenderer::render function with each MongoDb document returned by the cursor.
 *  - Invokes the object's mongotype::IBSONRenderer::end function once to finalize rendering.
 *
 * ##### Command Line Parameter Parsing:
 *
 * The mongotype::Parameters class uses the <a href="http://boost.org/">boost::program_options</a> library to
 * do the command line parsing, supplying initialization, validation overload functions, and getter access functions.
 * While technically _not_ an immutable class it functions as such in MongoType once command line parsing is completed.
 *
 * ##### Interface mongotype::IBSONRenderer:
 *
 * All the concrete style implementation classes implement mongotype::IBSONRenderer to provide a polymorphic _begin-render-end_ rendering sequence
 * to create the output. This interface allows the style classes to customize the output but present a common interface .aka. API.
 *
 * ##### Interface mongotype::IBSONObjectVisitor:
 *
 * All the concrete style implementation classes _also_ implement mongotype::IBSONObjectVisitor to receive the events that are generated when a mongo::BSONObj object is parsed.
 * This interface allows the style implementation classes to parse BSON objects via the common implementation class mongotype::BSONObjectParser.
 *
 * ##### BSON Object Parser: mongotype::BSONObjectParser
 *
 * The mechanics of decomposing BSON objects is encapsulated in the mongotype::BSONObjectParser class which relies on mongotype::IBSONObjectVisitor implementors described below.
 * It supplies the instance method mongotype::BSONObjectParser::parse as the entry point to initiate parsing of a mongo::BSONObj object.
 *
 * ##### Style Implementation Classes
 *
 * The following classes implement mongotype::IBSONRenderer and mongotype::IBSONObjectVisitor as described above:
 *
 *  - mongotype::BSONObjectTypeDump - Implements --style=tree
 *  - mongotype::BSONDotNotationDump - Implements --style=dotted
 *  - mongotype::JSONDump - Implements --style=json and --style=jsonpacked
 *
 *  #### Utility Classes:
 *
 *  - mongotype::BSONTypeMap - Decodes BSON Type codes into mnemonics and description strings.
 *  - mongotype::BSONTypeFormatter - Formats BSON Type Codes into strings per the current mongotype::Parameters::getTypeMask value.
 *  - mongotype::EnumMapper - Maps enumeration integers to their string equivalent.
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
 * \namespace mongotype
 * All classes defined in the MongoType project are in C++ namespace \b mongotype.
 */

namespace mongotype {

//----------------------------------------------------------------------------

const string VERSION("2.4.1");
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
			<< documentCount << " }\n";
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

