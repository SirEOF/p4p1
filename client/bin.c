#include "bin.h"

int cd(char *buf)				// change directory function
{						// processes the command buffer
	int i, z;				// to change directory
	int len;
	char ch;
	char *path;

	i = len = z = ch = 0;
	if(buf[0] == 'c' && buf[1] == 'd') {	// cd command hasent been removed
		for(len = 3; len < strlen(buf); len++) // start at space and get the length
			;
		path = malloc(len * sizeof(char));	// create a path that is a special size
		for(i = 3; i < strlen(buf); i++)	// for the buff.
			path[z++] = buf[i];		// cp the buffer char to the path
	}
	chdir(path);			// change directory part.
	free(path);			// free alocated path after moving.
}

int exe(char *cmd, char *r_buf)			// execute commands that are unknown to the
{						// shell.
	FILE * cmd_f = _popen(cmd, "r");	// open the terminal for commands
	char ch;
	int i;

	i = ch = 1;
	if(cmd_f == NULL) {
		r_buf = malloc((23+strlen(cmd)) * sizeof(char));	// create a string of size 23
		sprintf(r_buf, "[Err]Command %s : not found!\n", cmd);	// to tell that <- command not found
	} else {
		while((ch = fgetc(cmd_f)) != EOF)			// get initial size of the buffer
			i++;
		printf("len of cmd : %d\n", i);
		r_buf = malloc(i * sizeof(char));			// allocate it
		if(r_buf == NULL)					// process errors
			return -1;
		i = 0;
		rewind(cmd_f);						// rewind the file to
		while((ch = fgetc(cmd_f)) != EOF) {			// write back out the buffer to the given
			r_buf[i++] = ch;				// char *
			putchar(ch);
		}
		r_buf[i] = '\0';
	}
	_pclose(cmd_f);							// close terminal
	 return 0;
}