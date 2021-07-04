// Can BEYAZNAR
// 161044038

1- file names and directory names must not include '?' character. I handle this. If '?' If it includes, I quit the program.

2- file system file should not be empty. It should not be sent empty.

3- While trying my makefile file in Virtual Machine, I get the error "clock skew detected. Your build may be incomplete". To solve this, I wrote the following two commands to the terminal:

	1- sudo apt install ntp
	2- touch *

	After these two commands, my makefile file does not fail. If it still fails, please manually compile it.

4- Files sent by the user should not contain extended ascii characters. The way the program works is broken. Also, the file and directory names entered by the user should not contain extended ascii character.

5- In Part 2, if the user enters the number of inodes above the limit (specified in the report), the program accepts the maximum number of inodes that it can receive as input.

6- The file sent by the user should not be empty. If empty, an error is sent.


