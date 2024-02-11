
macro x(...);

args:
decl, stmt, expr, int, uint, bool

macro mul(expr a, expr b) -> expr
{
	return a ## * ## b;
}

add( (123, 4, 5+4), 7+8+9 ); // turns into ‹ (123, 4, 5+4) * (7+8+9) › 
/*
or more precisely
(* (, 123 (, 4 (+ 5 4))) (+ 7 (+ 8 9)))
*/

macro repeat(stmt S, uint N) -> stmt
{
	if (N == 1)
		return S ## ;
	return S ## ; ## repeat(S, N-1)) // not decided on syntax
}

repeat( x--, 10 ) // x--; x--; ... x--; // 10 raz

macro make_serializator(decl D) -> func
{
	static_assert(D.kind == class); // can't really serialize funcs
	class_decl CD = D;

	var_decl arg1(CD.type ## &, "value");
	var_decl arg2(Archiver&, "arc");

	func_decl FD;

	FD.name = "serialize";
	FD.args = {arg1, arg2};

	for (decl member : CD.members) {
		if (decl.kind != var)
			continue;
		var_decl v = member;
		if (!has_attribute(v, "serializable"))
			return;
		stmt s = archive ## ( ## arc ## , ## v.name ## ) ## ;
		FD.body.append(s);
	}
}
