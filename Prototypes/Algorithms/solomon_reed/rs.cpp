#include <algorithm>
#include <iostream>
#include <cstddef>
#include <limits>
#include <fstream>
#include <vector>
#include "range.h"

template<typename...Args>
void print(Args&&... a)
{
	((std::cout << a << ' '), ...) << '\n';
}

struct polynomial : std::vector<int>{
	using std::vector<int>::vector;

	int& operator[](int i)
	{
		if (i < 0)
			return at( size()+i );
		return at(i);
	}

	int const& operator[](int i) const
	{
		if (i < 0)
			return at( size()+i );
		return at(i);
	}

	template<typename Iterator, typename Sentinel>
	polynomial& append(Iterator it, Sentinel it2 )
	{
		insert(end(),it,it2);
		return *this;
	}

	polynomial& append(polynomial const& b)
	{
		return append(b.begin(),b.end());
	}
};

polynomial operator+(polynomial a, polynomial const& b)
{
	return a.append(b);
}
std::ostream& operator<<(std::ostream& os, polynomial const& p)
{
	os << '[';
	if (p.size()>0)os <<p[0];
	for (int i = 1,e=p.size(); i<e;++i)
		os << ", " << p[i];
	os << ']';
	return os;
}

polynomial preverse(polynomial x)
{
	std::reverse(x.begin(),x.end());
	return x;
}

int mod(int x, int y)
{
	return ((x % y) + y) % y;
}


constexpr int gf_size = 256;

int gf_add( int a, int b ) { return a ^ b; }
int gf_sub( int a, int b ) { return a ^ b; }


int gf_mul1( int x, int y, int p=0)
{
	int r = 0;
	while ( y > 0 ) {
		if (y & 1)
			r = r ^ x;
		y = y >> 1;
		x = x << 1;

		if (p > 0 && x >= gf_size)
			x = x ^ p;
	}
	return r;
}

polynomial gf_exp(2*gf_size);
polynomial gf_log(gf_size);

constexpr int gf_prim = 0x11d;

auto init_ = [] {
	int x = 1;
	for (int i = 0; i < 255; ++i) {
		gf_exp[i] = x;
		gf_log[x] = i;
		x = gf_mul1( x, 2, gf_prim );
	}

	for (int i = 255; i < 512; ++i)
		gf_exp[i] = gf_exp[i - 255];

	return x;
}();

int gf_mul(int x, int y)
{
	if (x==0 || y==0) return 0;
	return gf_exp[(gf_log[x] + gf_log[y])];
}


int gf_div(int x, int y)
{
    if (y==0) throw 0;
    if (x==0) return 0;
    return gf_exp[mod(gf_log[x] + 255 - gf_log[y],255)];
}
int gf_pow(int x, int power)
{
	return gf_exp[mod(gf_log[x] * power,255)];
}

int gf_inverse(int x)
{
	return gf_exp[255 - gf_log[x]];
}


polynomial gf_poly_scale(polynomial p, int x)
{
	polynomial r(size(p));
	for ( auto i : range(size(p)))
		r[i] = gf_mul(p[i], x);
	return r;
}

polynomial gf_poly_add(polynomial p, polynomial q)
{
	polynomial r(std::max(size(p),size(q)));
	for ( auto i : range(size(p)))
		r[i+size(r)-size(p)] = p[i];
	for ( auto i : range(size(q)))
		r[i+size(r)-size(q)] ^= q[i];
	return r;
}

polynomial gf_poly_mul(polynomial p, polynomial q)
{
	polynomial r(size(p)+size(q)-1);
	for (auto j : range(0, size(q))) {
		for (auto i : range(0, size(p))) {
			r[i+j] ^= gf_mul(p[i], q[j]);
			//r[i + j] = gf_add(r[i+j], gf_mul(p[i], q[j]))
		}
	}
	return r;
}

struct div_result
{
	polynomial q,r;
};

div_result gf_poly_div(polynomial dividend, polynomial divisor)
{
	auto r = dividend;
	auto e = size(dividend) - (size(divisor)-1);
	for (auto i : range(0, e)) {
		auto coef = r[i];
		if (coef != 0) {
			for (auto j : range(1, size(divisor))) {
				if (divisor[j] != 0) {
					r[i + j] ^= gf_mul(divisor[j], coef);
					//r[i + j] = gf_sub(r[i+j],gf_mul(divisor[j], coef));
				}
			}
		}
	}

	auto separator = size(divisor)-1;
	polynomial quotinent(r.begin(), r.end() - separator);
	polynomial remainder(r.end()-separator, r.end());
	return {quotinent, remainder};
}

