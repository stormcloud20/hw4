#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char names[20];
char nextChar;
int lexLen;
int token;
int nextToken;
int current;
int tcount = 0;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
void expr(void);
void term(void);
void factor(void);
void error(void);
void error2(void);
void program(void);
int lex(void);

char **KEYWORDS = {"for", "if", "else", "while", "do", "int", "float", "switch", "class", "int", "void", "bool"};

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define getName(var, names) sprintf(names, "%s", #var)
#define ASSIGN_OP 7
#define ADD_OP 8
#define SUB_OP 9
#define MULT_OP 10
#define DIV_OP 11
#define MOD_OP 12
#define LESS_THAN 13
#define LESS_EQUAL 14
#define GREATER_THAN 15
#define GREATER_EQUAL 16
#define EQUAL_TO 17
#define NOT_EQUAL_TO 18
#define EXCLAMATION 19
#define IDENT 20
#define KEYWORD 21
#define LEFT_PARENT 22
#define RIGHT_PARENT 23
#define COMMA 24
#define SEMICOLON 25
#define FOR_STMT 26
#define LEFT_BRCKT 27
#define RIGHT_BRCKT 28
#define DOT 29
#define SEMI 30
#define IF_STMT 31
#define ELSE_STMT 32
#define WHILE_LOOP 33
#define DO_LOOP 34
#define INT_TYPE 35
#define FLOAT_TYPE 36

/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	if ((in_fp = fopen("front", "r")) == NULL)
		printf("ERROR - cannot open front.in \n");
	else {
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	}
	return 0;
}

/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PARENT;
		getName(LEFTP, names);
		tcount++;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PARENT;
		getName(RIGHTP, names);
		tcount++;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		getName(ADD, names);
		tcount++;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		getName(SUB, names);
		tcount++;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		getName(MULT, names);
		tcount++;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		getName(DIV, names);
		tcount++;
		break;
	case '~':
		addChar();
		nextToken = ASSIGN_OP;
		getName(ASSIGN, names);
		tcount++;
		break;
	case '%':
		addChar();
		nextToken = MOD_OP;
		getName(MOD, names);
		tcount++;
		break;
	case ';':
		addChar();
		nextToken = SEMICOLON;
		getName(SEMI, names);
		tcount++;
		break;
	case ',':
		addChar();
		nextToken = COMMA;
		getName(COMMA, names);
		tcount++;
		break;
	case '{':
		addChar();
		nextToken = LEFT_BRCKT;
		getName(LEFTB, names);
		tcount++;
		break;
	case '}':
		addChar();
		nextToken = RIGHT_BRCKT;
		getName(RIGHTB, names);
		tcount++;
		break;
	case '.':
		addChar();
		nextToken = DOT;
		getName(DOT, names);
		tcount++;
		break;
	case '<':
		addChar();
		nextToken = LESS_THAN;
		getName(LESS, names);
		tcount++;
		break;
	case '>':
		addChar();
		nextToken = GREATER_THAN;
		getName(GREATER, names);
		tcount++;
		break;
	case '=':
		addChar();
		nextToken = EQUAL_TO;
		getName(EQUAL, names);
		tcount++;
		break;
	case '!':
		addChar();
		nextToken = NOT_EQUAL_TO;
		getName(NOT EQUAL, names);
		tcount++;
		break;
	default:
		addChar();
		nextToken = EOF;
		getName(END, names);
		tcount++;
		break;
	}
	return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
		printf("Error - lexeme is too long \n");
}

/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	} else
		charClass = EOF;
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
		getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int x;
int lex(void) {
	lexLen = 0;
	getNonBlank();
	int x;
	switch (charClass) {
	case LETTER:
		addChar();
		getChar();

		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		if (KEYWORDS) {
			nextToken = KEYWORD;
			getName(KEY, names);
			tcount++;
		} else
			nextToken = IDENT;
		getName(IDNT, names);
		tcount++;
		break;

	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		if (nextChar != '.') {
			nextToken = INT_TYPE;
			getName(INT, names);
			tcount++;
		} else {
			addChar();
			getChar();
			while (charClass == DIGIT) {
				addChar();
				getChar();
			}
			if (nextChar == 'd' || nextChar == 'f') {
				addChar();
				getChar();
			}
			nextToken = FLOAT_TYPE;
			getName(FLOAT, names);
			tcount++;
		}
		break;

	case UNKNOWN:
		if (nextChar == '.') {
			addChar();
			getChar();
			if (charClass == DIGIT) {
				while (charClass == DIGIT) {
					addChar();
					getChar();
				}
				if (nextChar == 'd' || nextChar == 'f') {
					addChar();
					getChar();
				}
				nextToken = FLOAT_TYPE;
				getName(LEFTP, names);
			}
			nextToken = DOT;
			getName(DOT, names);
			tcount++;
		} else {
			lookup(nextChar);
			getChar();
		}
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		getName(END, names);
		tcount++;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = '\0';
		break;
	} /* End of switch */
	printf("Next Symbol is: %s, Token is: %d, Lexeme is:  %s\n", names,	nextToken, lexeme);

	if (lexeme[0] == 'E' && lexeme[1] == 'O' && lexeme[2] == 'F') {
		printf("\nTotal Token count: %d", tcount);
		printf("\nSyntax Test: PASSED \n\n");
		expr();
	} else if (nextToken == -1 && lexeme[2] != 'F') {
		printf("\nTotal Token count: %d tokens", tcount);
		printf("\nSyntax Test: FAILED\n\n");
		expr();
	}

	return nextToken;

} 
// Lexical analysis ends

