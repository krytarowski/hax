/*
 * haxCmdMZ.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Hax built-in commands whose names begin with the letters
 *	M to Z.  It contains only commands in the generic core (i.e.
 *	those that don't depend much upon UNIX facilities).
 *
 * Copyright 1987-1991 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /user6/ouster/tcl/RCS/tclCmdMZ.c,v 1.18 93/01/29 09:40:49 ouster Exp $ SPRITE (Berkeley)";
#endif

#include "haxInt.h"

/*
 * Structure used to hold information about variable traces:
 */

typedef struct {
    int flags;			/* Operations for which Hax command is
				 * to be invoked. */
    int length;			/* Number of non-NULL chars. in command. */
    char command[4];		/* Space for Hax command to invoke.  Actual
				 * size will be as large as necessary to
				 * hold command.  This field must be the
				 * last in the structure, so that it can
				 * be larger than 4 bytes. */
} TraceVarInfo;

/*
 * Forward declarations for procedures defined in this file:
 */

static char *		TraceVarProc (ClientData clientData,
			    Hax_Interp *interp, char *name1, char *name2,
			    int flags);

/*
 *----------------------------------------------------------------------
 *
 * Hax_RegexpCmd --
 *
 *	This procedure is invoked to process the "regexp" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_RegexpCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int noCase = 0;
    int indices = 0;
    regexp *regexpPtr;
    char **argPtr, *string;
    int match, i;

    if (argc < 3) {
	wrongNumArgs:
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" ?-nocase? exp string ?matchVar? ?subMatchVar ",
		"subMatchVar ...?\"", (char *) NULL);
	return HAX_ERROR;
    }
    argPtr = argv+1;
    argc--;
    while ((argc > 0) && (argPtr[0][0] == '-')) {
	if (strcmp(argPtr[0], "-indices") == 0) {
	    argPtr++;
	    argc--;
	    indices = 1;
	} else if (strcmp(argPtr[0], "-nocase") == 0) {
	    argPtr++;
	    argc--;
	    noCase = 1;
	} else {
	    break;
	}
    }
    if (argc < 2) {
	goto wrongNumArgs;
    }
    regexpPtr = HaxCompileRegexp(interp, argPtr[0]);
    if (regexpPtr == NULL) {
	return HAX_ERROR;
    }

    /*
     * Convert the string to lower case, if desired, and perform
     * the match.
     */

    if (noCase) {
	char *dst, *src;

	string = (char *) ckalloc(memoryp, (unsigned) (strlen(argPtr[1]) + 1));
	for (src = argPtr[1], dst = string; *src != 0; src++, dst++) {
	    if (isupper(*src)) {
		*dst = tolower(*src);
	    } else {
		*dst = *src;
	    }
	}
	*dst = 0;
    } else {
	string = argPtr[1];
    }
    iPtr->haxRegexpError = NULL;
    match = RegExec(interp, regexpPtr, string);
    if (string != argPtr[1]) {
	ckfree(memoryp, string);
    }
    if (iPtr->haxRegexpError != NULL) {
	Hax_AppendResult(interp, "error while matching pattern: ",
		iPtr->haxRegexpError, (char *) NULL);
	return HAX_ERROR;
    }
    if (!match) {
	interp->result = (char *) "0";
	return HAX_OK;
    }

    /*
     * If additional variable names have been specified, return
     * index information in those variables.
     */

    argc -= 2;
    if (argc > NSUBEXP) {
	interp->result = (char *) "too many substring variables";
	return HAX_ERROR;
    }
    for (i = 0; i < argc; i++) {
	char *result, info[50];

	if (regexpPtr->startp[i] == NULL) {
	    if (indices) {
		result = Hax_SetVar(interp, argPtr[i+2], (char *) "-1 -1", 0);
	    } else {
		result = Hax_SetVar(interp, argPtr[i+2], (char *) "", 0);
	    }
	} else {
	    if (indices) {
		sprintf(info, "%ld %ld", regexpPtr->startp[i] - string,
			regexpPtr->endp[i] - string - 1);
		result = Hax_SetVar(interp, argPtr[i+2], info, 0);
	    } else {
		char savedChar, *first, *last;

		first = argPtr[1] + (regexpPtr->startp[i] - string);
		last = argPtr[1] + (regexpPtr->endp[i] - string);
		savedChar = *last;
		*last = 0;
		result = Hax_SetVar(interp, argPtr[i+2], first, 0);
		*last = savedChar;
	    }
	}
	if (result == NULL) {
	    Hax_AppendResult(interp, "couldn't set variable \"",
		    argPtr[i+2], "\"", (char *) NULL);
	    return HAX_ERROR;
	}
    }
    interp->result = (char *) "1";
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_RegsubCmd --
 *
 *	This procedure is invoked to process the "regsub" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_RegsubCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int noCase = 0, all = 0;
    regexp *regexpPtr;
    char *string, *p, *firstChar, *newValue, **argPtr;
    int match, result, flags;
    char *src, c;

    if (argc < 5) {
	wrongNumArgs:
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" ?-nocase? ?-all? exp string subSpec varName\"", (char *) NULL);
	return HAX_ERROR;
    }
    argPtr = argv+1;
    argc--;
    while (argPtr[0][0] == '-') {
	if (strcmp(argPtr[0], "-nocase") == 0) {
	    argPtr++;
	    argc--;
	    noCase = 1;
	} else if (strcmp(argPtr[0], "-all") == 0) {
	    argPtr++;
	    argc--;
	    all = 1;
	} else {
	    break;
	}
    }
    if (argc != 4) {
	goto wrongNumArgs;
    }
    regexpPtr = HaxCompileRegexp(interp, argPtr[0]);
    if (regexpPtr == NULL) {
	return HAX_ERROR;
    }

    /*
     * Convert the string to lower case, if desired.
     */

    if (noCase) {
	char *dst;

	string = (char *) ckalloc(memoryp, (unsigned) (strlen(argPtr[1]) + 1));
	for (src = argPtr[1], dst = string; *src != 0; src++, dst++) {
	    if (isupper(*src)) {
		*dst = tolower(*src);
	    } else {
		*dst = *src;
	    }
	}
	*dst = 0;
    } else {
	string = argPtr[1];
    }

    /*
     * The following loop is to handle multiple matches within the
     * same source string;  each iteration handles one match and its
     * corresponding substitution.  If "-all" hasn't been specified
     * then the loop body only gets executed once.
     */

    flags = 0;
    for (p = string; *p != 0; ) {
	iPtr->haxRegexpError = NULL;
	match = RegExec(interp, regexpPtr, p);
	if (iPtr->haxRegexpError != NULL) {
	    Hax_AppendResult(interp, "error while matching pattern: ",
		    iPtr->haxRegexpError, (char *) NULL);
	    result = HAX_ERROR;
	    goto done;
	}
	if (!match) {
	    break;
	}

	/*
	 * Copy the portion of the source string before the match to the
	 * result variable.
	 */

	src = argPtr[1] + (regexpPtr->startp[0] - string);
	c = *src;
	*src = 0;
	newValue = Hax_SetVar(interp, argPtr[3], argPtr[1] + (p - string),
		flags);
	*src = c;
	flags = HAX_APPEND_VALUE;
	if (newValue == NULL) {
	    cantSet:
	    Hax_AppendResult(interp, "couldn't set variable \"",
		    argPtr[3], "\"", (char *) NULL);
	    result = HAX_ERROR;
	    goto done;
	}

	/*
	 * Append the subSpec argument to the variable, making appropriate
	 * substitutions.  This code is a bit hairy because of the backslash
	 * conventions and because the code saves up ranges of characters in
	 * subSpec to reduce the number of calls to Hax_SetVar.
	 */

	for (src = firstChar = argPtr[2], c = *src; c != 0; src++, c = *src) {
	    int index;

	    if (c == '&') {
		index = 0;
	    } else if (c == '\\') {
		c = src[1];
		if ((c >= '0') && (c <= '9')) {
		    index = c - '0';
		} else if ((c == '\\') || (c == '&')) {
		    *src = c;
		    src[1] = 0;
		    newValue = Hax_SetVar(interp, argPtr[3], firstChar,
			    HAX_APPEND_VALUE);
		    *src = '\\';
		    src[1] = c;
		    if (newValue == NULL) {
			goto cantSet;
		    }
		    firstChar = src+2;
		    src++;
		    continue;
		} else {
		    continue;
		}
	    } else {
		continue;
	    }
	    if (firstChar != src) {
		c = *src;
		*src = 0;
		newValue = Hax_SetVar(interp, argPtr[3], firstChar,
			HAX_APPEND_VALUE);
		*src = c;
		if (newValue == NULL) {
		    goto cantSet;
		}
	    }
	    if ((index < NSUBEXP) && (regexpPtr->startp[index] != NULL)
		    && (regexpPtr->endp[index] != NULL)) {
		char *first, *last, saved;

		first = argPtr[1] + (regexpPtr->startp[index] - string);
		last = argPtr[1] + (regexpPtr->endp[index] - string);
		saved = *last;
		*last = 0;
		newValue = Hax_SetVar(interp, argPtr[3], first,
			HAX_APPEND_VALUE);
		*last = saved;
		if (newValue == NULL) {
		    goto cantSet;
		}
	    }
	    if (*src == '\\') {
		src++;
	    }
	    firstChar = src+1;
	}
	if (firstChar != src) {
	    if (Hax_SetVar(interp, argPtr[3], firstChar,
		    HAX_APPEND_VALUE) == NULL) {
		goto cantSet;
	    }
	}
	p = regexpPtr->endp[0];
	if (!all) {
	    break;
	}
    }

    /*
     * If there were no matches at all, then return a "0" result.
     */

    if (p == string) {
	interp->result = (char *) "0";
	result = HAX_OK;
	goto done;
    }

    /*
     * Copy the portion of the source string after the last match to the
     * result variable.
     */

    if (*p != 0) {
	if (Hax_SetVar(interp, argPtr[3], p, HAX_APPEND_VALUE) == NULL) {
	    goto cantSet;
	}
    }
    interp->result = (char *) "1";
    result = HAX_OK;

    done:
    if (string != argPtr[1]) {
	ckfree(memoryp, string);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_RenameCmd --
 *
 *	This procedure is invoked to process the "rename" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_RenameCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    Command *cmdPtr;
    Interp *iPtr = (Interp *) interp;
    Hax_HashEntry *hPtr;
    int newPtr;

    if (argc != 3) {
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" oldName newName\"", (char *) NULL);
	return HAX_ERROR;
    }
    if (argv[2][0] == '\0') {
	if (Hax_DeleteCommand(interp, argv[1]) != 0) {
	    Hax_AppendResult(interp, "can't delete \"", argv[1],
		    "\": command doesn't exist", (char *) NULL);
	    return HAX_ERROR;
	}
	return HAX_OK;
    }
    hPtr = Hax_FindHashEntry(&iPtr->commandTable, argv[2]);
    if (hPtr != NULL) {
	Hax_AppendResult(interp, "can't rename to \"", argv[2],
		"\": command already exists", (char *) NULL);
	return HAX_ERROR;
    }
    hPtr = Hax_FindHashEntry(&iPtr->commandTable, argv[1]);
    if (hPtr == NULL) {
	Hax_AppendResult(interp, "can't rename \"", argv[1],
		"\":  command doesn't exist", (char *) NULL);
	return HAX_ERROR;
    }
    cmdPtr = (Command *) Hax_GetHashValue(hPtr);
    Hax_DeleteHashEntry(interp, hPtr);
    hPtr = Hax_CreateHashEntry(interp, &iPtr->commandTable, argv[2], &newPtr);
    Hax_SetHashValue(hPtr, cmdPtr);
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_ReturnCmd --
 *
 *	This procedure is invoked to process the "return" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_ReturnCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    if (argc > 2) {
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" ?value?\"", (char *) NULL);
	return HAX_ERROR;
    }
    if (argc == 2) {
	Hax_SetResult(interp, argv[1], HAX_VOLATILE);
    }
    return HAX_RETURN;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_ScanCmd --
 *
 *	This procedure is invoked to process the "scan" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_ScanCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int arg1Length;			/* Number of bytes in argument to be
					 * scanned.  This gives an upper limit
					 * on string field sizes. */
