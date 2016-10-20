void traverse_depth_order(Func f)
{
	vector<node*> stack{ this };
	while (!stack.empty()) {
		node* top = stack.back();
		top.pop_back();
		f(top);
		stack.append( rbegin(top->children), rend(top->children) );
	}
}

void traverse_breadth_order(Func f)
{
	deque<node*> queue{ this };
	while (!queue.empty()) {
		node* first = queue.front();
		queue.pop_front();
		f(first);
		queue.append( begin(first->children), end(first->children) );
	}
}