int gf_poly_eval(polynomial poly, int x)
{
	auto y = poly[0];
	for (auto i : range(1, size(poly)))
		y = gf_mul(y, x) ^ poly[i];
	return y;
}

polynomial rs_generator_poly(int nsym)
{
	polynomial g = {1};
	for (auto i : range(0, nsym)) {
		polynomial m = {1, gf_pow(2, i)};
		g = gf_poly_mul(g, m);
	}
	return g;
}

polynomial rs_encode_msg(polynomial msg_in, int nsym)
{
	auto gen = rs_generator_poly(nsym);

	polynomial pad( size(gen)-1 );

	auto [_, remainder] = gf_poly_div(msg_in+pad, gen);
	auto msg_out = msg_in + remainder;

	return msg_out;
}

polynomial rs_calc_syndromes(polynomial msg, int nsym)
{
	polynomial synd(nsym);
	for (auto i : range(0, nsym))
		synd[i] = gf_poly_eval(msg, gf_pow(2,i));
	synd.insert(synd.begin(),0);
	return synd;
}

bool rs_check(polynomial msg, int nsym)
{
	auto synd = rs_calc_syndromes(msg, nsym);
	return *std::max_element(synd.begin(), synd.end()) == 0;
}

polynomial rs_find_errata_locator(polynomial e_pos)
{
	polynomial e_loc = {1};
	for (auto i : e_pos) {
		polynomial m = {gf_pow(2, i), 0};
		polynomial h = {1};
		e_loc = gf_poly_mul( e_loc, gf_poly_add(h, m) );
	}
	return e_loc;
}

polynomial rs_find_error_evaluator(polynomial synd, polynomial err_loc, int nsym)
{
	polynomial f = {1};
	f.resize(f.size()+nsym+1);
	auto [_, remainder] = gf_poly_div( gf_poly_mul(synd, err_loc), (f) );
	return remainder;
}

polynomial rs_correct_errata(polynomial msg_in, polynomial synd, polynomial err_pos)
{
	polynomial coef_pos;
	for (auto p : err_pos)
		coef_pos.push_back(size(msg_in) - 1 - p);

	auto err_loc = rs_find_errata_locator(coef_pos);
	auto err_eval = rs_find_error_evaluator(preverse(synd), err_loc, size(err_loc)-1);
	std::reverse(err_eval.begin(), err_eval.end());

	polynomial X;
	for (auto i : range(0, size(coef_pos)))
	{
		auto l = 255 - coef_pos[i];
		X.push_back( gf_pow(2, -l) );
	}

	polynomial E(size(msg_in));
	auto Xsize = size(X);
	for (auto [i, Xi] : ipairs(X)) {
		auto Xi_inv = gf_inverse(Xi);

		polynomial err_loc_prime_tmp;
		for (auto j : range(0, Xsize)) {
			if (j != i)
			{
				auto v = gf_sub(1, gf_mul(Xi_inv, X[j]));
				err_loc_prime_tmp.push_back( v );
			}
		}

		auto err_loc_prime = 1;
		for (auto coef : err_loc_prime_tmp)
			err_loc_prime = gf_mul(err_loc_prime, coef);

		auto y = gf_poly_eval(preverse(err_eval), Xi_inv);
		y = gf_mul(gf_pow(Xi, 1), y);


		if (err_loc_prime == 0)
			throw ("Could not find error magnitude");

		auto magnitude = gf_div(y, err_loc_prime);
		E[err_pos[i]] = magnitude;
	}

	msg_in = gf_poly_add(msg_in, E);
	return msg_in;
}


polynomial rs_find_error_locator(polynomial synd, int nsym, polynomial* erase_loc, int erase_count=0)
{
	polynomial err_loc, old_loc;
	if (erase_loc) {
		err_loc = *erase_loc;
		old_loc = *erase_loc;
	} else {
		err_loc = {1};
		old_loc = {1};
	}

	auto synd_shift = 0;
	if (size(synd) > nsym)
		synd_shift = size(synd) - nsym;


	for (auto i : range(0, nsym-erase_count)) {
		int K;
		if (erase_loc)
			K = erase_count+i+synd_shift;
		else
			K = i+synd_shift;

		auto delta = synd[K];
		for (auto j : range(1, size(err_loc)))
			delta ^= gf_mul(err_loc[-(j+1)], synd[K - j]);
		old_loc.push_back(0);
		if (delta != 0)
		{
			if (size(old_loc) > size(err_loc))
			{
				auto new_loc = gf_poly_scale(old_loc, delta);
				old_loc = gf_poly_scale(err_loc, gf_inverse(delta));
				err_loc = new_loc;
			}

			err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta));
		}
	}

	while (size(err_loc) && err_loc[0] == 0)
		err_loc.erase(err_loc.begin());
	auto errs = size(err_loc) - 1;
	if ((errs-erase_count) * 2 + erase_count > nsym)
		throw ("Too many errors to correct");
	return err_loc;
}


