struct sonsole {
	symbol_table symtab;
	using cmdfunc = void (sonsole::*)(token);
	std::map< string, cmdfunc > commands;

	sonsole()
	{
		comands["help"] = &sonsole::help;
		comands["set"] = &sonsole::set;
	}

	bool set( token tok ) {
		if ( tok.type() != tok_string ) {
			cerr << "varname, please\n";
			return false;
		}
		auto var = symtab.lookup( tok.to_string() );
		if (!var) {
			cerr << "nosuch var\n";
			return false;
		}

		tok = tok.next;
		if ( tok.type() != tok_number ) {
			cerr << "i can haz only nymber type\n";
			return false;
		}
		var->set( tok.to_double() );
	}

	// and so on for 999 more

	bool parse( string line )
	{
		token tok( line );

		if ( tok.type() != tok_string ) {
			cerr << "Enter comand blyad\n";
			continue;
		}

		auto func = commands( tok.to_string() );
		(this.*func)( tok.next() );
	}
};

int main()
{
	sonsole sos;

	using cmdfunc = function< bool(token) >;
	std::map< string, cmdfunc > commands;

	// and so on for 999 more

	string line;
	while (read_line(cout, line))
		sonsole.parse( sos );
}