/* Name: Renzo Moron-Svartz */
/* UID: 114145086 */
/* grace id: rsvartz */

#include <stdio.h>
#include <string.h>
#include "document.h"

#define TRUE 1
#define FALSE 0

static char *find_pattern(char *, const char *);
static void insert_replacement(char *, const char *);
static void shift_chars(char *, int , int);
static int is_blank(const char *);

/* Initiazilizes a document as empty, and gives it a name */
int init_document(Document *doc, const char *name) {

        /* Argument validation */
        if (!doc || !name || strlen(name) > MAX_STR_SIZE) {
                return FAILURE;
        }

        /* Initialize doc */
        doc->number_of_paragraphs = 0;
        strcpy(doc->name, name);

        return SUCCESS;
}

/* Sets the number of paragraphs in a doc to 0 */
int reset_document(Document *doc) {

        if (!doc) {
                return FAILURE;
        }

        doc->number_of_paragraphs = 0;

        return SUCCESS;
}

/* Print all paragraphs within a document */
int print_document(Document *doc) {
        int num_paragraphs = 0, num_lines = 0, i = 0, j = 0;
        Paragraph *current_paragraph_p = NULL;

        /* Validate docs */
        if (!doc) {
                return FAILURE;
        }

        num_paragraphs = doc->number_of_paragraphs;

        /* Print doc data */
        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", num_paragraphs);

        /* Establish a pointer to the paragraphs in doc */
        current_paragraph_p = doc->paragraphs;

        /* Print paragraphs */
        for (i = 0; i < num_paragraphs; i++) {
                num_lines = current_paragraph_p->number_of_lines;

                /* Print paragraph lines */
                for (j = 0; j < num_lines; j++) {

			printf("%s", current_paragraph_p->lines[j]);

			/* Print newlines depending on content of line */
			if (current_paragraph_p->
			lines[j][strlen(current_paragraph_p->lines[j]) - 1]
				!= '\n') {
				printf("\n");
			}
                        
			/* Print a newline between paragraphs */
			if (j == num_lines - 1 && i < num_paragraphs - 1) {
                                printf("\n");
                        }
                }

                /* Safely increment Paragraph pointer */
                if (i < num_paragraphs - 1) {
                        current_paragraph_p++;
                }
        }

        return SUCCESS;
}

/* Adds paragraph to the position specified by an argument to the document */
int add_paragraph_after(Document *doc, int paragraph_number) {
        int i = 0, num_paragraphs = 0;
        Paragraph *current_paragraph_p = NULL;
        Paragraph new_paragraph = {0};

        /* Validate doc, doc size, and requested position for insertion */
        if (!doc ||(num_paragraphs = doc->number_of_paragraphs) ==
        MAX_PARAGRAPHS || paragraph_number > num_paragraphs) {
                return FAILURE;
        }

        /* Establish access to document paragraphs array */
        current_paragraph_p = doc->paragraphs;

        /* Shift over Paragraphs to make room for new paragraph */
        for (i = 0; i < num_paragraphs - paragraph_number; i++) {
                *(current_paragraph_p + num_paragraphs - i) =
                        *(current_paragraph_p + num_paragraphs - 1 - i);
        }

        /* Insert new paragraph */
        current_paragraph_p[paragraph_number] = new_paragraph;

        /* Increment number of paragraphs */
        doc->number_of_paragraphs++;

        return SUCCESS;
}

/* Adds line to the position specified by an argument to the document */
int add_line_after(Document *doc, int paragraph_number, int line_number,
        const char *new_line) {
        int  num_lines = 0, i = 0;
        Paragraph *current_paragraph_p = NULL;

        /* Validate function arguments and define local variables above */
        if (!doc || paragraph_number > doc->number_of_paragraphs || 
		(current_paragraph_p = 
		&(doc->paragraphs[paragraph_number - 1]))->number_of_lines ==
                MAX_PARAGRAPH_LINES ||
                line_number > current_paragraph_p->number_of_lines ||
                !new_line) {
                return FAILURE;
        }

        num_lines = current_paragraph_p->number_of_lines;

        /* Shift over lines to make room for new line */
        for (i = 0; i < num_lines - line_number; i++) {

                strcpy(current_paragraph_p->lines[num_lines - i],
                       current_paragraph_p->lines[num_lines - 1 - i]);
        }


        /* Insert new line */
        strcpy(current_paragraph_p->lines[line_number], new_line);

        /* Increment number of lines */
        current_paragraph_p->number_of_lines++;

        return SUCCESS;
}

/* Returns the number of lines in a given paragraph */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
        int *number_of_lines) {

        /* Validates arguments */
        if (!doc || !number_of_lines || paragraph_number >
                doc->number_of_paragraphs) {
                return FAILURE;
        }

        *number_of_lines =
                doc->paragraphs[paragraph_number - 1].number_of_lines;

        return SUCCESS;
}

