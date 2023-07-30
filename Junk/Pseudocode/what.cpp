



somefunc()
{
	tok = tok.next();
	switch(tok[0]) {
		cast '[':return parse_rest_of](tok);
		cast '{':return parse_rest_of}(tok);
		cast '(':return parse_rest_of)(tok);
	}
}

someotherfunc()
{
	tok = tok.next();
	assert(tok.type == name);
	parse_[]( tok );
}

parse_[]()
{
	tok = tok.next();
	assert(tok[0] == '[');
	parse_rest_of](tok);
}

parse_rest_of]()
{
	tok = tok.next();
	assert(tok.type == int);
	auto idx = tok.to_int();
	tok = tok.next();
	assert(tok[0] == ']');
}