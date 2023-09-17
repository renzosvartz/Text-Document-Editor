/* Name: Renzo Alejo Moron-Svartz */
/* UID: 114145086 */
/* grace id: rsvartz */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include "document.h"

#define MAX_LINE 1024
#define MAX_DIGITS 2
#define CONTINUE 1
#define END 0
#define TRUE 1
#define FALSE 0

enum Command {ADD_PARAGRAPH_AFTER, ADD_LINE_AFTER, PRINT_DOCUMENT, QUIT,
		EXIT, APPEND_LINE, REMOVE_LINE, LOAD_FILE, REPLACE_TEXT,
		HIGHLIGHT_TEXT, REMOVE_TEXT, SAVE_DOCUMENT, RESET_DOCUMENT};

static void read_from_stdin();
static void read_from_file(const char *);
static int process_arg_1(const char *, Document *);
static int process_arg_2(const char *, Document *, int);
static void process_arg_3(const char *, Document *, int);
static void process_append(const char *, Document *);
static void get_line(const char *, char *);
static int get_quoted_text(const char *, char *, int);
static void clear_input_buffer();
static int is_blank(const char *);

/* Executes the proper code corresponding to the command-line usage */
int main(int argc, char *argv[]) {

	/* Determines which program to run corresponding to command-line arg */	
	if (argc == 1) {
		read_from_stdin();
	}
	else if (argc == 2) {
		read_from_file(argv[1]);
	}
	else {
		
		fprintf(stderr, 
		"Usage: user_interface\nUsage: user_interface <filename>\n");
		
		exit(EX_USAGE);
	}

	return SUCCESS;
}

/* Reads lines and processes them from stdin */
static void read_from_stdin() {
	Document doc = {0};
	char current_line[MAX_LINE + 1] = "";
	int current_state = CONTINUE;

	/* Initialize main document */
	init_document(&doc, "main_document");

	/* Continously reads line from stdin and processes it */
	while (current_state == CONTINUE) {

		/* Prompt */
		printf("> ");

		/* Read a line from the stdin */
		if (fgets(current_line, MAX_LINE, stdin)) {
			
			/* Clear input buffer */
			if (current_line[strlen(current_line) - 1] != '\n') {
				clear_input_buffer();
			}
			
			/* Process line text */
			current_state = process_arg_1(current_line, &doc);

		}
		else {

			/*  EOF encountered */
			current_state = END;

		}
	}
}

/* Reads lines and processes them from an input file */
static void read_from_file(const char *filename) {
	Document doc = {0};	
	FILE *file = NULL;
	char current_line[MAX_LINE + 1] = "";
	int current_state = CONTINUE;

	/* Initialize main document */
	init_document(&doc, "main_document");

	/* Validates file opening/access */
	if (!(file = fopen(filename, "r"))) {

		fprintf(stderr, "%s cannot be opened.\n", filename);
		exit(EX_OSERR);

	}

	/* Continuously reads line from file and processes it */
	while (current_state == CONTINUE) {

		/* Read a line from the file */
		if (fgets(current_line, MAX_LINE, file)) {

			/* Clear input buffer */
                        if (current_line[strlen(current_line) - 1] != '\n') {
                                clear_input_buffer();
                        }

			/* Process line text */
			current_state = process_arg_1(current_line, &doc);	

		}
		else {

			/* EOF encountered */
			current_state = END;
			
			/* Close file */
			fclose(file);

		}
	}

}

/* Processes lines read from the input stream */
static int process_arg_1(const char *current_line, Document *doc_ptr) {
	char command[MAX_LINE + 1] = "";

	/* Blank line encountered */
	if (is_blank(current_line)) {

		return CONTINUE;

	}
	else {
	
		/* Read first argument/command */
		sscanf(current_line, "%s", command);

	}
	
	/* Process argument 1 */
	if (command[0] == '#') {
		
		/* Comment read */		
		return CONTINUE;

	}
	else if (strcmp(command, "add_paragraph_after") == 0) {
		
		/* Process argument 2 */
		return process_arg_2(current_line, doc_ptr, ADD_PARAGRAPH_AFTER);

	}
	else if (strcmp(command, "add_line_after") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, ADD_LINE_AFTER);

        }
	else if (strcmp(command, "print_document") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, PRINT_DOCUMENT);

        }
	else if (strcmp(command, "quit") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, QUIT);

        }
	else if (strcmp(command, "exit") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, EXIT);

        }
	else if (strcmp(command, "append_line") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, APPEND_LINE);

        }
	else if (strcmp(command, "remove_line") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, REMOVE_LINE);

        }
	else if (strcmp(command, "load_file") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, LOAD_FILE);

        }
	else if (strcmp(command, "replace_text") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, REPLACE_TEXT);

        }
	else if (strcmp(command, "highlight_text") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, HIGHLIGHT_TEXT);

        }
	else if (strcmp(command, "remove_text") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, REMOVE_TEXT);

        }
	else if (strcmp(command, "save_document") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, SAVE_DOCUMENT);

        }
	else if (strcmp(command, "reset_document") == 0) {

                /* Process argument 2 */
                return process_arg_2(current_line, doc_ptr, RESET_DOCUMENT);

        }
	else {
	
		/* Invalid Command */
                fprintf(stdout, "Invalid Command\n");
                return CONTINUE;
	
	}	
}


