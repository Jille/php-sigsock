--TEST--
Stream attach sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	$stream2 = sigsock_register($stream, SIGUSR2, true);
	var_dump($stream == $stream2);
	posix_kill(posix_getpid(), SIGUSR2);
	posix_kill(posix_getpid(), SIGUSR1);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR2);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR1);
?>
--EXPECTF--
bool(true)
int(%d)
bool(true)
int(%d)
bool(true)
