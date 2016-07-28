// Местами псевдокод
struct Scope {
	hashmap<std::string, Symbol> syms;
	Scope* parent;
};

struct SymbolTable {
	void push(std::string name, Symbol sym)
	{
		table[name].push_back(sym);
	}

	void pop(std::string name)
	{
		table[name].pop_back(sym);
	}

	Symbol get(std::string name)
	{
		return table[name].back();
	}

	void push(Scope scope)
	{
		for (auto p : scope.syms())
			push(p->first, p->second);
	}

	void pop(Scope scope)
	{
		for (auto p : scope.syms())
			pop(p->first);
	}


	hashmap<std::string, std::vector<Symbol>> table;
};


struct SymbolTable {
	void push(Scope* s)
	{
		top = s;
	}

	void pop(Scope* s)
	{
		top = s->parent;
	}

	Symbol get(std::string name)
	{
		for (Scope* s = top; s; s = s->parent)
			if (auto sym = s->syms[name])
				return sym;
		return 0;
	}

	Scope* top;
};


struct SymbolTable {
	SymbolTable(SymbolTable* dis, SymbolTable* prev)
		: cur(dis->cur), old(prev)
	{
	}

	void push(Scope s)
	{
		old = new SymbolTable(this, old);

		for (auto p : s->syms)
			cur[p->first] = p->second;
	}

	void pop()
	{
		auto _old = old;
		old = _old->old;
		delete _old;
	}

	Symbol get(std::string name)
	{
		return cur[name];
	}

	hashmap<std::string, Symbol> cur;
	SymbolTable* old;
};