static int process_arg_2(const char *current_line, Document *doc_ptr,
	int cmd) {
	char prev_data[MAX_LINE + 1] = "";
	char excess_data[MAX_LINE + 1] = "";
	char file_name[MAX_STR_SIZE + 1] = "";
	char target[MAX_STR_SIZE + 1] = "";
	char replacement[MAX_STR_SIZE + 1] = "";
	int values_read = 0;
	int paragraph_num = 0;
	int chars_read = 0;

	/* Validate command inputs based on which command was called */
	if (cmd == ADD_PARAGRAPH_AFTER) {
	
		/* Validate add_paragraph_after input format */
		if ((values_read = sscanf(current_line, "%s %d %s", prev_data, 
			&paragraph_num, excess_data)) == 1
			|| values_read == 3) {
		
			fprintf(stdout, "Invalid Command\n");

		}
		else {
			
			/* Validate paragraph number and perform command */
			if (paragraph_num < 0) {
	
				fprintf(stdout, "Invalid Command\n");

			}
			else if (add_paragraph_after(doc_ptr, paragraph_num) 
					== FAILURE) {

				fprintf(stdout, 
					"add_paragraph_after failed\n");
			}
		
		}

	}
	else if (cmd == ADD_LINE_AFTER || cmd == APPEND_LINE
			|| cmd == REMOVE_LINE) {

                /* Validate command input format */
                if ((values_read = sscanf(current_line, "%s %d", prev_data,
                        &paragraph_num)) == 1) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

                        /* Validate paragraph number and perform command */
                        if (paragraph_num <= 0) {

                                fprintf(stdout, "Invalid Command\n");

                        }
			else {
				
				/* Process argument 3 based on command */
				if (cmd == ADD_LINE_AFTER) {
				
					process_arg_3(current_line, 
						doc_ptr, ADD_LINE_AFTER); 
				
				}
				else if (cmd == APPEND_LINE) {

                                        process_append(current_line, doc_ptr);

                                }
				else if (cmd == REMOVE_LINE) {

                                        process_arg_3(current_line,
                                                doc_ptr, REMOVE_LINE);

                                }
			
                        }
         
       		}

        }
	else if (cmd == PRINT_DOCUMENT || cmd == QUIT || cmd == EXIT
			|| cmd == RESET_DOCUMENT) {

                /* Validate command input format */
                if ((values_read = sscanf(current_line, "%s %s", prev_data,
                        excess_data)) == 2) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

			/* Perform corresponding command */
			if (cmd == QUIT || cmd == EXIT) {
			
				return END;

			}
			else if (cmd == PRINT_DOCUMENT) {
			
				print_document(doc_ptr);			

			}
			else if (cmd == RESET_DOCUMENT) {
		
				reset_document(doc_ptr);
				
			}

                }

        }
	else if (cmd == LOAD_FILE || cmd == SAVE_DOCUMENT) {
                
		/* Validate command input format */
                if ((values_read = sscanf(current_line, "%s %s %s", prev_data,
                        file_name, excess_data)) == 1
                        || values_read == 3) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

                        /* Validate the command functioning/execution */
                        if (cmd == LOAD_FILE) {

				if (load_file(doc_ptr, file_name) == FAILURE) {
                                
					fprintf(stdout, "load_file failed\n");
				}

                        }
			else if (cmd == SAVE_DOCUMENT) {
			
				if (save_document(doc_ptr, file_name) == FAILURE) {

                                        fprintf(stdout, "save_document failed\n");
				}
			}

                }

        }
	else if (cmd == HIGHLIGHT_TEXT || cmd == REMOVE_TEXT) {
		
                /* Validate/Extract target text from current line */
          	if (get_quoted_text(current_line, target, 0) == FAILURE) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

			/* Execute command */
                        if (cmd == HIGHLIGHT_TEXT) {

				highlight_text(doc_ptr, target);
	
                        }
                        else if (cmd == REMOVE_TEXT) {

				remove_text(doc_ptr, target);

                        }
		
		}

	}
	else if (cmd == REPLACE_TEXT) {
	
		/* Validate/Extract target text from current line */
                if ((chars_read = get_quoted_text(current_line, target, 0)) 
			== FAILURE) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

			/* Validate/Extract replacement text */
			if (get_quoted_text(current_line, replacement,
				chars_read) == FAILURE) {

                        	fprintf(stdout, "Invalid Command\n");

                	}
			else if (replace_text(doc_ptr, target, replacement)
				== FAILURE) {
				
				fprintf(stdout, "replace_text failed\n");
	
			}

                }

	}

	return CONTINUE;
}

