--TEST--
Override signal with new stream sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	$stream2 = sigsock_register(NULL, SIGUSR1, true);
	var_dump($stream);
	var_dump($stream2);
	var_dump($stream != $stream2);
	posix_kill(posix_getpid(), SIGUSR1);
	var_dump($s = ord(fread($stream2, 1)), $s === SIGUSR1);
?>
--EXPECTF--
resource(%d) of type (stream)
resource(%d) of type (stream)
bool(true)
int(%d)
bool(true)
