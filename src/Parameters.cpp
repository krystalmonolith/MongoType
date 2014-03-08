/*
 * Parameters.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: mdeazley
 */

#include <Parameters.hpp>

namespace po = boost::program_options;

namespace mongotype {

static bool initMap = true;
static EnumMapper<StyleParam> styleMapper;
static EnumMapper<TypeParamMask> typeMapper;

static void mapperInit() {
	if (initMap) {
		initMap = false;
		styleMapper.insert("dotted",      STYLE_DOTTED);
		styleMapper.insert("tree",        STYLE_TREE);
		styleMapper.insert("json",        STYLE_JSON);
		styleMapper.insert("jsonpacked",  STYLE_JSONPACKED);
		typeMapper.insert("none", TYPE_NONE);
		typeMapper.insert("name", TYPE_NAME);
		typeMapper.insert("desc", TYPE_DESC);
		typeMapper.insert("code", TYPE_CODE);
		typeMapper.insert("all",  TYPE_ALL);
	}
}

Parameters::Parameters() : valid(false), port(DEFAULT_PORT), scalarFirst(false), style(STYLE_DOTTED), typeMask(TYPE_ALL) {
	mapperInit();
}

Parameters::~Parameters() {
}

void validate(boost::any& v,
              const std::vector<std::string>& values,
              StyleParam*, int)
{
    po::validators::check_first_occurrence(v);
    const string& s = po::validators::get_single_string(values);
    StyleParam st = styleMapper.find(const_cast<string&>(s), STYLE_UNDEF);
    if (st != STYLE_UNDEF) {
        v = boost::any(st);
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value);
    }
}

void validate(boost::any& v,
              const std::vector<std::string>& values,
              TypeParamMask*, int)
{
    po::validators::check_first_occurrence(v);
    const string& s = po::validators::get_single_string(values);
    TypeParamMask t = typeMapper.find(const_cast<string&>(s), TYPE_UNDEF);
    if (t != TYPE_UNDEF) {
        v = boost::any(t);
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value);
    }
}

int Parameters::parse(int ac, char* av[])
{
	int rv = 0;
    try {
        // Options that will only be allowed on the command line.
        po::options_description general("General Options");
        general.add_options()
            ("help", "print help message")
            ("version,v", "print version string")
            ("debug,d", "print debugging info")
            ("stack,q", "print stack debugging info")
            ("config,c", po::value<string>(&config_file)->default_value(DEFAULT_CONFIGURATION_FILE),
                  "path of configuration file. Default configuration file: " DEFAULT_CONFIGURATION_FILE)
            ;

        // Options that will be allowed both on command line and in the configuration file.
        po::options_description server("MongoDB Server Options");
        server.add_options()
			("host,h", po::value<string>(&host)->default_value(DEFAULT_HOST),
                  "MongoDB server host name: <host>[:port]")
            ("port,p", po::value<int>(&port)->default_value(DEFAULT_PORT),
                 "MongoDB server port number.")
            ;

        // Options that will be allowed both on command line and in the configuration file.
        po::options_description oformat("Output Format Options");
        oformat.add_options()
					("style,s", po::value<StyleParam>(&style)->default_value(STYLE_DOTTED),
		                  "Output Style: {dotted,tree,json,jsonpacked}.")
	      			("type,t", po::value<TypeParamMask>(&typeMask)->default_value(TYPE_ALL),
                          "BSON Type: {none,name,desc,code,all}.")
                    ("scalarfirst,f", po::value<bool>(&scalarFirst)->default_value(false),
                          "Output scalar objects elements before any embedded objects or arrays.")
            ;

        // Hidden options, will be allowed both on command line and in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
                    ("dbcollection", po::value<string>(&dbCollection), //->required(),
                    		"Database and collection names concatenated with a '.' between them, i.e., \"mydb.mycollection\".")
//                    ("query", po::value<string>(&query)->default_value(string("")),
//                       		"Optional JSON query, i.e., the first parameter to find()")
//                    ("projection", po::value<string>(&projection)->default_value(string("")),
//                       		"Optional JSON query projection, i.e., the second parameter to find().")
            ;

        po::options_description cmdline_options;
        cmdline_options.add(general).add(server).add(oformat).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(server).add(oformat).add(hidden);

        po::options_description visible("\n\nSyntax:\n\tmongotype [<options>] <db.collection> [<query>] [<projection>]\n\nOptions");
        visible.add(general).add(server).add(oformat);

        po::positional_options_description p;
        p.add("dbcollection", 1);
//        p.add("query", 1);
//        p.add("projection", 1);

        store(po::command_line_parser(ac, av).options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        if (isDebug()) {
        	cout << "Parameters After Command Line:\n" << *this << "\n";
        }

        ifstream ifs(config_file.c_str());
        if (ifs)
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
            ifs.close();
        }

        if (vm.count("version")) {
            cout << mongotype::VERSION << "\n";
            exit(0);
        }

        if (vm.count("help")) {
            cout << visible << "\n";
            exit(0);
        }

        if (vm.count("dbcollection") == 0) {
            cout << po::invalid_syntax("<db.collection>", po::invalid_syntax::missing_parameter).what() << visible << "\n";
            exit(0);
        }

        valid = true;

        if (isDebug()) {
        	cout << "Parameters After Config File:\n" << *this << "\n";
        }
   }
    catch(exception& e)
    {
        cerr << e.what() << "\n";
        exit(1);
    }
    return rv;
}

ostream& operator <<(ostream& os, Parameters& p) {
    os << "valid:" << p.valid << "\n";
    os << "config_file:" << p.config_file << "\n";
    os << "host:" << p.host << "\n";
    os << "port:" << p.port << "\n";
    os << "style:" << p.style << "\n";
    os << "typeMask:" << p.typeMask << "\n";
    os << "scalarFirst:" << p.scalarFirst << "\n";
    os << "dbCollection:" << p.dbCollection << "\n";
//    os << "query:" << p.query << "\n";
//    os << "projection:" << p.projection << "\n";
    return os;
}

} /* namespace mongotype */
