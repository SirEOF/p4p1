/*
 * C:\WINDOWS\system32\drivers\etc\hosts
 * could use this to create something that modifies the dns entries
 * to phishing site and such
 */
#ifndef _DEFAULT_H_
#define _DEFAULT_H_

#define DEFAULT_PORT 4441 // port to connect to
#define DEFAULT_IP "192.168.1.16"   // ip to connect to
#define DEFAULT_IP_SIZE (strlen(DEFAULT_IP) + 1)	// size of ip to allocate it later
#define TEXT_FILE_OFF
#define _DEBUG

#if defined(TEXT_FILE_ON)
	#define TEXT_CONTENT "helloWorld"
	#define FILE_NAME "nameofthefile.txt"
#endif
#endif
