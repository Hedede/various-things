core-rewrite


vs.


bool CHndfParser::getStringValue(std::string& val)
{	
	if(token_.type == TOKEN_STRING) {
		val = token_.value;
		return true;
	} 
	
	if (token_.type != TOKEN_LITERAL) {
		addError("expected value or type");
		return false;
	}
	
	stringValue_ = token_.value;
	readToken(token_);

	if(token_.type == TOKEN_SEPARATOR) {
		val = stringValue_;
		skipSeparators();
		return true;
	} else if(token_.type == TOKEN_TYPE_SEPARATOR && stringValue_ != "string") {
		addError("expected string value");
		return false;
	} else if(token_.type == TOKEN_LITERAL) {
		do {
			stringValue_ += " " + token_.value;
			readToken(token_);
		} while (token_.type != TOKEN_SEPARATOR);
		val = stringValue_;
		skipSeparators();
		return true;
	}
			
	readToken(token_);

	if(token_.type == TOKEN_LITERAL) {
		stringValue_ = "";
		do {
			stringValue_ += " " + token_.value;
			readToken(token_);
		} while (token_.type != TOKEN_SEPARATOR);
		val = stringValue_;
		skipSeparators();
		return true;
	} else if(token_.type == TOKEN_STRING) {
		val = token_.value;
		skipSeparators();
		return true;
	} else if(token_.type == TOKEN_NUMERIC) {
		val = token_.value;
		skipSeparators();
		return true;
	}
	
	addError("expected string value");
	return false;
}

bool CHndfParser::getFloatValue(float& val)
{
	if (token_.type != TOKEN_LITERAL) {
		addError("expected type");
		return false;

	}

	if(token_.value != "float") {
		addError("expeted float");
		return false;
	}

	readToken(token_);
	
	if (token_.type != TOKEN_TYPE_SEPARATOR) {
		addError("expected ':'");
		return false;
	}
	
	readToken(token_);

	if (token_.type != TOKEN_NUMERIC) {
		addError("expected numeric value");
		return false;
	}

	val = strtof(token_.value.c_str(), 0);

	skipSeparators();

	return true;
}

bool CHndfParser::getIntegerValue(int& val)
{
	if (token_.type != TOKEN_LITERAL) {
		addError("expected type");
		return false;

	}

	if(token_.value != "int") {
		addError("expeted integer");
		return false;
	}

	readToken(token_);
	
	if (token_.type != TOKEN_TYPE_SEPARATOR) {
		addError("expected ':'");
		return false;
	}
	
	readToken(token_);

	if (token_.type != TOKEN_NUMERIC) {
		addError("expected numeric value");
		return false;
	}

	val = strtol(token_.value.c_str(), 0, 0);

	skipSeparators();

	return true;
}