//Syntax Analyzer begins
void expr(void) {
	printf("Enter <expr>\n");
	// Parse the first term
	term();
	// As long as the next token is + or -, get the next token and parse the next term
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
	printf("Exit <expr>\n");
}

// Parses strings in the language generated by the rule: <term> -> <factor> {(* | / | % ) <factor>}

void term(void) {
	printf("Enter <term>\n");
	// Parse the first factor
	factor();
	// As long as the next token is * or /, get the
	// next token and parse the next factor
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
		lex();
		factor();
	}
	printf("Exit <term>\n");
}

// factor
// Parses strings in the language generated by the rule:
//<factor> -> id | int_constant | ( <expr> )

void factor(void) {
	printf("Enter <factor>\n");
	// Determine which RHS
	if (nextToken == IDENT || nextToken == INT_TYPE)
		// Get the next token
		lex();
	// If the RHS is ( <expr> ), call lex to pass over the
	// left parenthesis, call expr, and check for the right
	// parenthesis
	else {
		if (nextToken == LEFT_PARENT) {
			lex();
			expr();
			if (nextToken == RIGHT_PARENT)
				lex();
			else {
				error2();
			}
		}
		/* It was not an id, an integer literal, or a left parenthesis */
		else
			error2();
	}
	printf("Exit <factor>\n");
}

// SEMANTIC ANALYZER

void stmtlist();
void stmt();
void program(void) {
	if (nextToken != LEFT_BRCKT)
		error();
	else {
		lex();
		stmtlist();
		if (nextToken != RIGHT_BRCKT)
			error();
	}
}

void stmtlist(void) {
	stmt();
	while (nextToken == SEMI) {
		lex();
		stmt();
	}
}

void stmt(void) {
	expr();
	while (nextToken == LESS_THAN || nextToken == GREATER_THAN || nextToken == EQUAL_TO || nextToken == ASSIGN_OP) {
		lex();
		expr();
	}
}
/*while - statements
WhileStmt = "while""(" < STMT > ")"
	< STMT_LIST >
 */
void whilestmt(void) {
	if (nextToken != WHILE_LOOP)
		error();
	else {
		lex();
		if (nextToken != LEFT_PARENT)
			error();
		else {
			lex();
			stmt();
			if (nextToken != RIGHT_PARENT)
				error();
			else {
				lex();
				stmtlist();
			}
		}
	}
}

/* if-then-else- statements
 IfStmt = "if""(" < STMT > ")"
	 <STMT_LIST>
"else" ( IfStmt | <STMT_LIST> )  .
 */
void ifstmt(void) {
	if (nextToken != IF_STMT)
		error();
	else {
		lex();
		if (nextToken != LEFT_PARENT)
			error();
		else {
			lex();
			stmt();
			if (nextToken != RIGHT_PARENT)
				error();
			else {
				lex();
				stmtlist();
				if (nextToken == ELSE_STMT) {
					lex();
					stmt();
				}
			}
		}
	}
}

/*do - while - statements
DoWhileStmt = "do" < STMT_LIST >
"while""(" < STMT > ")"
*/
void dowhilestmt(void) {
	if (nextToken != DO_LOOP)
		error();
	else {
		lex();
		stmtlist();
		if (nextToken != WHILE_LOOP)
			error();
		else {
			lex();
			if (nextToken != LEFT_PARENT)
				error();
			else {
				lex();
				stmt();
				if (nextToken != RIGHT_PARENT)
					error();
				else {
					lex();
					stmt();
				}
			}
		}
	}
}
void error2(void) {
	printf("ERROR STOP");
	exit(0);
}