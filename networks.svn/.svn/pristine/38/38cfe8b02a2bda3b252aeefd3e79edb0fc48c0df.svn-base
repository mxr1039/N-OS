#!/usr/bin/perl

$infile = "test.in" ;
$outfile = "test.out" ;

open (F, $infile) || die "cannot open $infile" ;
while ( $l =<F> ) {
	chop $l ;
	$cmd = "./arrange-h-e ".$l ;
	system($cmd) ;
}