polynomial rs_find_errors(polynomial err_loc, int nmess)
{
	auto errs = size(err_loc) - 1;
	polynomial err_pos;
	for (auto i : range(nmess))
	{
		if (gf_poly_eval(err_loc, gf_pow(2, i)) == 0)
			err_pos.push_back(nmess - 1 - i);
	}
	if (size(err_pos) != errs)
		throw ("Too many (or few) errors found by Chien Search for the errata locator polynomialial!");
	return err_pos;
}


polynomial rs_forney_syndromes(polynomial synd, polynomial pos, int nmess)
{
	polynomial erase_pos_reversed;
	for (auto p : pos)
		erase_pos_reversed.push_back(nmess-1-p);


	polynomial fsynd(synd.begin()+1,synd.end());
	for (auto i : range(0, size(pos)))
	{
		auto x = gf_pow(2, erase_pos_reversed[i]);
		for (auto j : range(0, size(fsynd) - 1))
			fsynd[j] = gf_mul(fsynd[j], x) ^ fsynd[j + 1];
	}

	return fsynd;
}


struct decoded_msg {
	polynomial msg;
	polynomial ecc;
};

decoded_msg rs_correct_msg(polynomial msg_in, int nsym, polynomial* erase_pos_p)
{
	if (size(msg_in) > 255)
		throw ("Message is too long");

	auto msg_out = msg_in;

	polynomial erase_pos;
	if (erase_pos_p) {
		erase_pos = *erase_pos_p;
		for (auto e_pos : erase_pos)
			msg_out[e_pos] = 0;
	}

	if (size(erase_pos) > nsym)
		throw ("Too many erasures to correct");

	auto synd = rs_calc_syndromes(msg_out, nsym);

	if (*std::max_element(synd.begin(),synd.end()) > 0) {
		auto fsynd = rs_forney_syndromes(synd, erase_pos, size(msg_out));
		auto err_loc = rs_find_error_locator(fsynd, nsym, 0, size(erase_pos));
		auto err_pos = rs_find_errors(preverse(err_loc), size(msg_out));

		msg_out = rs_correct_errata(msg_out, synd, (erase_pos + err_pos));

		auto synd = rs_calc_syndromes(msg_out, nsym);
		if (*std::max_element(synd.begin(),synd.end()) > 0)
			throw ("Could not correct message");
	}
	polynomial msg(msg_out.begin(),msg_out.end()-nsym);
	polynomial ecc(msg_out.end()-nsym,msg_out.end());
	return { msg, ecc };
}


polynomial mkpoly(std::string s)
{
	polynomial p;
	for (auto c : s)
		p.push_back(int(c));
	return p;
}

std::string mks(polynomial p)
{
	std::string s;
	for (auto c : p)
		s.push_back(char(c));
	return s;
}


using vec = std::vector<char>;

struct msg_ecc {
	vec msg;
	vec ecc;
};

constexpr auto prim = 0x11d;
constexpr auto n = 255;
constexpr auto k = 238;

void to_poly( polynomial& bk, vec const& ec )
{
	for (auto& e : ec)
		bk.push_back( int(unsigned(e) & 0xFF) );
}


msg_ecc encode(vec const& msg)
{
	auto sz = msg.size();
	auto nblocks = sz/k + 1;

	vec ecc;

	for (auto i = 0; i < nblocks; ++i) {
		polynomial bk;

		size_t m1 = i*k;
		size_t m2 = (i+1)*k;
		m2 = std::min(m2,msg.size());
		to_poly( bk, vec( msg.begin() + m1, msg.begin() + m2 ) );
		//bk.resize(k);
		auto msecc = rs_encode_msg( bk, n-k );

		auto sz = msecc.size() - (n-k);
		for (auto j = sz; j < msecc.size(); ++j)
			ecc.push_back(unsigned(msecc[j])&0xFF);
		auto [m,ec] = rs_correct_msg( msecc, n-k, 0 );
	}
	return { msg, ecc };
}

