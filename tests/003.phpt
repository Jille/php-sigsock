--TEST--
Two streams sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	$stream2 = sigsock_register(NULL, SIGUSR2, true);
	posix_kill(posix_getpid(), SIGUSR2);
	posix_kill(posix_getpid(), SIGUSR1);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR1);
	var_dump($s = ord(fread($stream2, 1)), $s === SIGUSR2);
?>
--EXPECTF--
int(%d)
bool(true)
int(%d)
bool(true)
