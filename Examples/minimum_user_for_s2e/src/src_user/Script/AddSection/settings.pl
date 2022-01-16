use strict;
use warnings;
use utf8;

# package sub;

sub GetSetting {
	print "Laod Settings...\n";

	my %SETTING;

	$SETTING{'HEADER'} = '#pragma section REPRO';
	$SETTING{'FOOTER'} = '#pragma section';

	$SETTING{'ROOT_PATH'} = '../../';
	$SETTING{'FILE_ENCODING'} = 'utf8';
	$SETTING{'LOG_FILE'}  = './log.log';
	$SETTING{'SEARCH_PATH'} = [
		'Applications',
		'Drivers',
		'IfWrapper',
		'Library',
		'Settings',
		'TlmCmd'
	];

	# スキップファイル
	$SETTING{'IGNORE_FILE'} = [
		'print.c',
		'watchdog_timer.c',
		'wdt_sils.c'
	];

	return %SETTING;
}

1;
