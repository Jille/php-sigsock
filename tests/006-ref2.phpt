--TEST--
Dereferencing #2 sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	$stream2 = sigsock_register($stream, SIGUSR2, true);
	var_dump($stream);
	var_dump($stream2);
	unset($stream2);
	var_dump($stream);
	posix_kill(posix_getpid(), SIGUSR2);
	posix_kill(posix_getpid(), SIGUSR1);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR2);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR1);
?>
--EXPECTF--
resource(%d) of type (stream)
resource(%d) of type (stream)
resource(%d) of type (stream)
int(%d)
bool(true)
int(%d)
bool(true)
