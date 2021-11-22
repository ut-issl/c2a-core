use strict;
use warnings;
use utf8;

package Utils;

use Time::Local;
use Time::HiRes qw/ gettimeofday /;

sub GetStringTime {

	my ($epocsec, $microsec) = gettimeofday();
	my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime($epocsec);
	$year += 1900;
	$mon += 1;
	#printf("%020d:\n",12345678);
	# return sprintf("%04d.%02d.%02d-%02d.%02d.%02d.%06d",$year,$mon,$mday,$hour,$min,$sec,$microsec);
	return sprintf("%04d-%02d-%02d_%02d-%02d-%02d",$year,$mon,$mday,$hour,$min,$sec);

}


sub CreateLogFile {
	my $outputFile = $_[0];

	open(FH,"> ".$outputFile) or die("Error:$!");
	close(FH);
}


sub OutputFile {
	my $outputFile = $_[0];
	my $outputStr  = $_[1];

	open(FH, ">> ".$outputFile) or die("Error:$!");
	print FH $outputStr;
	close(FH);
}


1;
