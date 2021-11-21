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
	$SETTING{'LOG_FILE'}  = './log.log';
	$SETTING{'SEARCH_PATH'} = [
		'Applications',
		'CmdTlm',
		'Core',
		'Drivers',
		'IfWrapper',
		'Library',
		# 'Modes'
	];

	# スキップファイル
	$SETTING{'IGNORE_FILE'} = [
		# 'obc_time.c',
		# 'time_manager.c'
		'utility.c',
		'print.c',
		'MRAM.c',
		'WDTMW.c',			# 今はもうない
		'watchdog_timer.c',
		'wdt_issl6u_mobc.c',
		'XACT_STT_IMG.c'
	];

	return %SETTING;
}

1;
