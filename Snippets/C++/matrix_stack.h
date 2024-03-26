struct matrix_stack {
	void push()
	{
		stack.push( _top );
	}


	void pop()
	{
		_top = stack.top;
		stack.pop();
	}

	void apply(matrix4<float> const& mat)
	{
		push();
		top *= mat;
	}

	matrix4<float>& top()
	{
		return _top;
	}

private:
	matrix4<float> _top;
	std::stack< matrix4<float>, std::vector<matrix4<float>> > stack;
};
