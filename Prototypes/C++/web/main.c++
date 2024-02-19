#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcgio.h>
#include <fcgi_config.h>
#include <iostream>
#include <fstream>
#include <limits>

#include <aw/utility/string/split.h>
#include "parser.h"

enum class content {
	html
};

char const* name( content type )
{
	switch (type) {
	case content::html: return "text/html";
	}
}

void send_content_type( std::ostream& os, content type )
{
	os << "Content-type: " << name(type) << "\r\n\r\n";
}

using namespace std::string_view_literals;
constexpr aw::string_view filler = R"(<h1>FILLER</h1>
<h2>FILLER FILLER</h2>
<h3>FILLER FILLER FILLER</h3>
<h4>FILLER FILLER FILLER FILLER</h4>
<h5>FILLER FILLER FILLER FILLER FILLER</h5>
<h6>FILLER FILLER FILLER FILLER FILLER FILLER</h6>
)"sv;

aw::web::context make_ctx()
{
	aw::web::context ctx;
	ctx.vars.insert({
		{ "title",  "absurd worlds" },
		{ "css-path", "/static" },
		{ "header", "<h2>AW temporary home</h2>" },
		{ "sidebar", "<h2>FILLER</h2>" },
		{
			"navbar",
			R"(<p><a href="https://bitbucket.org/absurdworlds">AW repos</a></p>
<p><a href="https://bitbucket.org/Hedede">Hedede's repos</a></p>
<p><a href="http://git.absurdworlds.org/">absurdworlds git</a></p>)"
		},
		{
			"content",
			(std::string)filler,
		}
	});
	return ctx;
}


#include <aw/types/optional.h>
aw::optional<aw::web::html_template> tpl;

void load_tpl( std::string const& path )
{
	std::ifstream file{ path };
	std::string buf{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	tpl.emplace( std::move(buf) );
}

int main()
{
	using std::cin; using std::cout; using std::cerr;

	int count = 0;
	long pid = getpid();

	constexpr auto path = "/srv/http/dynamic/templates/base.html";
	auto reload_tpl = [] (int=0) { load_tpl(path); };
	signal(SIGUSR1, reload_tpl);
	reload_tpl();

	auto ctx = make_ctx();
	ctx.funcs.emplace("footer", [&] (std::ostream& os) {
		os << "<p>Page served " << ++count << " times by PID" << pid << "</p>\n";
	});

	std::streambuf* cin_streambuf  = cin.rdbuf();
	std::streambuf* cout_streambuf = cout.rdbuf();
	std::streambuf* cerr_streambuf = cerr.rdbuf();

	FCGX_Request request;

	FCGX_Init();
	FCGX_InitRequest(&request, 0, 0);

	while (FCGX_Accept_r(&request) == 0) {
		fcgi_streambuf cin_fcgi_streambuf(request.in);
		fcgi_streambuf cout_fcgi_streambuf(request.out);
		fcgi_streambuf cerr_fcgi_streambuf(request.err);

		std::istream is(&cin_fcgi_streambuf);
		std::ostream os(&cout_fcgi_streambuf);
		std::ostream es(&cerr_fcgi_streambuf);

		es << "Serving page\n";

		int lines = 0;
		std::string data;
		std::string line;
		while (getline(is, line)) {
			data += line;
			if (++lines > 20) break;
		}

		send_content_type( os, content::html );

		if (!line.empty())
			ctx.vars["content"] = line;
		else
			ctx.vars["content"] = (std::string)filler;

		cin.ignore(std::numeric_limits<int>::max());

		tpl->produce( os, ctx );

		char const* const* envp = request.envp;
		if (envp) while (*envp) {
			auto p = aw::string::split_off(*envp++, "=");
			if (p.first == "REQUEST_URI") {
				os << "<!--" << p.second << "-->\n";
				break;
			}
		}
	}

	return 0;
}
