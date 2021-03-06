/*
 * haxUtil.c --
 *
 *	This file contains utility procedures that are used by many Hax
 *	commands.
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
static char rcsid[] = "$Header: /user6/ouster/tcl/RCS/tclUtil.c,v 1.66 92/10/21 16:12:01 ouster Exp $ SPRITE (Berkeley)";
#endif

#include "haxInt.h"

/*
 * The following values are used in the flags returned by Hax_ScanElement
 * and used by Hax_ConvertElement.  The value HAX_DONT_USE_BRACES is also
 * defined in hax.h;  make sure its value doesn't overlap with any of the
 * values below.
 *
 * HAX_DONT_USE_BRACES -	1 means the string mustn't be enclosed in
 *				braces (e.g. it contains unmatched braces,
 *				or ends in a backslash character, or user
 *				just doesn't want braces);  handle all
 *				special characters by adding backslashes.
 * USE_BRACES -			1 means the string contains a special
 *				character that can be handled simply by
 *				enclosing the entire argument in braces.
 * BRACES_UNMATCHED -		1 means that braces aren't properly matched
 *				in the argument.
 */

#define USE_BRACES		2
#define BRACES_UNMATCHED	4

/*
 * Function prototypes for local procedures in this file:
 */

static void		SetupAppendBuffer (Interp *iPtr,
			    int newSpace);

