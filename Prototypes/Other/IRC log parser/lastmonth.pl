#!/usr/bin/perl -CS
use utf8;
use IPC::Open3;

my $filename = '#log_full.txt';
open(my $file, '<:encoding(UTF-8)', $filename);
open(my $parser, '|-:encoding(UTF-8)', './#log_parser 2>/dev/null');

# moth currently being experimented on
my $cut_moth;
@lines = reverse <$file>;
foreach $line (@lines) {
	if ($line =~ /^--- Day changed .. (...)/) {
		$cut_moth ||= $1;
		last if $1 ne $cut_moth;
	}
	print $parser $line;
}

#print $cut_moth;

close $parser;
close $file;

#
#while (<$file>) {
#	print if 1000..2000;
#}