msg_ecc decode(vec const& msg, vec const& ecc)
{
	auto sz = msg.size();
	auto nblocks = sz/k + 1;

	vec dec;
	vec dececc;

	for (auto i = 0; i < nblocks; ++i) {
		polynomial bk;

		size_t m1 = i*k;
		size_t m2 = (i+1)*k;
		m2 = std::min(m2,msg.size());
		to_poly( bk, vec( msg.begin() + m1, msg.begin() + m2 ) );

		size_t e1 = i*(n-k);
		size_t e2 = (i+1)*(n-k);
		e2 = std::min(e2,ecc.size());
		to_poly( bk, vec( ecc.begin() + e1, ecc.begin() + e2 ) );
		//bk.resize(k);

		auto [cm, ecc] = rs_correct_msg(bk, n-k, nullptr);

		for (auto c : cm)
			dec.push_back(char(c));
		for (auto c : ecc)
			dececc.push_back(char(c));

	};
	return { dec, dececc };
}

vec read_file( const char* path )
{
	std::ifstream file( path, std::ios::binary );
	vec contents;
	file.ignore( std::numeric_limits<std::streamsize>::max() );
	std::streamsize length = file.gcount();
	file.clear();
	file.seekg( 0, std::ios_base::beg );
	contents.reserve( length );
	contents.assign(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);
	return contents;
}

void write_file( vec const& v, const char* path )
{
	std::ofstream file( path, std::ios::binary );
	file.write( v.data(), v.size() );
}

void encode(const char* file, const char* ecc)
{
	auto data = read_file( file );

	try {
		auto [dmsg,decc] = encode( data );

		write_file( decc, ecc );
		std::cerr << "ECC data was written to: " << ecc << '\n';
	} catch (const char* err) {
		std::cerr << err << '\n';
	}
}

void check(const char* file, const char* ecc)
{
	auto msg = read_file( file );
	auto rs  = read_file( ecc );

	try {
		auto [dmsg,decc] = decode( msg, rs );

		if (dmsg == msg && rs == decc) {
			std::cerr << "File contents are OK." << '\n';
		} else {
			std::cerr << "Errors were corrected and written to file." << '\n';
			write_file( dmsg, file );
			write_file( decc, ecc );
		}

	} catch (const char* err) {
		std::cerr << err << '\n';
	}
}


int main(int argc, char** argv)
{
	std::string longmsg = R"(A long messsage for test:
----------------------------------------------------------
----------------------------------------------------------
----------------------------------------------------------
----------------------------------=-----------------------
----------------------------------------------------------
----------------------------------------------------------
----------------------------------------------------------
----------------------------------------------------------
----------------------------------------------------------
----------------------------------------------------------)";

	vec msg1;
	if (argv[1]) {
		msg1 = read_file( argv[1] );
	} else msg1 = vec(longmsg.begin(), longmsg.end());
	auto [msg,ecc] = encode( msg1 );
	for (auto b : ecc)
		std::cout << std::hex << (unsigned(b)&0xFF) << ' ';

	auto msgcorrupt = msg1;
		/*R"(A long messsage for test:
----------------------------------------------------------
---------------------+------------------------------------
---------------------+------------------------------------
---------------------+------------=-----------------------
---------------------+------------------------------------
---------------------+------------------------------------
---------------------+------------------------------------
---------------------+------------------------------------
---------------------+------------------------------------
----------------------------------------------------------)"*/;
	try {
	auto [dmsg,decc] = decode( msgcorrupt, ecc );
	} catch(const char* s) {
		std::cerr << s << '\n';
	}


	try{
	auto prim = 0x11d;
	auto n = 256;
	auto k = 238;
	std::string msg = "hello world!";


	auto mesecc = rs_encode_msg(mkpoly(msg), n-k);
	std::cout << mesecc.size() << '\n';
	std::cout << "Original: ";
	for (auto c :mesecc)
		std::cout << c << ' ';
	std::cout << '\n';

	mesecc[0] = 0;
	mesecc[1] = 2;
	mesecc[2] = 2;
	mesecc[3] = 2;
	mesecc[4] = 2;
	mesecc[5] = 2;
	std::cout << "Corrupted: ";
	std::cerr<< mesecc <<'\n';

	polynomial erase{0, 1, 2};
	auto [cm, ecc] = rs_correct_msg(mesecc, n-k, &erase);

	std::cout << "Repaired: ";
	for (auto c : cm )
		std::cout << c << ' ';
	for (auto c : ecc )
		std::cout << c << ' ';
	std::cout << '\n';
	std::cout << mks(cm) << '\n';
	} catch (const char* s) {
		std::cout << s;
	}
}