/* Append a line to a given paragraph */
int append_line(Document *doc, int paragraph_number,
        const char *new_line) {
        int num_lines = 0;

        /* Validate funtion arguments */
        if (!doc || paragraph_number > doc->number_of_paragraphs) {
                return FAILURE;
        }

        /* Locate line number to append new line at */
        num_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

        /* Effectively perform append_line by using add_line_after */
        return add_line_after(doc, paragraph_number, num_lines, new_line);
}

/* Removes a line from a specific paragraph in a document */
int remove_line(Document *doc, int paragraph_number, int line_number) {
        int i = 0, j = 0, num_lines = 0;
        Paragraph *current_paragraph_p = NULL;
        char *current_line = NULL;

        /* Validate function arguments */
        if (!doc || paragraph_number >
                doc->number_of_paragraphs || (current_paragraph_p =
                &(doc->paragraphs[paragraph_number - 1]))->number_of_lines ==
                MAX_PARAGRAPH_LINES || line_number > 
		(num_lines = current_paragraph_p->number_of_lines)) {
                return FAILURE;
        }

        /* Access line to be removed, remove line(s), and shift lines */
        for (i = line_number - 1; i < num_lines; i++) {

                /* Access line to be removed, remove line(s), and shift lines */
                current_line = current_paragraph_p->lines[i];

                /* Remove line */
                for (j = 0; j < MAX_STR_SIZE + 1; j++) {
                        current_line[j] = '\0';
                }

                /* Shift over line(s) */
                if (i < num_lines - 1) {
                        strcpy(current_line, current_paragraph_p->lines[i+1]);
                }
        }

        /* Decrement number of lines */
        current_paragraph_p->number_of_lines--;

        return SUCCESS;
}

/* Loads a specified number of lines from the data array to the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
	int data_lines) {
        int i = 0;
        int *num_paragraphs = NULL, *num_lines = NULL;

        /* Validate function arguments */
        if (!doc || !data || data_lines == 0) {
                return FAILURE;
        }

        /* By default create the first paragraph */
        add_paragraph_after(doc, 0);

        /* Establish access to the number of paragraphs */
        num_paragraphs = &(doc->number_of_paragraphs);

        /* Loop through data_lines */
        for (i = 0; i < data_lines; i++) {

                /* Establish access to number of lines in current paragraph */
                num_lines =
                       &(doc->paragraphs[*num_paragraphs - 1].number_of_lines);

                /* Create a new paragraph for all empty strings */
                if (!strlen(data[i])) {
                        add_paragraph_after(doc, *num_paragraphs);
                }
                else {
                        /* Add line to current paragraph */
                        add_line_after(doc, *num_paragraphs, *num_lines,
                                 data[i]);
                }
        }

        return SUCCESS;
}

/* Replaces target text in the document with replacement text */
int replace_text(Document *doc, const char *target, const char *replacement) {
        int i = 0, j = 0, target_len = 0, replacement_len = 0;
        char *current_line = NULL;
        Paragraph *current_paragraph_p = NULL;

        /* Validate function arguments */
        if (!doc || !target || !replacement) {
                return FAILURE;
        }

        target_len = strlen(target);
        replacement_len = strlen(replacement);

        /* Cycle through all paragraphs */
        for (i = 0; i < doc->number_of_paragraphs; i++) {
                
		/* Obtain access to current paragraph */
		current_paragraph_p = &(doc->paragraphs[i]);

                /* Cycle through all lines */
                for (j = 0; j < current_paragraph_p->number_of_lines; j++) {

                        /* Obtain access to current line */
                        current_line = current_paragraph_p->lines[j];

                        /* Pattern found */
                        while ((current_line = 
				find_pattern(current_line, target))) {

                                /* Shift characters to make space 
  				 * for replacement string */
                                shift_chars(current_line, target_len,
					replacement_len - target_len);

                                insert_replacement(current_line, replacement);
          
                                /* Increment by the length of inserted text */
                                current_line += replacement_len;  
                        }
                }
        }

        return SUCCESS;
}

/* Helper function finds patterns in a line and returns a pointer to
 *     the beginning of the pattern or NULL */
static char *find_pattern(char *current_line, const char *target) {
        int i = 0, j = 0, k = 0;

        /* Cycle through all characters in the current line */
        for (i = 0; current_line[i] != '\0'; i++) {

                /* Check if i-th character begins target string */
                for (j = i, k = 0; target[k] != '\0' &&
                        current_line[j] == target[k]; j++, k++) {
                	;        
                }

                /* Found target string in current line */
                if (k > 0 && target[k] == '\0') {

                        /* Return pointer to pattern */
                        return (&current_line[i]);
                }
        }

        return NULL;
}