#   define MAX_FIELDS 20
    typedef struct {
	char fmt;			/* Format for field. */
	int size;			/* How many bytes to allow for
					 * field. */
	char *location;			/* Where field will be stored. */
    } Field;
    Field fields[MAX_FIELDS];		/* Info about all the fields in the
					 * format string. */
    Field *curField;
    int numFields = 0;			/* Number of fields actually
					 * specified. */
    int suppress;			/* Current field is assignment-
					 * suppressed. */
    int totalSize = 0;			/* Number of bytes needed to store
					 * all results combined. */
    char *results;			/* Where scanned output goes.  */
    int numScanned;			/* sscanf's result. */
    char *fmt;
    int i, widthSpecified;

    if (argc < 3) {
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" string format ?varName varName ...?\"", (char *) NULL);
	return HAX_ERROR;
    }

    /*
     * This procedure operates in four stages:
     * 1. Scan the format string, collecting information about each field.
     * 2. Allocate an array to hold all of the scanned fields.
     * 3. Call sscanf to do all the dirty work, and have it store the
     *    parsed fields in the array.
     * 4. Pick off the fields from the array and assign them to variables.
     */

    arg1Length = (strlen(argv[1]) + 4) & ~03;
    for (fmt = argv[2]; *fmt != 0; fmt++) {
	if (*fmt != '%') {
	    continue;
	}
	fmt++;
	if (*fmt == '*') {
	    suppress = 1;
	    fmt++;
	} else {
	    suppress = 0;
	}
	widthSpecified = 0;
	while (isdigit(*fmt)) {
	    widthSpecified = 1;
	    fmt++;
	}
	if (suppress) {
	    continue;
	}
	if (numFields == MAX_FIELDS) {
	    interp->result = (char *) "too many fields to scan";
	    return HAX_ERROR;
	}
	curField = &fields[numFields];
	numFields++;
	switch (*fmt) {
	    case 'D':
	    case 'O':
	    case 'X':
	    case 'd':
	    case 'o':
	    case 'x':
		curField->fmt = 'd';
		curField->size = sizeof(int);
		break;

	    case 's':
		curField->fmt = 's';
		curField->size = arg1Length;
		break;

	    case 'c':
                if (widthSpecified) {
                    interp->result =
                         (char *) "field width may not be specified in %c "
			    "conversion";
                    return HAX_ERROR;
                }
		curField->fmt = 'c';
		curField->size = sizeof(int);
		break;

	    case 'E':
	    case 'F':
		curField->fmt = 'F';
		curField->size = sizeof(double);
		break;

	    case 'e':
	    case 'f':
		curField->fmt = 'f';
		curField->size = sizeof(float);
		break;

	    case '[':
		curField->fmt = 's';
		curField->size = arg1Length;
		do {
		    fmt++;
		} while (*fmt != ']');
		break;

	    default:
		sprintf(interp->result, "bad scan conversion character \"%c\"",
			*fmt);
		return HAX_ERROR;
	}
	totalSize += curField->size;
    }

    if (numFields != (argc-3)) {
	interp->result =
		(char *) "different numbers of variable names and field "
		    "specifiers";
	return HAX_ERROR;
    }

    /*
     * Step 2:
     */

    results = (char *) ckalloc(memoryp, (unsigned) totalSize);
    for (i = 0, totalSize = 0, curField = fields;
	    i < numFields; i++, curField++) {
	curField->location = results + totalSize;
	totalSize += curField->size;
    }

    /*
     * Fill in the remaining fields with NULL;  the only purpose of
     * this is to keep some memory analyzers, like Purify, from
     * complaining.
     */

    for ( ; i < MAX_FIELDS; i++, curField++) {
	curField->location = NULL;
    }

    /*
     * Step 3:
     */

    numScanned = sscanf(argv[1], argv[2],
	    fields[0].location, fields[1].location, fields[2].location,
	    fields[3].location, fields[4].location, fields[5].location,
	    fields[6].location, fields[7].location, fields[8].location,
	    fields[9].location, fields[10].location, fields[11].location,
	    fields[12].location, fields[13].location, fields[14].location,
	    fields[15].location, fields[16].location, fields[17].location,
	    fields[18].location, fields[19].location);

    /*
     * Step 4:
     */

    if (numScanned < numFields) {
	numFields = numScanned;
    }
    for (i = 0, curField = fields; i < numFields; i++, curField++) {
	switch (curField->fmt) {
	    char string[120];

	    case 'd':
		sprintf(string, "%d", *((int *) curField->location));
		if (Hax_SetVar(interp, argv[i+3], string, 0) == NULL) {
		    storeError:
		    Hax_AppendResult(interp,
			    "couldn't set variable \"", argv[i+3], "\"",
			    (char *) NULL);
		    ckfree(memoryp, (char *) results);
		    return HAX_ERROR;
		}
		break;

	    case 'c':
		sprintf(string, "%d", *((char *) curField->location) & 0xff);
		if (Hax_SetVar(interp, argv[i+3], string, 0) == NULL) {
		    goto storeError;
		}
		break;

	    case 's':
		if (Hax_SetVar(interp, argv[i+3], curField->location, 0)
			== NULL) {
		    goto storeError;
		}
		break;

	    case 'F':
		sprintf(string, "%g", *((double *) curField->location));
		if (Hax_SetVar(interp, argv[i+3], string, 0) == NULL) {
		    goto storeError;
		}
		break;

	    case 'f':
		sprintf(string, "%g", *((float *) curField->location));
		if (Hax_SetVar(interp, argv[i+3], string, 0) == NULL) {
		    goto storeError;
		}
		break;
	}
    }
    ckfree(memoryp, results);
    sprintf(interp->result, "%d", numScanned);
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_SplitCmd --
 *
 *	This procedure is invoked to process the "split" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_SplitCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    char *splitChars;
    char *p, *p2;
    char *elementStart;

    if (argc == 2) {
	splitChars = (char *) " \n\t\r";
    } else if (argc == 3) {
	splitChars = argv[2];
    } else {
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" string ?splitChars?\"", (char *) NULL);
	return HAX_ERROR;
    }

    /*
     * Handle the special case of splitting on every character.
     */

    if (*splitChars == 0) {
	char string[2];
	string[1] = 0;
	for (p = argv[1]; *p != 0; p++) {
	    string[0] = *p;
	    Hax_AppendElement(interp, string, 0);
	}
	return HAX_OK;
    }

    /*
     * Normal case: split on any of a given set of characters.
     * Discard instances of the split characters.
     */

    for (p = elementStart = argv[1]; *p != 0; p++) {
	char c = *p;
	for (p2 = splitChars; *p2 != 0; p2++) {
	    if (*p2 == c) {
		*p = 0;
		Hax_AppendElement(interp, elementStart, 0);
		*p = c;
		elementStart = p+1;
		break;
	    }
	}
    }
    if (p != argv[1]) {
	Hax_AppendElement(interp, elementStart, 0);
    }
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_StringCmd --
 *
 *	This procedure is invoked to process the "string" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_StringCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    int length;
    char *p, c;
    int match;
    int first;
    int left = 0, right = 0;

    if (argc < 2) {
	Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" option arg ?arg ...?\"", (char *) NULL);
	return HAX_ERROR;
    }
    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'c') && (strncmp(argv[1], "compare", length) == 0)) {
	if (argc != 4) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " compare string1 string2\"", (char *) NULL);
	    return HAX_ERROR;
	}
	match = strcmp(argv[2], argv[3]);
	if (match > 0) {
	    interp->result = (char *) "1";
	} else if (match < 0) {
	    interp->result = (char *) "-1";
	} else {
	    interp->result = (char *) "0";
	}
	return HAX_OK;
    } else if ((c == 'f') && (strncmp(argv[1], "first", length) == 0)) {
	if (argc != 4) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " first string1 string2\"", (char *) NULL);
	    return HAX_ERROR;
	}
	first = 1;

	firstLast:
	match = -1;
	c = *argv[2];
	length = strlen(argv[2]);
	for (p = argv[3]; *p != 0; p++) {
	    if (*p != c) {
		continue;
	    }
	    if (strncmp(argv[2], p, length) == 0) {
		match = p-argv[3];
		if (first) {
		    break;
		}
	    }
	}
	sprintf(interp->result, "%d", match);
	return HAX_OK;
    } else if ((c == 'i') && (strncmp(argv[1], "index", length) == 0)) {
	long int index;

	if (argc != 4) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " index string charIndex\"", (char *) NULL);
	    return HAX_ERROR;
	}
	if (Hax_GetLong(interp, argv[3], &index) != HAX_OK) {
	    return HAX_ERROR;
	}
	if ((index >= 0) && (index < strlen(argv[2]))) {
	    interp->result[0] = argv[2][index];
	    interp->result[1] = 0;
	}
	return HAX_OK;
    } else if ((c == 'l') && (strncmp(argv[1], "last", length) == 0)
	    && (length >= 2)) {
	if (argc != 4) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " last string1 string2\"", (char *) NULL);
	    return HAX_ERROR;
	}
	first = 0;
	goto firstLast;
    } else if ((c == 'l') && (strncmp(argv[1], "length", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " length string\"", (char *) NULL);
	    return HAX_ERROR;
	}
	sprintf(interp->result, "%lu", (unsigned long)strlen(argv[2]));
	return HAX_OK;
    } else if ((c == 'm') && (strncmp(argv[1], "match", length) == 0)) {
	if (argc != 4) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " match pattern string\"", (char *) NULL);
	    return HAX_ERROR;
	}
	if (Hax_StringMatch(argv[3], argv[2]) != 0) {
	    interp->result = (char *) "1";
	} else {
	    interp->result = (char *) "0";
	}
	return HAX_OK;
    } else if ((c == 'r') && (strncmp(argv[1], "range", length) == 0)) {
	long int first, last, stringLength;

	if (argc != 5) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " range string first last\"", (char *) NULL);
	    return HAX_ERROR;
	}
	stringLength = strlen(argv[2]);
	if (Hax_GetLong(interp, argv[3], &first) != HAX_OK) {
	    return HAX_ERROR;
	}
	if ((*argv[4] == 'e')
		&& (strncmp(argv[4], "end", strlen(argv[4])) == 0)) {
	    last = stringLength-1;
	} else {
	    if (Hax_GetLong(interp, argv[4], &last) != HAX_OK) {
		Hax_ResetResult(interp);
		Hax_AppendResult(interp,
			"expected integer or \"end\" but got \"",
			argv[4], "\"", (char *) NULL);
		return HAX_ERROR;
	    }
	}
	if (first < 0) {
	    first = 0;
	}
	if (last >= stringLength) {
	    last = stringLength-1;
	}
	if (last >= first) {
	    char saved, *p;

	    p = argv[2] + last + 1;
	    saved = *p;
	    *p = 0;
	    Hax_SetResult(interp, argv[2] + first, HAX_VOLATILE);
	    *p = saved;
	}
	return HAX_OK;
    } else if ((c == 't') && (strncmp(argv[1], "tolower", length) == 0)
	    && (length >= 3)) {
	char *p;

	if (argc != 3) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " tolower string\"", (char *) NULL);
	    return HAX_ERROR;
	}
	Hax_SetResult(interp, argv[2], HAX_VOLATILE);
	for (p = interp->result; *p != 0; p++) {
	    if (isupper(*p)) {
		*p = tolower(*p);
	    }
	}
	return HAX_OK;
    } else if ((c == 't') && (strncmp(argv[1], "toupper", length) == 0)
	    && (length >= 3)) {
	char *p;

	if (argc != 3) {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " toupper string\"", (char *) NULL);
	    return HAX_ERROR;
	}
	Hax_SetResult(interp, argv[2], HAX_VOLATILE);
	for (p = interp->result; *p != 0; p++) {
	    if (islower(*p)) {
		*p = toupper(*p);
	    }
	}
	return HAX_OK;
    } else if ((c == 't') && (strncmp(argv[1], "trim", length) == 0)
	    && (length == 4)) {
	char *trimChars;
	char *p, *checkPtr;

	left = right = 1;

	trim:
	if (argc == 4) {
	    trimChars = argv[3];
	} else if (argc == 3) {
	    trimChars = (char *) " \t\n\r";
	} else {
	    Hax_AppendResult(interp, "wrong # args: should be \"", argv[0],
		    " ", argv[1], " string ?chars?\"", (char *) NULL);
	    return HAX_ERROR;
	}
	p = argv[2];
	if (left) {
	    for (c = *p; c != 0; p++, c = *p) {
		for (checkPtr = trimChars; *checkPtr != c; checkPtr++) {
		    if (*checkPtr == 0) {
			goto doneLeft;
		    }
		}
	    }
	}
	doneLeft:
	Hax_SetResult(interp, p, HAX_VOLATILE);
	if (right) {
	    char *donePtr;

	    p = interp->result + strlen(interp->result) - 1;
	    donePtr = &interp->result[-1];
	    for (c = *p; p != donePtr; p--, c = *p) {
		for (checkPtr = trimChars; *checkPtr != c; checkPtr++) {
		    if (*checkPtr == 0) {
			goto doneRight;
		    }
		}
	    }
	    doneRight:
	    p[1] = 0;
	}
	return HAX_OK;
    } else if ((c == 't') && (strncmp(argv[1], "trimleft", length) == 0)
	    && (length > 4)) {
	left = 1;
	argv[1] = (char *) "trimleft";
	goto trim;
    } else if ((c == 't') && (strncmp(argv[1], "trimright", length) == 0)
	    && (length > 4)) {
	right = 1;
	argv[1] = (char *) "trimright";
	goto trim;
    } else {
	Hax_AppendResult(interp, "bad option \"", argv[1],
		"\": should be compare, first, index, last, length, match, ",
		"range, tolower, toupper, trim, trimleft, or trimright",
		(char *) NULL);
	return HAX_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_TraceCmd --
 *
 *	This procedure is invoked to process the "trace" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_TraceCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    char c;
    int length;

    if (argc < 2) {
	Hax_AppendResult(interp, "too few args: should be \"",
		argv[0], " option [arg arg ...]\"", (char *) NULL);
	return HAX_ERROR;
    }
    c = argv[1][1];
    length = strlen(argv[1]);
    if ((c == 'a') && (strncmp(argv[1], "variable", length) == 0)
	    && (length >= 2)) {
	char *p;
	int flags, length;
	TraceVarInfo *tvarPtr;

	if (argc != 5) {
	    Hax_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " variable name ops command\"", (char *) NULL);
	    return HAX_ERROR;
	}

	flags = 0;
	for (p = argv[3] ; *p != 0; p++) {
	    if (*p == 'r') {
		flags |= HAX_TRACE_READS;
	    } else if (*p == 'w') {
		flags |= HAX_TRACE_WRITES;
	    } else if (*p == 'u') {
		flags |= HAX_TRACE_UNSETS;
	    } else {
		goto badOps;
	    }
	}
	if (flags == 0) {
	    goto badOps;
	}

	length = strlen(argv[4]);
	tvarPtr = (TraceVarInfo *) ckalloc(memoryp, (unsigned)
		(sizeof(TraceVarInfo) - sizeof(tvarPtr->command) + length + 1));
	tvarPtr->flags = flags;
	tvarPtr->length = length;
	flags |= HAX_TRACE_UNSETS;
	strcpy(tvarPtr->command, argv[4]);
	if (Hax_TraceVar(interp, argv[2], flags, TraceVarProc,
		(ClientData) tvarPtr) != HAX_OK) {
	    ckfree(memoryp, (char *) tvarPtr);
	    return HAX_ERROR;
	}
    } else if ((c == 'd') && (strncmp(argv[1], "vdelete", length)
	    && (length >= 2)) == 0) {
	char *p;
	int flags, length;
	TraceVarInfo *tvarPtr;
	ClientData clientData;

	if (argc != 5) {
	    Hax_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " vdelete name ops command\"", (char *) NULL);
	    return HAX_ERROR;
	}

	flags = 0;
	for (p = argv[3] ; *p != 0; p++) {
	    if (*p == 'r') {
		flags |= HAX_TRACE_READS;
	    } else if (*p == 'w') {
		flags |= HAX_TRACE_WRITES;
	    } else if (*p == 'u') {
		flags |= HAX_TRACE_UNSETS;
	    } else {
		goto badOps;
	    }
	}
	if (flags == 0) {
	    goto badOps;
	}

	/*
	 * Search through all of our traces on this variable to
	 * see if there's one with the given command.  If so, then
	 * delete the first one that matches.
	 */

	length = strlen(argv[4]);
	clientData = 0;
	while ((clientData = Hax_VarTraceInfo(interp, argv[2], 0,
		TraceVarProc, clientData)) != 0) {
	    tvarPtr = (TraceVarInfo *) clientData;
	    if ((tvarPtr->length == length) && (tvarPtr->flags == flags)
		    && (strncmp(argv[4], tvarPtr->command, length) == 0)) {
		Hax_UntraceVar(interp, argv[2], flags | HAX_TRACE_UNSETS,
			TraceVarProc, clientData);
		ckfree(memoryp, (char *) tvarPtr);
		break;
	    }
	}
    } else if ((c == 'i') && (strncmp(argv[1], "vinfo", length) == 0)
	    && (length >= 2)) {
	ClientData clientData;
	char ops[4], *p;
	char *prefix = (char *) "{";

	if (argc != 3) {
	    Hax_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " vinfo name\"", (char *) NULL);
	    return HAX_ERROR;
	}
	clientData = 0;
	while ((clientData = Hax_VarTraceInfo(interp, argv[2], 0,
		TraceVarProc, clientData)) != 0) {
	    TraceVarInfo *tvarPtr = (TraceVarInfo *) clientData;
	    p = ops;
	    if (tvarPtr->flags & HAX_TRACE_READS) {
		*p = 'r';
		p++;
	    }
	    if (tvarPtr->flags & HAX_TRACE_WRITES) {
		*p = 'w';
		p++;
	    }
	    if (tvarPtr->flags & HAX_TRACE_UNSETS) {
		*p = 'u';
		p++;
	    }
	    *p = '\0';
	    Hax_AppendResult(interp, prefix, (char *) NULL);
	    Hax_AppendElement(interp, ops, 1);
	    Hax_AppendElement(interp, tvarPtr->command, 0);
	    Hax_AppendResult(interp, "}", (char *) NULL);
	    prefix = (char *) " {";
	}
    } else {
	Hax_AppendResult(interp, "bad option \"", argv[1],
		"\": should be variable, vdelete, or vinfo",
		(char *) NULL);
	return HAX_ERROR;
    }
    return HAX_OK;

    badOps:
    Hax_AppendResult(interp, "bad operations \"", argv[3],
	    "\": should be one or more of rwu", (char *) NULL);
    return HAX_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * TraceVarProc --
 *
 *	This procedure is called to handle variable accesses that have
 *	been traced using the "trace" command.
 *
 * Results:
 *	Normally returns NULL.  If the trace command returns an error,
 *	then this procedure returns an error string.
 *
 * Side effects:
 *	Depends on the command associated with the trace.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