/* Process command's third argument and ensure validity of command */
static void process_arg_3(const char *current_line, Document *doc_ptr,
	int cmd) {
	char prev_data[MAX_LINE + 1] = "";
	char excess_data[MAX_LINE + 1] = "";
	char line[MAX_STR_SIZE + 1] = "";
	char star = '\0';
	int paragraph_num = 0;
	int line_num = 0;
	int values_read = 0;

	if (cmd == REMOVE_LINE) {

                /* Validate command input format */
                if ((values_read = sscanf(current_line, "%s %d %d %s", prev_data,
                        &paragraph_num, &line_num, excess_data)) != 3) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

			/* Validate line number and perform command */
                        if (line_num <= 0) {

                                fprintf(stdout, "Invalid Command\n");

                        }
                        else {
				
				/* Execute command */
				if (remove_line(doc_ptr, paragraph_num,
					line_num) == FAILURE) {
					
					fprintf(stdout, 
						"remove_line failed\n");

				}
	
			}
	
		}

	}
	else if (cmd == ADD_LINE_AFTER) {
	
		/* Validate command input format */
                if ((values_read = sscanf(current_line, "%s %d %d %c", prev_data,
                        &paragraph_num, &line_num, &star)) != 4) {

                        fprintf(stdout, "Invalid Command\n");

                }
                else {

                        /* Validate paragraph number and perform command */
                        if (line_num < 0 || star != '*') {

                                fprintf(stdout, "Invalid Command\n");

                        }
                        else {

				/* Extract Line to add */
				get_line(current_line, line);

				/* Execute command */
                                if (add_line_after(doc_ptr, paragraph_num,
                                        line_num, line) == FAILURE) {

                                        fprintf(stdout,
                                                "add_line_after failed\n");

                                }

                        }

                }
			
	}

}

/* Process append_line command */
static void process_append(const char *current_line, Document *doc_ptr) {
	char prev_data[MAX_LINE + 1] = "";
	char line[MAX_STR_SIZE + 1] = "";
	char star = '\0';
	int paragraph_num = 0;
	int values_read = 0;
	

	/* Validate command input format */
        if ((values_read = sscanf(current_line, "%s %d %c", prev_data,
                        &paragraph_num, &star)) != 3) {

            	fprintf(stdout, "Invalid Command\n");

        }
        else {

               	/* Validate paragraph number and perform command */
                if (star != '*') {

                  	fprintf(stdout, "Invalid Command\n");

                }
                else {
			/* Extract Line to append */
                       	get_line(current_line, line);

                        /* Execute command */
                        if (append_line(doc_ptr, paragraph_num, line) 
				== FAILURE) {

                                        fprintf(stdout,
                                                "append_line failed\n");

                        }

		}
	}
}

/* Extracts line to be added or appended from line being read */
static void get_line(const char *current_line, char *line) {
	int i = 0, j = 0;

	/* Skip characters until * is found */
        while (current_line[i] != '*' && current_line[i] != '\n'
                && current_line[i] != '\0') {
                i++;
        }

	/* LINE found */
        if (current_line[i++] == '*') {
	
		/* Collect LINE */
                while (current_line[i] != '\n' && current_line[i] != '\0') {
                        line[j++] = current_line[i++];
                }

		line[j] = '\0';

	}

}

/* Extracts quoted text from a line of text.
 * Returns number of characters read to reach first quoted text 
 * starting at address current_line + skip, or FAILURE if text not found */
static int get_quoted_text(const char *current_line, char *text, int skip) {
	int i = skip, j = 0;

	/* Count characters skipped until double quote is found */
	while (current_line[i] != '"' && current_line[i] != '\n'
		&& current_line[i] != '\0') {
		i++;
	}


	/* Collect quoted text and return characters read or return FAILURE */
	if (current_line[i++] == '"') {
	
		
		/* Collect quoted text */
		while (current_line[i] != '"' && current_line[i] != '\n'
                && current_line[i] != '\0') {
                	text[j++] = current_line[i++];
        	}

		/* Quoted text found. Return characters read */
		if (current_line[i++] == '"') {
			
			text[j] = '\0';
			return i;
	
		}

	}
	
	/* Quoted text not found */
	return FAILURE;
}

/* Clear input buffer */
static void clear_input_buffer() {
	int c = 0;

  	while ((c = getchar()) != EOF && c != '\n') {
		;
	}
}

/* Determines whether a line is blank or not. Returns TRUE if blank */
static int is_blank(const char *line) {
        int i = 0;

        while (line[i] == '\t' || line[i] == ' ' || line[i] == '\n') {
                i++;
        }

        if (line[i] == '\0') {
                return TRUE;
        }
        else {
                return FALSE;
        }

}