/* Helper function shift characters forwards/backwards */
static void shift_chars(char *current_line, int target_len , int diff) {
        int new_length = strlen(current_line) + diff;
	int og_len = strlen(current_line);
	int i = 0, j = 0;

        /* Forward shift */
        if (diff > 0) {
          
                /* Shifts over characters to allow replacement string to fit */
                for (i = 0; i < diff; i++) {
                        for (j = og_len - 1 + i; j > target_len - 1 + i; j--) {
                                current_line[j+1] = current_line[j];
                        }
                }

                /* Null terminates the new line */
                current_line[new_length] = '\0';
        }
        
        /* Backward shift */
        else if (diff < 0) {
          
                /* Shift characters to fit replacement string properly */
                for (i = 0; i < diff * -1; i++) {
			for (j = target_len - i; j <= og_len - i; j++) {
                                current_line[j - 1] = current_line[j];
                        }
                }
        }

}

/* Helper function inserts replacement text into char pointer */
static void insert_replacement(char *current_line, const char *replacement) {
        int i = 0, replacement_len = strlen(replacement);
        
        /* Insert replacement line */
        for (i = 0; i < replacement_len; i++) {
                current_line[i] = replacement[i];
        }
}

/* Highlights target strings in the doc using "[" and "]" */
int highlight_text(Document *doc, const char *target) {
        char replacement_text[MAX_STR_SIZE] = HIGHLIGHT_START_STR;

        /* Validate function arguments */
        if (!doc || !target) {
                return FAILURE;
        }

        /* Create replacement text string */
        strcat(replacement_text, target);
        strcat(replacement_text, HIGHLIGHT_END_STR);

        /* Effectively highlight target using replacement text */
        replace_text(doc, target, replacement_text);

        return SUCCESS;
}

/* Removes target text from a document */
int remove_text(Document *doc, const char *target) {
        const char *replacement_text = "";
  
        /* Validate function arguments */
        if (!doc || !target) {
                return FAILURE;
        }

        /* Effectively remove target using empty string */
        replace_text(doc, target, replacement_text);

        return SUCCESS;
}

/* Loads the contents of a file into the Document argument */
int load_file(Document *doc, const char *filename) {
	FILE *file = NULL;
	char current_line[MAX_STR_SIZE + 1] = "";
	int current_paragraph = 0;
        int *num_paragraphs = NULL, *num_lines = NULL;

        /* Validate function arguments */
        if (!doc || !filename) {
                return FAILURE;
        }

	/* Access file */
	file = fopen(filename, "r");
	if (!file) {
		return FAILURE;
	}

	/* Establish access to the number of paragraphs */
        num_paragraphs = &(doc->number_of_paragraphs);

	/* Check paragraph space availability */
	if (*num_paragraphs > MAX_PARAGRAPHS) {
		fclose(file);
		return FAILURE;
	}

        /* By default create the first paragraph */
       	add_paragraph_after(doc, current_paragraph++);

        /* Loop through data_lines in the document */
        while(fgets(current_line, MAX_STR_SIZE + 1, file)) {

                /* Establish access to number of lines in current paragraph */
                num_lines =
               	&(doc->paragraphs[current_paragraph - 1].number_of_lines);

		/* Check paragraph and line space availability */
                if (*num_paragraphs > MAX_PARAGRAPHS
                        || *num_lines > MAX_PARAGRAPH_LINES) {

			fclose(file);
                        return FAILURE;
                }

                /* Create a new paragraph for all blank strings */
                if (is_blank(current_line)) {
                        add_paragraph_after(doc, current_paragraph++);
                }
                else {
                        /* Add line to current paragraph */
                        add_line_after(doc, current_paragraph, *num_lines,
                                 current_line);
                }
        }

	fclose(file);
        return SUCCESS;
}

/* Saves (prints) the contents of the Document to the file */
int save_document(Document *doc, const char *filename) {
	FILE *file = NULL;
	int num_paragraphs = 0, num_lines = 0, i = 0, j = 0;
        Paragraph *current_paragraph_p = NULL;

        /* Validate arguments */
        if (!doc || !filename) {
                return FAILURE;
        }

	/* Access file */
        file = fopen(filename, "w");
        if (!file) {
                return FAILURE;
        }

	/* Establish access to number of paragraphs in the Document */
        num_paragraphs = doc->number_of_paragraphs;

        /* Establish a pointer to the paragraphs in doc */
        current_paragraph_p = doc->paragraphs;

        /* Print paragraphs */
        for (i = 0; i < num_paragraphs; i++) {
                num_lines = current_paragraph_p->number_of_lines;

		/* Print paragraph lines */
                for (j = 0; j < num_lines; j++) {

                        fprintf(file, "%s", current_paragraph_p->lines[j]);

                        /* Print newlines depending on content of line */
                        if (current_paragraph_p->
                        lines[j][strlen(current_paragraph_p->lines[j]) - 1]
                                != '\n') {
                                fprintf(file, "\n");
                        }

                        /* Print a newline between paragraphs */
                        if (j == num_lines - 1 && i < num_paragraphs - 1) {
                                fprintf(file, "\n");
                        }
                }

                /* Safely increment Paragraph pointer */
                if (i < num_paragraphs - 1) {
                        current_paragraph_p++;
                }

        }

	fclose(file);
	return SUCCESS;

}

/* Helper funcion determines if a line is blank. Returns TRUE if blank */
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
