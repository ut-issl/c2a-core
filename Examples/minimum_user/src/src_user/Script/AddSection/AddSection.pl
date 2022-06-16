use strict;
use warnings;
use utf8;
binmode STDIN,  ':encoding(cp932)';
binmode STDOUT, ':encoding(cp932)';
binmode STDERR, ':encoding(cp932)';
# use Tie::File;
use File::Basename;

require './settings.pl';
require './utils.pl';

# print &Utils::GetStringTime()."\n";
&p(&Utils::GetStringTime());
my %SETTING = &GetSetting();

my $HEADER = $SETTING{'HEADER'};
my $FOOTER = $SETTING{'FOOTER'};

# &Utils::CreateLogFile($SETTING{'LOG_FILE'});		# 追記にしたのでコメントアウト
&Utils::OutputFile($SETTING{'LOG_FILE'}, &Utils::GetStringTime()."\n");


&p("");
&p('Searching...');

# .cを全探索
# &ProcFileRecursive('..', \&ProcFile);		# test
foreach (@{$SETTING{'SEARCH_PATH'}}) {
	&ProcFileRecursive($SETTING{'ROOT_PATH'}.$_, \&ProcFile);
}

exit;

sub ProcFileRecursive {
	my $root     = $_[0];
	my $callback = $_[1];

	my $dir = $root;
	# カレントディレクトリのファイル一覧
	opendir(DIR, $dir) or die("can't open directory: $dir ($!)");
	my @list = readdir(DIR);
	# &plist(@list);
	closedir(DIR);

	foreach (sort @list) {
		next if ( $_ =~ /^\.{1,2}$/ );		 # '.' と '..' はスキップ

		my $file = "$dir/$_";
		# ディレクトリの場合は自分自身を呼び出す
		if ( -d $file ) {
			&ProcFileRecursive($file, \&ProcFile);
		} else {
			next if !($file =~ /\.c$/);
			# next if  (grep {$_ eq $filename} @{$SETTING{'IGNORE_FILE'}});
			&{$callback}($file);
		}
 	}
}

sub ProcFile {
	my $file = $_[0];
	# &p($file);
	# print "      ".&ReadHeadLine($file);
	# print "      ".&ReadLastLine($file);

	my $header = &ReadHeadLine($file);
	my $footer = &ReadLastLine($file);

	if (grep {$_ eq basename($file)} @{$SETTING{'IGNORE_FILE'}}) {
		# いぐのあ
		my $flag = 0;
		# &p("いぐのあ！！！！");
		if ($header eq $HEADER) {
			EraseHeadLine($file);
			$flag = 1;
		}
		if ($footer eq $FOOTER) {
			EraseLastLine($file);
			$flag = 1;
		}
		if ($flag == 1) {
			&p("Delete $file");
			&Utils::OutputFile($SETTING{'LOG_FILE'}, "[Delete]\t$file"."\n");
		}
	} else {
		# いぐのあじゃない
		my $flag = 0;
		# &p("いぐのあじゃない！！！！");
		if ($header ne $HEADER) {
			AddHeadLine("$HEADER\n", $file);
			$flag = 1;
		}
		if ($footer ne $FOOTER) {
			AddLastLine("$FOOTER\n", $file);
			$flag = 1;
		}
		if ($flag == 1) {
			&p("Insert $file");
			&Utils::OutputFile($SETTING{'LOG_FILE'}, "[Insert]\t$file"."\n");
		}
	}
}

sub ReadHeadLine {
	my $file = $_[0];
	# open (FH, "<:encoding(cp932)", $file) || die "can't open $file: $!";
	open (FH, "<:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't open $file: $!";
	my $line = <FH>;
	# $line =~ s/ /_/g;
	# seek FH, 0, 0; # go back to the start of the file
	# printf FH $line;
	close FH;
	chomp($line);
	return $line;
}

sub ReadLastLine {
	my $file = $_[0];
	# print $_[0], "\n";
	open (FH, "<:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't open $file: $!";
	my $line;
	while (<FH>) {
		eof $_ and $line = $_;
	}
	close FH;
	chomp($line);
	return $line;
}

sub EraseHeadLine {
	my $file = $_[0];
	open (FH, "+<:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't open $file: $!";
	my @lines=<FH>;
	shift(@lines);
	seek(FH, 0, 0);
	truncate(FH, tell);
	print FH @lines;
	close FH;
}

sub EraseLastLine {
	my $file = $_[0];
	open (FH, "+<:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't update $file: $!";
	my $addr;
	while (<FH>) {
		$addr = tell(FH) unless eof(FH);
	}
	truncate(FH, $addr) or die "can't truncate $file: $!";
	close FH;
}

sub AddHeadLine {
	my $str  = $_[0];
	my $file = $_[1];
	open (FH, "+<:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't update $file: $!";

	my @orig = <FH>;
	seek(FH, 0, 0);
	print FH $str;
	print FH @orig;
	close FH;
}

sub AddLastLine {
	my $str  = $_[0];
	my $file = $_[1];
	open (FH, ">>:encoding(".$SETTING{'FILE_ENCODING'}.")", $file) or die "can't update $file: $!";
	print FH $str;
	close FH;
}

sub plist {
	my @Str = @_;
	print join("\n", @Str), "\n";
	return 1;
}

sub pjoin {
	my @Str = @_;
	print join("_", @Str), "\n";
	return 1;
}

sub p {
	my ($str) = @_;
	print $str, "\n";
	return 1;
}
