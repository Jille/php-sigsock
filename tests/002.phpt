--TEST--
Signal queueing sigsock test
--FILE--
<?php
	$stream = sigsock_register(NULL, SIGUSR1, true);
	posix_kill(posix_getpid(), SIGUSR1);
	posix_kill(posix_getpid(), SIGUSR1);
	var_dump(ord(fread($stream, 1)));
	var_dump(ord(fread($stream, 1)));
?>
--EXPECTF--
int(%d)
int(%d)