static char *
TraceVarProc(
    ClientData clientData,	/* Information about the variable trace. */
    Hax_Interp *interp,		/* Interpreter containing variable. */
    char *name1,		/* Name of variable or array. */
    char *name2,		/* Name of element within array;  NULL means
				 * scalar variable is being referenced. */
    int flags			/* OR-ed bits giving operation and other
				 * information. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    TraceVarInfo *tvarPtr = (TraceVarInfo *) clientData;
    char *result;
    int code, cmdLength, flags1, flags2;
    Interp dummy;
#define STATIC_SIZE 199
    char staticSpace[STATIC_SIZE+1];
    char *cmdPtr, *p;

    result = NULL;
    if ((tvarPtr->flags & flags) && !(flags & HAX_INTERP_DESTROYED)) {

	/*
	 * Generate a command to execute by appending list elements
	 * for the two variable names and the operation.  The five
	 * extra characters are for three space, the opcode character,
	 * and the terminating null.
	 */

	if (name2 == NULL) {
	    name2 = (char *) "";
	}
	cmdLength = tvarPtr->length + Hax_ScanElement(name1, &flags1) +
		Hax_ScanElement(name2, &flags2) + 5;
	if (cmdLength < STATIC_SIZE) {
	    cmdPtr = staticSpace;
	} else {
	    cmdPtr = (char *) ckalloc(memoryp, (unsigned) cmdLength);
	}
	p = cmdPtr;
	strcpy(p, tvarPtr->command);
	p += tvarPtr->length;
	*p = ' ';
	p++;
	p += Hax_ConvertElement(name1, p, flags1);
	*p = ' ';
	p++;
	p += Hax_ConvertElement(name2, p, flags2);
	*p = ' ';
	if (flags & HAX_TRACE_READS) {
	    p[1] = 'r';
	} else if (flags & HAX_TRACE_WRITES) {
	    p[1] = 'w';
	} else if (flags & HAX_TRACE_UNSETS) {
	    p[1] = 'u';
	}
	p[2] = '\0';

	/*
	 * Execute the command.  Be careful to save and restore the
	 * result from the interpreter used for the command.
	 */

	dummy.memoryp = iPtr->memoryp;
	if (interp->freeProc == 0) {
	    dummy.freeProc = (Hax_FreeProc *) 0;
	    dummy.result = dummy.resultSpace;
	    dummy.resultSpace[0] = 0;
	    Hax_SetResult((Hax_Interp *) &dummy, interp->result, HAX_VOLATILE);
	} else {
	    dummy.freeProc = interp->freeProc;
	    dummy.result = interp->result;
	}
	code = Hax_Eval(interp, NULL, cmdPtr, 0, (char **) NULL);
	if (cmdPtr != staticSpace) {
	    ckfree(memoryp, cmdPtr);
	}
	if (code != HAX_OK) {
	    result = (char *) "access disallowed by trace command";
	    Hax_ResetResult(interp);		/* Must clear error state. */
	}
	Hax_FreeResult(interp);
	if (dummy.result == dummy.resultSpace) {
	    if (dummy.freeProc != HAX_STATIC) {
		Hax_Panic((char *) "Corrupted state of dummy Interpreter");
	    }
	    iPtr->result = iPtr->resultSpace;
	    strcpy(iPtr->resultSpace, dummy.resultSpace);
	    iPtr->freeProc = HAX_STATIC;
	} else {
	    interp->result = dummy.result;
	    interp->freeProc = dummy.freeProc;
	}
    }
    if (flags & HAX_TRACE_DESTROYED) {
	ckfree(memoryp, (char *) tvarPtr);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_WhileCmd --
 *
 *	This procedure is invoked to process the "while" Hax command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Hax result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Hax_WhileCmd(
    ClientData dummy,			/* Not used. */
    Hax_Interp *interp,			/* Current interpreter. */
    int argc,				/* Number of arguments. */
    char **argv				/* Argument strings. */)
{
    int result, value;

    if (argc != 3) {
	Hax_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " test command\"", (char *) NULL);
	return HAX_ERROR;
    }

    while (1) {
	result = Hax_ExprBoolean(interp, argv[1], &value);
	if (result != HAX_OK) {
	    return result;
	}
	if (!value) {
	    break;
	}
	result = Hax_Eval(interp, NULL, argv[2], 0, (char **) NULL);
	if (result == HAX_CONTINUE) {
	    result = HAX_OK;
	} else if (result != HAX_OK) {
	    if (result == HAX_ERROR) {
		char msg[60];
		sprintf(msg, "\n    (\"while\" body line %d)",
			interp->errorLine);
		Hax_AddErrorInfo(interp, msg);
	    }
	    break;
	}
    }
    if (result == HAX_BREAK) {
	result = HAX_OK;
    }
    if (result == HAX_OK) {
	Hax_ResetResult(interp);
    }
    return result;
}
