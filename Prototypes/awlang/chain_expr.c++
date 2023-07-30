bool can_chain(const ast::binary_expression& in_expr)
{
	auto op = convert_operator(in_expr.op);
	if (op != ir::binary_operator::assignment) {
		// For left-associative expresions, the parser always produces
		// a tree of form (+ (+ a b) c), so we can skip checking the rhs
		if (auto lhs = get_if<ast::binary_expression>(in_expr.lhs.get()))
			return lhs->op == in_expr.op;
	}

	return false;
}

auto convert_chain_expr(const ast::binary_expression& in_expr) -> middle::chain_expression
{
	middle::chain_expression expr{
		.op = convert_operator(in_expr.op),
	};

	auto* lhs = &in_expr;

	std::vector<const ast::binary_expression*> stack{ lhs };
	while (true) {
		lhs = get_if<ast::binary_expression>(lhs->lhs.get());
		if (lhs && lhs->op == in_expr.op) {
			stack.push_back(lhs);
		} else {
			break;
		}
	}

	auto* top = stack.back();
	expr.operands.push_back(convert_expr(*top->lhs));

	while (!stack.empty()) {
		stack.pop_back();
		expr.operands.push_back(convert_expr(*top->rhs));
		top = stack.back();
	}

	return expr;
}
