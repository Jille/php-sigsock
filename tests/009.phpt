--TEST--
Override attached signal with new stream sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	$stream2 = sigsock_register($stream, SIGUSR2, true);
	var_dump($stream == $stream2);
	$stream3 = sigsock_register(NULL, SIGUSR1, true);
	var_dump($stream == $stream2);
	var_dump($stream != $stream3);
	var_dump($stream);
	var_dump($stream3);
	posix_kill(posix_getpid(), SIGUSR2);
	var_dump($s = ord(fread($stream, 1)), $s === SIGUSR2);
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
resource(%d) of type (stream)
resource(%d) of type (stream)
int(%d)
bool(true)