/*
 *----------------------------------------------------------------------
 *
 * HaxFindElement --
 *
 *	Given a pointer into a Hax list, locate the first (or next)
 *	element in the list.
 *
 * Results:
 *	The return value is normally HAX_OK, which means that the
 *	element was successfully located.  If HAX_ERROR is returned
 *	it means that list didn't have proper list structure;
 *	interp->result contains a more detailed error message.
 *
 *	If HAX_OK is returned, then *elementPtr will be set to point
 *	to the first element of list, and *nextPtr will be set to point
 *	to the character just after any white space following the last
 *	character that's part of the element.  If this is the last argument
 *	in the list, then *nextPtr will point to the NULL character at the
 *	end of list.  If sizePtr is non-NULL, *sizePtr is filled in with
 *	the number of characters in the element.  If the element is in
 *	braces, then *elementPtr will point to the character after the
 *	opening brace and *sizePtr will not include either of the braces.
 *	If there isn't an element in the list, *sizePtr will be zero, and
 *	both *elementPtr and *termPtr will refer to the null character at
 *	the end of list.  Note:  this procedure does NOT collapse backslash
 *	sequences.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
HaxFindElement(
    Hax_Interp *interp,		/* Interpreter to use for error reporting. */
    char *list,			/* String containing Hax list with zero
				 * or more elements (possibly in braces). */
    char **elementPtr,		/* Fill in with location of first significant
				 * character in first element of list. */
    char **nextPtr,		/* Fill in with location of character just
				 * after all white space following end of
				 * argument (i.e. next argument or end of
				 * list). */
    long int *sizePtr,		/* If non-zero, fill in with size of
				 * element. */
    int *bracePtr		/* If non-zero fill in with non-zero/zero
				 * to indicate that arg was/wasn't
				 * in braces. */)
{
    char *p;
    int openBraces = 0;
    int inQuotes = 0;
    long int size;

    /*
     * Skim off leading white space and check for an opening brace or
     * quote.   Note:  use of "isascii" below and elsewhere in this
     * procedure is a temporary hack (7/27/90) because Mx uses characters
     * with the high-order bit set for some things.  This should probably
     * be changed back eventually, or all of Hax should call isascii.
     */

    while (isascii(*list) && isspace(*list)) {
	list++;
    }
    if (*list == '{') {
	openBraces = 1;
	list++;
    } else if (*list == '"') {
	inQuotes = 1;
	list++;
    }
    if (bracePtr != 0) {
	*bracePtr = openBraces;
    }
    p = list;

    /*
     * Find the end of the element (either a space or a close brace or
     * the end of the string).
     */

    while (1) {
	switch (*p) {

	    /*
	     * Open brace: don't treat specially unless the element is
	     * in braces.  In this case, keep a nesting count.
	     */

	    case '{':
		if (openBraces != 0) {
		    openBraces++;
		}
		break;

	    /*
	     * Close brace: if element is in braces, keep nesting
	     * count and quit when the last close brace is seen.
	     */

	    case '}':
		if (openBraces == 1) {
		    char *p2;

		    size = p - list;
		    p++;
		    if ((isascii(*p) && isspace(*p)) || (*p == 0)) {
			goto done;
		    }
		    for (p2 = p; (*p2 != 0) && (!isspace(*p2)) && (p2 < p+20);
			    p2++) {
			/* null body */
		    }
		    Hax_ResetResult(interp);
		    sprintf(interp->result,
			    "list element in braces followed by \"%.*s\" instead of space",
			    (int)(p2-p), p);
		    return HAX_ERROR;
		} else if (openBraces != 0) {
		    openBraces--;
		}
		break;

	    /*
	     * Backslash:  skip over everything up to the end of the
	     * backslash sequence.
	     */

	    case '\\': {
		int sizeb;

		(void) Hax_Backslash(p, &sizeb);
		p += sizeb - 1;
		break;
	    }

	    /*
	     * Space: ignore if element is in braces or quotes;  otherwise
	     * terminate element.
	     */

	    case ' ':
	    case '\f':
	    case '\n':
	    case '\r':
	    case '\t':
	    case '\v':
		if ((openBraces == 0) && !inQuotes) {
		    size = p - list;
		    goto done;
		}
		break;

	    /*
	     * Double-quote:  if element is in quotes then terminate it.
	     */

	    case '"':
		if (inQuotes) {
		    char *p2;

		    size = p-list;
		    p++;
		    if ((isascii(*p) && isspace(*p)) || (*p == 0)) {
			goto done;
		    }
		    for (p2 = p; (*p2 != 0) && (!isspace(*p2)) && (p2 < p+20);
			    p2++) {
			/* null body */
		    }
		    Hax_ResetResult(interp);
		    sprintf(interp->result,
			    "list element in quotes followed by \"%.*s\" %s",
			    (int)(p2-p), p, "instead of space");
		    return HAX_ERROR;
		}
		break;

	    /*
	     * End of list:  terminate element.
	     */

	    case 0:
		if (openBraces != 0) {
		    Hax_SetResult(interp,
			    (char *) "unmatched open brace in list",
			    HAX_STATIC);
		    return HAX_ERROR;
		} else if (inQuotes) {
		    Hax_SetResult(interp,
			    (char *) "unmatched open quote in list",
			    HAX_STATIC);
		    return HAX_ERROR;
		}
		size = p - list;
		goto done;

	}
	p++;
    }

    done:
    while (isascii(*p) && isspace(*p)) {
	p++;
    }
    *elementPtr = list;
    *nextPtr = p;
    if (sizePtr != 0) {
	*sizePtr = size;
    }
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * HaxCopyAndCollapse --
 *
 *	Copy a string and eliminate any backslashes that aren't in braces.
 *
 * Results:
 *	There is no return value.  Count chars. get copied from src
 *	to dst.  Along the way, if backslash sequences are found outside
 *	braces, the backslashes are eliminated in the copy.
 *	After scanning count chars. from source, a null character is
 *	placed at the end of dst.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
HaxCopyAndCollapse(
    int count,			/* Total number of characters to copy
				 * from src. */
    char *src,		/* Copy from here... */
    char *dst		/* ... to here. */)
{
    char c;
    int numRead;

    for (c = *src; count > 0; src++, c = *src, count--) {
	if (c == '\\') {
	    *dst = Hax_Backslash(src, &numRead);
	    if (*dst != 0) {
		dst++;
	    }
	    src += numRead-1;
	    count -= numRead-1;
	} else {
	    *dst = c;
	    dst++;
	}
    }
    *dst = 0;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_SplitList --
 *
 *	Splits a list up into its constituent fields.
 *
 * Results
 *	The return value is normally HAX_OK, which means that
 *	the list was successfully split up.  If HAX_ERROR is
 *	returned, it means that "list" didn't have proper list
 *	structure;  interp->result will contain a more detailed
 *	error message.
 *
 *	*argvPtr will be filled in with the address of an array
 *	whose elements point to the elements of list, in order.
 *	*argcPtr will get filled in with the number of valid elements
 *	in the array.  A single block of memory is dynamically allocated
 *	to hold both the argv array and a copy of the list (with
 *	backslashes and braces removed in the standard way).
 *	The caller must eventually free this memory by calling free()
 *	on *argvPtr.  Note:  *argvPtr and *argcPtr are only modified
 *	if the procedure returns normally.
 *
 * Side effects:
 *	Memory is allocated.
 *
 *----------------------------------------------------------------------
 */

int
Hax_SplitList(
    Hax_Interp *interp,		/* Interpreter to use for error reporting. */
    char *list,			/* Pointer to string with list structure. */
    int *argcPtr,		/* Pointer to location to fill in with
				 * the number of elements in the list. */
    char ***argvPtr		/* Pointer to place to store pointer to array
				 * of pointers to list elements. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    char **argv;
    char *p;
    int result, brace;
    long int size, i, elSize;
    char *element;

    /*
     * Figure out how much space to allocate.  There must be enough
     * space for both the array of pointers and also for a copy of
     * the list.  To estimate the number of pointers needed, count
     * the number of space characters in the list.
     */

    for (size = 1, p = list; *p != 0; p++) {
	if (isspace(*p)) {
	    size++;
	}
    }
    size++;			/* Leave space for final NULL pointer. */
    argv = (char **) ckalloc(memoryp, (unsigned)
	    ((size * sizeof(char *)) + (p - list) + 1));
    for (i = 0, p = ((char *) argv) + size*sizeof(char *);
	    *list != 0; i++) {
	result = HaxFindElement(interp, list, &element, &list, &elSize, &brace);
	if (result != HAX_OK) {
	    ckfree(memoryp, (char *) argv);
	    return result;
	}
	if (*element == 0) {
	    break;
	}
	if (i >= size) {
	    ckfree(memoryp, (char *) argv);
	    Hax_SetResult(interp, (char *) "internal error in Hax_SplitList",
		    HAX_STATIC);
	    return HAX_ERROR;
	}
	argv[i] = p;
	if (brace) {
	    strncpy(p, element, elSize);
	    p += elSize;
	    *p = 0;
	    p++;
	} else {
	    HaxCopyAndCollapse(elSize, element, p);
	    p += elSize+1;
	}
    }

    argv[i] = NULL;
    *argvPtr = argv;
    *argcPtr = i;
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_ScanElement --
 *
 *	This procedure is a companion procedure to Hax_ConvertElement.
 *	It scans a string to see what needs to be done to it (e.g.
 *	add backslashes or enclosing braces) to make the string into
 *	a valid Hax list element.
 *
 * Results:
 *	The return value is an overestimate of the number of characters
 *	that will be needed by Hax_ConvertElement to produce a valid
 *	list element from string.  The word at *flagPtr is filled in
 *	with a value needed by Hax_ConvertElement when doing the actual
 *	conversion.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Hax_ScanElement(
    char *string,		/* String to convert to Hax list element. */
    int *flagPtr		/* Where to store information to guide
				 * Hax_ConvertElement. */)
{
    int flags, nestingLevel;
    char *p;

    /*
     * This procedure and Hax_ConvertElement together do two things:
     *
     * 1. They produce a proper list, one that will yield back the
     * argument strings when evaluated or when disassembled with
     * Hax_SplitList.  This is the most important thing.
     *
     * 2. They try to produce legible output, which means minimizing the
     * use of backslashes (using braces instead).  However, there are
     * some situations where backslashes must be used (e.g. an element
     * like "{abc": the leading brace will have to be backslashed.  For
     * each element, one of three things must be done:
     *
     * (a) Use the element as-is (it doesn't contain anything special
     * characters).  This is the most desirable option.
     *
     * (b) Enclose the element in braces, but leave the contents alone.
     * This happens if the element contains embedded space, or if it
     * contains characters with special interpretation ($, [, ;, or \),
     * or if it starts with a brace or double-quote, or if there are
     * no characters in the element.
     *
     * (c) Don't enclose the element in braces, but add backslashes to
     * prevent special interpretation of special characters.  This is a
     * last resort used when the argument would normally fall under case
     * (b) but contains unmatched braces.  It also occurs if the last
     * character of the argument is a backslash or if the element contains
     * a backslash followed by newline.
     *
     * The procedure figures out how many bytes will be needed to store
     * the result (actually, it overestimates).  It also collects information
     * about the element in the form of a flags word.
     */

    nestingLevel = 0;
    flags = 0;
    if (string == NULL) {
	string = (char *) "";
    }
    p = string;
    if ((*p == '{') || (*p == '"') || (*p == 0)) {
	flags |= USE_BRACES;
    }
    for ( ; *p != 0; p++) {
	switch (*p) {
	    case '{':
		nestingLevel++;
		break;
	    case '}':
		nestingLevel--;
		if (nestingLevel < 0) {
		    flags |= HAX_DONT_USE_BRACES|BRACES_UNMATCHED;
		}
		break;
	    case '[':
	    case '$':
	    case ';':
	    case ' ':
	    case '\f':
	    case '\n':
	    case '\r':
	    case '\t':
	    case '\v':
		flags |= USE_BRACES;
		break;
	    case '\\':
		if ((p[1] == 0) || (p[1] == '\n')) {
		    flags = HAX_DONT_USE_BRACES;
		} else {
		    int size;

		    (void) Hax_Backslash(p, &size);
		    p += size-1;
		    flags |= USE_BRACES;
		}
		break;
	}
    }
    if (nestingLevel != 0) {
	flags = HAX_DONT_USE_BRACES | BRACES_UNMATCHED;
    }
    *flagPtr = flags;

    /*
     * Allow enough space to backslash every character plus leave
     * two spaces for braces.
     */

    return 2*(p-string) + 2;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_ConvertElement --
 *
 *	This is a companion procedure to Hax_ScanElement.  Given the
 *	information produced by Hax_ScanElement, this procedure converts
 *	a string to a list element equal to that string.
 *
 * Results:
 *	Information is copied to *dst in the form of a list element
 *	identical to src (i.e. if Hax_SplitList is applied to dst it
 *	will produce a string identical to src).  The return value is
 *	a count of the number of characters copied (not including the
 *	terminating NULL character).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Hax_ConvertElement(
    char *src,		/* Source information for list element. */
    char *dst,			/* Place to put list-ified element. */
    int flags			/* Flags produced by Hax_ScanElement. */)
{
    char *p = dst;

    /*
     * See the comment block at the beginning of the Hax_ScanElement
     * code for details of how this works.
     */

    if (src == NULL) {
	src = (char *) "";
    }
    if ((flags & USE_BRACES) && !(flags & HAX_DONT_USE_BRACES)) {
	*p = '{';
	p++;
	for ( ; *src != 0; src++, p++) {
	    *p = *src;
	}
	*p = '}';
	p++;
    } else if (*src == 0) {
	/*
	 * If string is empty but can't use braces, then use special
	 * backslash sequence that maps to empty string.
	 */

	p[0] = '\\';
	p[1] = '0';
	p += 2;
    } else {
	for (; *src != 0 ; src++) {
	    switch (*src) {
		case ']':
		case '[':
		case '$':
		case ';':
		case ' ':
		case '\\':
		case '"':
		    *p = '\\';
		    p++;
		    break;
		case '{':
		case '}':
		    if (flags & BRACES_UNMATCHED) {
			*p = '\\';
			p++;
		    }
		    break;
		case '\f':
		    *p = '\\';
		    p++;
		    *p = 'f';
		    p++;
		    continue;
		case '\n':
		    *p = '\\';
		    p++;
		    *p = 'n';
		    p++;
		    continue;
		case '\r':
		    *p = '\\';
		    p++;
		    *p = 'r';
		    p++;
		    continue;
		case '\t':
		    *p = '\\';
		    p++;
		    *p = 't';
		    p++;
		    continue;
		case '\v':
		    *p = '\\';
		    p++;
		    *p = 'v';
		    p++;
		    continue;
	    }
	    *p = *src;
	    p++;
	}
    }
    *p = '\0';
    return p-dst;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_Merge --
 *
 *	Given a collection of strings, merge them together into a
 *	single string that has proper Hax list structured (i.e.
 *	Hax_SplitList may be used to retrieve strings equal to the
 *	original elements, and Hax_Eval will parse the string back
 *	into its original elements).
 *
 * Results:
 *	The return value is the address of a dynamically-allocated
 *	string containing the merged list.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Hax_Merge(
    Hax_Interp *interp,
    int argc,			/* How many strings to merge. */
    char **argv			/* Array of string values. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
#   define LOCAL_SIZE 20
    int localFlags[LOCAL_SIZE], *flagPtr;
    int numChars;
    char *result;
    char *dst;
    int i;

    /*
     * Pass 1: estimate space, gather flags.
     */

    if (argc <= LOCAL_SIZE) {
	flagPtr = localFlags;
    } else {
	flagPtr = (int *) ckalloc(memoryp, (unsigned) argc*sizeof(int));
    }
    numChars = 1;
    for (i = 0; i < argc; i++) {
	numChars += Hax_ScanElement(argv[i], &flagPtr[i]) + 1;
    }

    /*
     * Pass two: copy into the result area.
     */

    result = (char *) ckalloc(memoryp, (unsigned) numChars);
    dst = result;
    for (i = 0; i < argc; i++) {
	numChars = Hax_ConvertElement(argv[i], dst, flagPtr[i]);
	dst += numChars;
	*dst = ' ';
	dst++;
    }
    if (dst == result) {
	*dst = 0;
    } else {
	dst[-1] = 0;
    }

    if (flagPtr != localFlags) {
	ckfree(memoryp, (char *) flagPtr);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_Concat --
 *
 *	Concatenate a set of strings into a single large string.
 *
 * Results:
 *	The return value is dynamically-allocated string containing
 *	a concatenation of all the strings in argv, with spaces between
 *	the original argv elements.
 *
 * Side effects:
 *	Memory is allocated for the result;  the caller is responsible
 *	for freeing the memory.
 *
 *----------------------------------------------------------------------
 */

char *
Hax_Concat(
    Hax_Interp *interp,
    int argc,			/* Number of strings to concatenate. */
    char **argv			/* Array of strings to concatenate. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int totalSize, i;
    char *p;
    char *result;

    for (totalSize = 1, i = 0; i < argc; i++) {
	totalSize += strlen(argv[i]) + 1;
    }
    result = (char *) ckalloc(memoryp, (unsigned) totalSize);
    if (argc == 0) {
	*result = '\0';
	return result;
    }
    for (p = result, i = 0; i < argc; i++) {
	char *element;
	int length;

	/*
	 * Clip white space off the front and back of the string
	 * to generate a neater result, and ignore any empty
	 * elements.
	 */

	element = argv[i];
	while (isspace(*element)) {
	    element++;
	}
	for (length = strlen(element);
		(length > 0) && (isspace(element[length-1]));
		length--) {
	    /* Null loop body. */
	}
	if (length == 0) {
	    continue;
	}
	(void) strncpy(p, element, length);
	p += length;
	*p = ' ';
	p++;
    }
    if (p != result) {
	p[-1] = 0;
    } else {
	*p = 0;
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_StringMatch --
 *
 *	See if a particular string matches a particular pattern.
 *
 * Results:
 *	The return value is 1 if string matches pattern, and
 *	0 otherwise.  The matching operation permits the following
 *	special characters in the pattern: *?\[] (see the manual
 *	entry for details on what these mean).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Hax_StringMatch(
    char *string,	/* String. */
    char *pattern	/* Pattern, which may contain
				 * special characters. */)
{
    char c2;

    while (1) {
	/* See if we're at the end of both the pattern and the string.
	 * If so, we succeeded.  If we're at the end of the pattern
	 * but not at the end of the string, we failed.
	 */

	if (*pattern == 0) {
	    if (*string == 0) {
		return 1;
	    } else {
		return 0;
	    }
	}
	if ((*string == 0) && (*pattern != '*')) {
	    return 0;
	}

	/* Check for a "*" as the next pattern character.  It matches
	 * any substring.  We handle this by calling ourselves
	 * recursively for each postfix of string, until either we
	 * match or we reach the end of the string.
	 */

	if (*pattern == '*') {
	    pattern += 1;
	    if (*pattern == 0) {
		return 1;
	    }
	    while (1) {
		if (Hax_StringMatch(string, pattern)) {
		    return 1;
		}
		if (*string == 0) {
		    return 0;
		}
		string += 1;
	    }
	}

	/* Check for a "?" as the next pattern character.  It matches
	 * any single character.
	 */

	if (*pattern == '?') {
	    goto thisCharOK;
	}

	/* Check for a "[" as the next pattern character.  It is followed
	 * by a list of characters that are acceptable, or by a range
	 * (two characters separated by "-").
	 */

	if (*pattern == '[') {
	    pattern += 1;
	    while (1) {
		if ((*pattern == ']') || (*pattern == 0)) {
		    return 0;
		}
		if (*pattern == *string) {
		    break;
		}
		if (pattern[1] == '-') {
		    c2 = pattern[2];
		    if (c2 == 0) {
			return 0;
		    }
		    if ((*pattern <= *string) && (c2 >= *string)) {
			break;
		    }
		    if ((*pattern >= *string) && (c2 <= *string)) {
			break;
		    }
		    pattern += 2;
		}
		pattern += 1;
	    }
	    while ((*pattern != ']') && (*pattern != 0)) {
		pattern += 1;
	    }
	    goto thisCharOK;
	}

	/* If the next pattern character is '/', just strip off the '/'
	 * so we do exact matching on the character that follows.
	 */

	if (*pattern == '\\') {
	    pattern += 1;
	    if (*pattern == 0) {
		return 0;
	    }
	}

	/* There's no special character.  Just make sure that the next
	 * characters of each string match.
	 */

	if (*pattern != *string) {
	    return 0;
	}

	thisCharOK: pattern += 1;
	string += 1;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_SetResult --
 *
 *	Arrange for "string" to be the Hax return value.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	interp->result is left pointing either to "string" (if "copy" is 0)
 *	or to a copy of string.
 *
 *----------------------------------------------------------------------
 */

void
Hax_SetResult(
    Hax_Interp *interp,		/* Interpreter with which to associate the
				 * return value. */
    char *string,		/* Value to be returned.  If NULL,
				 * the result is set to an empty string. */
    Hax_FreeProc *freeProc	/* Gives information about the string:
				 * HAX_STATIC, HAX_VOLATILE, or the address
				 * of a Hax_FreeProc such as free. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int length;
    Hax_FreeProc *oldFreeProc = iPtr->freeProc;
    char *oldResult = iPtr->result;

    iPtr->freeProc = freeProc;
    if (string == NULL) {
	iPtr->resultSpace[0] = 0;
	iPtr->result = iPtr->resultSpace;
	iPtr->freeProc = 0;
    } else if (freeProc == HAX_VOLATILE) {
	length = strlen(string);
	if (length > HAX_RESULT_SIZE) {
	    iPtr->result = (char *) ckalloc(memoryp, (unsigned) length+1);
	    iPtr->freeProc = (Hax_FreeProc *) free;
	} else {
	    iPtr->result = iPtr->resultSpace;
	    iPtr->freeProc = 0;
	}
	strcpy(iPtr->result, string);
    } else {
	iPtr->result = string;
    }

    /*
     * If the old result was dynamically-allocated, free it up.  Do it
     * here, rather than at the beginning, in case the new result value
     * was part of the old result value.
     */

    if (oldFreeProc != 0) {
	if (oldFreeProc == (Hax_FreeProc *) free) {
	    ckfree(memoryp, oldResult);
	} else {
	    (*oldFreeProc)(oldResult);
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_AppendResult --
 *
 *	Append a variable number of strings onto the result already
 *	present for an interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The result in the interpreter given by the first argument
 *	is extended by the strings given by the second and following
 *	arguments (up to a terminating NULL argument).
 *
 *----------------------------------------------------------------------
 */

void
	/* VARARGS2 */ /* ARGSUSED */
Hax_AppendResult(
    Hax_Interp *interp,		/* Interpreter whose result is to be
				 * extended. */
    ...				/* One or more strings to add to the
				 * result, terminated with NULL. */)
{
    va_list argList, argList2;
    Interp *iPtr;
    char *string;
    int newSpace;

    /*
     * First, scan through all the arguments to see how much space is
     * needed.
     */

    va_start(argList, interp);
    va_copy(argList2, argList);
    iPtr = (Interp *) interp;
    newSpace = 0;
    while (1) {
	string = va_arg(argList, char *);
	if (string == NULL) {
	    break;
	}
	newSpace += strlen(string);
    }
    va_end(argList);

    /*
     * If the append buffer isn't already setup and large enough
     * to hold the new data, set it up.
     */

    if ((iPtr->result != iPtr->appendResult)
	   || ((newSpace + iPtr->appendUsed) >= iPtr->appendAvl)) {
       SetupAppendBuffer(iPtr, newSpace);
    }

    /*
     * Final step:  go through all the argument strings again, copying
     * them into the buffer.
     */

    while (1) {
	string = va_arg(argList2, char *);
	if (string == NULL) {
	    break;
	}
	strcpy(iPtr->appendResult + iPtr->appendUsed, string);
	iPtr->appendUsed += strlen(string);
    }
    va_end(argList2);
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_AppendElement --
 *
 *	Convert a string to a valid Hax list element and append it
 *	to the current result (which is ostensibly a list).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The result in the interpreter given by the first argument
 *	is extended with a list element converted from string.  If
 *	the original result wasn't empty, then a blank is added before
 *	the converted list element.
 *
 *----------------------------------------------------------------------
 */

void
Hax_AppendElement(
    Hax_Interp *interp,		/* Interpreter whose result is to be
				 * extended. */
    char *string,		/* String to convert to list element and
				 * add to result. */
    int noSep			/* If non-zero, then don't output a
				 * space character before this element,
				 * even if the element isn't the first
				 * thing in the output buffer. */)
{
    Interp *iPtr = (Interp *) interp;
    int size, flags;
    char *dst;

    /*
     * See how much space is needed, and grow the append buffer if
     * needed to accommodate the list element.
     */

    size = Hax_ScanElement(string, &flags) + 1;
    if ((iPtr->result != iPtr->appendResult)
	   || ((size + iPtr->appendUsed) >= iPtr->appendAvl)) {
       SetupAppendBuffer(iPtr, size+iPtr->appendUsed);
    }

    /*
     * Convert the string into a list element and copy it to the
     * buffer that's forming.
     */

    dst = iPtr->appendResult + iPtr->appendUsed;
    if (!noSep && (iPtr->appendUsed != 0)) {
	iPtr->appendUsed++;
	*dst = ' ';
	dst++;
    }
    iPtr->appendUsed += Hax_ConvertElement(string, dst, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * SetupAppendBuffer --
 *
 *	This procedure makes sure that there is an append buffer
 *	properly initialized for interp, and that it has at least
 *	enough room to accommodate newSpace new bytes of information.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static void
SetupAppendBuffer(
    Interp *iPtr,	/* Interpreter whose result is being set up. */
    int newSpace		/* Make sure that at least this many bytes
				 * of new information may be added. */)
{
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int totalSpace;

    /*
     * Make the append buffer larger, if that's necessary, then
     * copy the current result into the append buffer and make the
     * append buffer the official Hax result.
     */

    if (iPtr->result != iPtr->appendResult) {
	/*
	 * If an oversized buffer was used recently, then free it up
	 * so we go back to a smaller buffer.  This avoids tying up
	 * memory forever after a large operation.
	 */

	if (iPtr->appendAvl > 500) {
	    ckfree(memoryp, iPtr->appendResult);
	    iPtr->appendResult = NULL;
	    iPtr->appendAvl = 0;
	}
	iPtr->appendUsed = strlen(iPtr->result);
    }
    totalSpace = newSpace + iPtr->appendUsed;
    if (totalSpace >= iPtr->appendAvl) {
	char *newPtr;

	if (totalSpace < 100) {
	    totalSpace = 200;
	} else {
	    totalSpace *= 2;
	}
	newPtr = (char *) ckalloc(memoryp, (unsigned) totalSpace);
	strcpy(newPtr, iPtr->result);
	if (iPtr->appendResult != NULL) {
	    ckfree(memoryp, iPtr->appendResult);
	}
	iPtr->appendResult = newPtr;
	iPtr->appendAvl = totalSpace;
    } else if (iPtr->result != iPtr->appendResult) {
	strcpy(iPtr->appendResult, iPtr->result);
    }
    Hax_FreeResult((Hax_Interp *) iPtr);
    iPtr->result = iPtr->appendResult;
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_ResetResult --
 *
 *	This procedure restores the result area for an interpreter
 *	to its default initialized state, freeing up any memory that
 *	may have been allocated for the result and clearing any
 *	error information for the interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
Hax_ResetResult(
    Hax_Interp *interp		/* Interpreter for which to clear result. */)
{
    Interp *iPtr = (Interp *) interp;

    Hax_FreeResult(interp);
    iPtr->result = iPtr->resultSpace;
    iPtr->resultSpace[0] = 0;
    iPtr->flags &=
	    ~(ERR_ALREADY_LOGGED | ERR_IN_PROGRESS | ERROR_CODE_SET);
}

/*
 *----------------------------------------------------------------------
 *
 * Hax_SetErrorCode --
 *
 *	This procedure is called to record machine-readable information
 *	about an error that is about to be returned.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The errorCode global variable is modified to hold all of the
 *	arguments to this procedure, in a list form with each argument
 *	becoming one element of the list.  A flag is set internally
 *	to remember that errorCode has been set, so the variable doesn't
 *	get set automatically when the error is returned.
 *
 *----------------------------------------------------------------------
 */

void
	/* VARARGS2 */ /* ARGSUSED */
Hax_SetErrorCode(
    Hax_Interp *interp,		/* Interpreter whose errorCode variable is
				 * to be set. */
    ...				/* One or more elements to add to errorCode,
				 * terminated with NULL. */)
{
    va_list argList;
    char *string;
    int flags;
    Interp *iPtr;

    /*
     * Scan through the arguments one at a time, appending them to
     * $errorCode as list elements.
     */

    va_start(argList, interp);
    iPtr = (Interp *) interp;
    flags = HAX_GLOBAL_ONLY | HAX_LIST_ELEMENT;
    while (1) {
	string = va_arg(argList, char *);
	if (string == NULL) {
	    break;
	}
	(void) Hax_SetVar2((Hax_Interp *) iPtr, (char *) "errorCode",
		(char *) NULL, string, flags);
	flags |= HAX_APPEND_VALUE;
    }
    va_end(argList);
    iPtr->flags |= ERROR_CODE_SET;
}

/*
 *----------------------------------------------------------------------
 *
 * HaxGetListIndex --
 *
 *	Parse a list index, which may be either an integer or the
 *	value "end".
 *
 * Results:
 *	The return value is either HAX_OK or HAX_ERROR.  If it is
 *	HAX_OK, then the index corresponding to string is left in
 *	*indexPtr.  If the return value is HAX_ERROR, then string
 *	was bogus;  an error message is returned in interp->result.
 *	If a negative index is specified, it is rounded up to 0.
 *	The index value may be larger than the size of the list
 *	(this happens when "end" is specified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
HaxGetListIndex(
    Hax_Interp *interp,			/* Interpreter for error reporting. */
    char *string,			/* String containing list index. */
    long int *indexPtr			/* Where to store index. */)
{
    if (isdigit(*string) || (*string == '-')) {
	if (Hax_GetLong(interp, string, indexPtr) != HAX_OK) {
	    return HAX_ERROR;
	}
	if (*indexPtr < 0) {
	    *indexPtr = 0;
	}
    } else if (strncmp(string, "end", strlen(string)) == 0) {
	*indexPtr = 1<<30;
    } else {
	Hax_AppendResult(interp, "bad index \"", string,
		"\": must be integer or \"end\"", (char *) NULL);
	return HAX_ERROR;
    }
    return HAX_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * HaxCompileRegexp --
 *
 *	Compile a regular expression into a form suitable for fast
 *	matching.  This procedure retains a small cache of pre-compiled
 *	regular expressions in the interpreter, in order to avoid
 *	compilation costs as much as possible.
 *
 * Results:
 *	The return value is a pointer to the compiled form of string,
 *	suitable for passing to regexec.  If an error occurred while
 *	compiling the pattern, then NULL is returned and an error
 *	message is left in interp->result.
 *
 * Side effects:
 *	The cache of compiled regexp's in interp will be modified to
 *	hold information for string, if such information isn't already
 *	present in the cache.
 *
 *----------------------------------------------------------------------
 */

regexp *
HaxCompileRegexp(
    Hax_Interp *interp,			/* For use in error reporting. */
    char *string			/* String for which to produce
					 * compiled regular expression. */)
{
    Interp *iPtr = (Interp *) interp;
    Hax_Memoryp *memoryp = iPtr->memoryp;
    int i, length;
    regexp *result;

    length = strlen(string);
    for (i = 0; i < NUM_REGEXPS; i++) {
	if ((length == iPtr->patLengths[i])
		&& (strcmp(string, iPtr->patterns[i]) == 0)) {
	    /*
	     * Move the matched pattern to the first slot in the
	     * cache and shift the other patterns down one position.
	     */

	    if (i != 0) {
		int j;
		char *cachedString;

		cachedString = iPtr->patterns[i];
		result = iPtr->regexps[i];
		for (j = i-1; j >= 0; j--) {
		    iPtr->patterns[j+1] = iPtr->patterns[j];
		    iPtr->patLengths[j+1] = iPtr->patLengths[j];
		    iPtr->regexps[j+1] = iPtr->regexps[j];
		}
		iPtr->patterns[0] = cachedString;
		iPtr->patLengths[0] = length;
		iPtr->regexps[0] = result;
	    }
	    return iPtr->regexps[0];
	}
    }

    /*
     * No match in the cache.  Compile the string and add it to the
     * cache.
     */

    iPtr->haxRegexpError = NULL;
    result = RegComp(interp, string);
    if (iPtr->haxRegexpError != NULL) {
	Hax_AppendResult(interp,
	    "couldn't compile regular expression pattern: ",
	    iPtr->haxRegexpError, (char *) NULL);
	return NULL;
    }
    if (iPtr->patterns[NUM_REGEXPS-1] != NULL) {
	ckfree(memoryp, iPtr->patterns[NUM_REGEXPS-1]);
	ckfree(memoryp, (char *) iPtr->regexps[NUM_REGEXPS-1]);
    }
    for (i = NUM_REGEXPS - 2; i >= 0; i--) {
	iPtr->patterns[i+1] = iPtr->patterns[i];
	iPtr->patLengths[i+1] = iPtr->patLengths[i];
	iPtr->regexps[i+1] = iPtr->regexps[i];
    }
    iPtr->patterns[0] = (char *) ckalloc(memoryp, (unsigned) (length+1));
    strcpy(iPtr->patterns[0], string);
    iPtr->patLengths[0] = length;
    iPtr->regexps[0] = result;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * RegError --
 *
 *	This procedure is invoked by the Henry Spencer's regexp code
 *	when an error occurs.  It saves the error message so it can
 *	be seen by the code that called Spencer's code.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The value of "string" is saved in "haxRegexpError".
 *
 *----------------------------------------------------------------------
 */

void
RegError(
    Hax_Interp *interp,			/* For use in error reporting. */
    char *string			/* Error message. */)
{
    Interp *iPtr = (Interp *) interp;

    iPtr->haxRegexpError = string;
}
