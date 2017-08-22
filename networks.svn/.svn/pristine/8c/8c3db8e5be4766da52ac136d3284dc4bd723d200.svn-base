#!/usr/bin/perl


# 
# diff program for cs424 program 2
# author: b rosenberg
# date: 11 feb 2016
# last update: 11 feb 2016
#

$machine1 = shift @ARGV ;
$machine2 = shift @ARGV ;
$fn = shift @ARGV ;
#$fo = shift @ARGV ;

open (F, $fn) || die "Can not open $fn" ; 
#open (G, ">".$fo) || die "Can not open $fo" ;


sub checkhostnames {
	my ( $i, @hns ) = @_ ;
	foreach $n (@hns) {
		if ( $n =~ /^(($machine1)|($machine2)|(localhost))$/ ) {
		}
		else {
			printf "%d ERROR unknown machine -> %s\n", $i, $n ;
		}
	}
}

$i = 1 ;
while ( $l = <F> ) {
	if ( $l =~ /(.*):\s+([\d.]+):(\d+)\s+(.*)\s*$/ ) {
		($t, $h, $p, $rest) = ($1, $2, $3, $4) ;

		if ( $rest =~ /\|(.*)\|/ ) {
			@hns = split /:/,$1 ;
			checkhostnames ($i, @hns) ;
			printf "%d type:%s host: %s port: %s message:%s\n", $i, 
				$t, $h, $p, $rest ;
		}
		else {
			chop $l ;
			printf "%d ERROR <%s>\n", $i, $rest ; 
		}
	}
	else {
		chop $l ;
		printf "%d ERROR <%s>\n", $i, $l ;
	}
	$i = $i + 1 ;
}

