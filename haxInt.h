/*
 * haxInt.h --
 *
 *	Declarations of things used internally by the Hax interpreter.
 *
 * Copyright 1987-1991 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Header: /user6/ouster/tcl/RCS/tclInt.h,v 1.70 93/01/22 15:17:35 ouster Exp $ SPRITE (Berkeley)
 */

#ifndef _HAXINT
#define _HAXINT

/*
 * Common include files needed by most of the Hax source files are
 * included here, so that system-dependent personalizations for the
 * include files only have to be made in once place.  This results
 * in a few extra includes, but greater modularity.  The order of
 * the three groups of #includes is important.  For example, stdio.h
 * is needed by hax.h.
 */

#include <stdio.h>

#ifndef _HAX
#include "hax.h"
#endif
#ifndef _HAXHASH
#include "haxHash.h"
#endif
#ifndef _REGEXP
#include "haxRegexp.h"
#endif

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 *----------------------------------------------------------------
 * Data structures related to variables.   These are used primarily
 * in haxVar.c
 *----------------------------------------------------------------
 */

/*
 * The following structure defines a variable trace, which is used to
 * invoke a specific C procedure whenever certain operations are performed
 * on a variable.
 */

typedef struct VarTrace {
    Hax_VarTraceProc *traceProc;/* Procedure to call when operations given
				 * by flags are performed on variable. */
    ClientData clientData;	/* Argument to pass to proc. */
    int flags;			/* What events the trace procedure is
				 * interested in:  OR-ed combination of
				 * HAX_TRACE_READS, HAX_TRACE_WRITES, and
				 * HAX_TRACE_UNSETS. */
    struct VarTrace *nextPtr;	/* Next in list of traces associated with
				 * a particular variable. */
} VarTrace;

/*
 * When a variable trace is active (i.e. its associated procedure is
 * executing), one of the following structures is linked into a list
 * associated with the variable's interpreter.  The information in
 * the structure is needed in order for Hax to behave reasonably
 * if traces are deleted while traces are active.
 */

typedef struct ActiveVarTrace {
    struct ActiveVarTrace *nextPtr;
				/* Next in list of all active variable
				 * traces for the interpreter, or NULL
				 * if no more. */
    VarTrace *nextTracePtr;	/* Next trace to check after current
				 * trace procedure returns;  if this
				 * trace gets deleted, must update pointer
				 * to avoid using free'd memory. */
} ActiveVarTrace;

/*
 * The following structure describes an enumerative search in progress on
 * an array variable;  this are invoked with options to the "array"
 * command.
 */

typedef struct ArraySearch {
    int id;			/* Integer id used to distinguish among
				 * multiple concurrent searches for the
				 * same array. */
    struct Var *varPtr;		/* Pointer to array variable that's being
				 * searched. */
    Hax_HashSearch search;	/* Info kept by the hash module about
				 * progress through the array. */
    Hax_HashEntry *nextEntry;	/* Non-null means this is the next element
				 * to be enumerated (it's leftover from
				 * the Hax_FirstHashEntry call or from
				 * an "array anymore" command).  NULL
				 * means must call Hax_NextHashEntry
				 * to get value to return. */
    struct ArraySearch *nextPtr;/* Next in list of all active searches
				 * for this variable, or NULL if this is
				 * the last one. */
} ArraySearch;

/*
 * The structure below defines a variable, which associates a string name
 * with a string value.  Pointers to these structures are kept as the
 * values of hash table entries, and the name of each variable is stored
 * in the hash entry.
 */

typedef struct Var {
    int valueLength;		/* Holds the number of non-null bytes
				 * actually occupied by the variable's
				 * current value in value.string (extra
				 * space is sometimes left for expansion).
				 * For array and global variables this is
				 * meaningless. */
    int valueSpace;		/* Total number of bytes of space allocated
				 * at value. */
    int upvarUses;		/* Counts number of times variable is
				 * is referenced via global or upvar variables
				 * (i.e. how many variables have "upvarPtr"
				 * pointing to this variable).  Variable
				 * can't be deleted until this count reaches
				 * 0. */
    VarTrace *tracePtr;		/* First in list of all traces set for this
				 * variable. */
    ArraySearch *searchPtr;	/* First in list of all searches active
				 * for this variable, or NULL if none. */
    int flags;			/* Miscellaneous bits of information about
				 * variable.  See below for definitions. */
    union {
	char string[4];		/* String value of variable.  The actual
				 * length of this field is given by the
				 * valueSpace field above. */
	Hax_HashTable *tablePtr;/* For array variables, this points to
				 * information about the hash table used
				 * to implement the associative array. 
				 * Points to malloc-ed data. */
	Hax_HashEntry *upvarPtr;
				/* If this is a global variable being
				 * referred to in a procedure, or a variable
				 * created by "upvar", this field points to
				 * the hash table entry for the higher-level
				 * variable. */
    } value;			/* MUST BE LAST FIELD IN STRUCTURE!!! */
} Var;

/*
 * Flag bits for variables:
 *
 * VAR_ARRAY	-		1 means this is an array variable rather
 *				than a scalar variable.
 * VAR_UPVAR - 			1 means this variable just contains a
 *				pointer to another variable that has the
 *				real value.  Variables like this come
 *				about through the "upvar" and "global"
 *				commands.
 * VAR_UNDEFINED -		1 means that the variable is currently
 *				undefined.  Undefined variables usually
 *				go away completely, but if an undefined
 *				variable has a trace on it, or if it is
 *				a global variable being used by a procedure,
 *				then it stays around even when undefined.
 * VAR_ELEMENT_ACTIVE -		Used only in array variables;  1 means that
 *				an element of the array is currently being
 *				manipulated in some way, so that it isn't
 *				safe to delete the whole array.
 * VAR_TRACE_ACTIVE -		1 means that trace processing is currently
 *				underway for a read or write access, so
 *				new read or write accesses should not cause
 *				trace procedures to be called and the
 *				variable can't be deleted.
 */

#define VAR_ARRAY		1
#define VAR_UPVAR		2
#define VAR_UNDEFINED		4
#define VAR_ELEMENT_ACTIVE	0x10
#define VAR_TRACE_ACTIVE	0x20
#define VAR_SEARCHES_POSSIBLE	0x40

/*
 *----------------------------------------------------------------
 * Data structures related to procedures.   These are used primarily
 * in haxProc.c
 *----------------------------------------------------------------
 */

/*
 * The structure below defines an argument to a procedure, which
 * consists of a name and an (optional) default value.
 */

typedef struct Arg {
    struct Arg *nextPtr;	/* Next argument for this procedure,
				 * or NULL if this is the last argument. */
    char *defValue;		/* Pointer to arg's default value, or NULL
				 * if no default value. */
    char name[4];		/* Name of argument starts here.  The name
				 * is followed by space for the default,
				 * if there is one.  The actual size of this
				 * field will be as large as necessary to
				 * hold both name and default value.  THIS
				 * MUST BE THE LAST FIELD IN THE STRUCTURE!! */
} Arg;

/*
 * The structure below defines a command procedure, which consists of
 * a collection of Hax commands plus information about arguments and
 * variables.
 */

typedef struct Proc {
    struct Interp *iPtr;	/* Interpreter for which this command
				 * is defined. */
    char *command;		/* Command that constitutes the body of
				 * the procedure (dynamically allocated). */
    Arg *argPtr;		/* Pointer to first of procedure's formal
				 * arguments, or NULL if none. */
} Proc;

/*
 * The structure below defines a command trace.  This is used to allow Hax
 * clients to find out whenever a command is about to be executed.
 */

typedef struct Trace {
    int level;			/* Only trace commands at nesting level
				 * less than or equal to this. */
    Hax_CmdTraceProc *proc;	/* Procedure to call to trace command. */
    ClientData clientData;	/* Arbitrary value to pass to proc. */
    struct Trace *nextPtr;	/* Next in list of traces for this interp. */
} Trace;

/*
 * The structure below defines a frame, which is a procedure invocation.
 * These structures exist only while procedures are being executed, and
 * provide a sort of call stack.
 */

typedef struct CallFrame {
    Hax_HashTable varTable;	/* Hash table containing all of procedure's
				 * local variables. */
    int level;			/* Level of this procedure, for "uplevel"
				 * purposes (i.e. corresponds to nesting of
				 * callerVarPtr's, not callerPtr's).  1 means
				 * outer-most procedure, 0 means top-level. */
    int argc;			/* This and argv below describe name and
				 * arguments for this procedure invocation. */
    char **argv;		/* Array of arguments. */
    struct CallFrame *callerPtr;
				/* Value of interp->framePtr when this
				 * procedure was invoked (i.e. next in
				 * stack of all active procedures). */
    struct CallFrame *callerVarPtr;
				/* Value of interp->varFramePtr when this
				 * procedure was invoked (i.e. determines
				 * variable scoping within caller;  same
				 * as callerPtr unless an "uplevel" command
				 * or something equivalent was active in
				 * the caller). */
} CallFrame;

/*
 * The structure below defines one history event (a previously-executed
 * command that can be re-executed in whole or in part).
 */

typedef struct {
    char *command;		/* String containing previously-executed
				 * command. */
    int bytesAvl;		/* Total # of bytes available at *event (not
				 * all are necessarily in use now). */
} HistoryEvent;

/*
 *----------------------------------------------------------------
 * Data structures related to history.   These are used primarily
 * in haxHistory.c
 *----------------------------------------------------------------
 */

/*
 * The structure below defines a pending revision to the most recent
 * history event.  Changes are linked together into a list and applied
 * during the next call to Hax_RecordHistory.  See the comments at the
 * beginning of haxHistory.c for information on revisions.
 */

typedef struct HistoryRev {
    int firstIndex;		/* Index of the first byte to replace in
				 * current history event. */
    int lastIndex;		/* Index of last byte to replace in
				 * current history event. */
    int newSize;		/* Number of bytes in newBytes. */
    char *newBytes;		/* Replacement for the range given by
				 * firstIndex and lastIndex. */
    struct HistoryRev *nextPtr;	/* Next in chain of revisions to apply, or
				 * NULL for end of list. */
} HistoryRev;

/*
 *----------------------------------------------------------------
 * Data structures related to files.  These are used primarily in
 * haxUnixUtil.c and haxUnixAZ.c.
 *----------------------------------------------------------------
 */

/*
 * The data structure below defines an open file (or connection to
 * a process pipeline) as returned by the "open" command.
 */

typedef struct OpenFile {
    FILE *f;			/* Stdio file to use for reading and/or
				 * writing. */
    FILE *f2;			/* Normally NULL.  In the special case of
				 * a command pipeline with pipes for both
				 * input and output, this is a stdio file
				 * to use for writing to the pipeline. */
    int readable;		/* Non-zero means file may be read. */
    int writable;		/* Non-zero means file may be written. */
    int numPids;		/* If this is a connection to a process
				 * pipeline, gives number of processes
				 * in pidPtr array below;  otherwise it
				 * is 0. */
    int *pidPtr;		/* Pointer to malloc-ed array of child
				 * process ids (numPids of them), or NULL
				 * if this isn't a connection to a process
				 * pipeline. */
    int errorId;		/* File id of file that receives error
				 * output from pipeline.  -1 means not
				 * used (i.e. this is a normal file). */
} OpenFile;

/*
 *----------------------------------------------------------------
 * This structure defines an interpreter, which is a collection of
 * commands plus other state information related to interpreting
 * commands, such as variable storage.  Primary responsibility for
 * this data structure is in haxBasic.c, but almost every Hax
 * source file uses something in here.
 *----------------------------------------------------------------
 */

typedef struct Command {
    Hax_CmdProc *proc;		/* Procedure to process command. */
    ClientData clientData;	/* Arbitrary value to pass to proc. */
    Hax_CmdDeleteProc *deleteProc;
				/* Procedure to invoke when deleting
				 * command. */
} Command;

#define CMD_SIZE(nameLength) ((unsigned) sizeof(Command) + nameLength - 3)

typedef struct Interp {

    /*
     * Note:  the first three fields must match exactly the fields in
     * a Hax_Interp struct (see hax.h).  If you change one, be sure to
     * change the other.
     */

    char *result;		/* Points to result returned by last
				 * command. */
    Hax_FreeProc *freeProc;	/* Zero means result is statically allocated.
				 * If non-zero, gives address of procedure
				 * to invoke to free the result.  Must be
				 * freed by Hax_Eval before executing next
				 * command. */
    int errorLine;		/* When HAX_ERROR is returned, this gives
				 * the line number within the command where
				 * the error occurred (1 means first line). */
    Hax_HashTable commandTable;	/* Contains all of the commands currently
				 * registered in this interpreter.  Indexed
				 * by strings; values have type (Command *). */

    /*
     * Information related to procedures and variables.  See haxProc.c
     * and haxvar.c for usage.
     */

    Hax_HashTable globalTable;	/* Contains all global variables for
				 * interpreter. */
    int numLevels;		/* Keeps track of how many nested calls to
				 * Hax_Eval are in progress for this
				 * interpreter.  It's used to delay deletion
				 * of the table until all Hax_Eval invocations
				 * are completed. */
    CallFrame *framePtr;	/* Points to top-most in stack of all nested
				 * procedure invocations.  NULL means there
				 * are no active procedures. */
    CallFrame *varFramePtr;	/* Points to the call frame whose variables
				 * are currently in use (same as framePtr
				 * unless an "uplevel" command is being
				 * executed).  NULL means no procedure is
				 * active or "uplevel 0" is being exec'ed. */
    ActiveVarTrace *activeTracePtr;
				/* First in list of active traces for interp,
				 * or NULL if no active traces. */

    /*
     * Information related to history:
     */

    int numEvents;		/* Number of previously-executed commands
				 * to retain. */
    HistoryEvent *events;	/* Array containing numEvents entries
				 * (dynamically allocated). */
    int curEvent;		/* Index into events of place where current
				 * (or most recent) command is recorded. */
    int curEventNum;		/* Event number associated with the slot
				 * given by curEvent. */
    HistoryRev *revPtr;		/* First in list of pending revisions. */
    char *historyFirst;		/* First char. of current command executed
				 * from history module or NULL if none. */
    int revDisables;		/* 0 means history revision OK;  > 0 gives
				 * a count of number of times revision has
				 * been disabled. */
    char *evalFirst;		/* If HAX_RECORD_BOUNDS flag set, Hax_Eval
				 * sets this field to point to the first
				 * char. of text from which the current
				 * command came.  Otherwise Hax_Eval sets
				 * this to NULL. */
    char *evalLast;		/* Similar to evalFirst, except points to
				 * last character of current command. */

    /*
     * Information used by Hax_AppendResult to keep track of partial
     * results.  See Hax_AppendResult code for details.
     */

    char *appendResult;		/* Storage space for results generated
				 * by Hax_AppendResult.  Malloc-ed.  NULL
				 * means not yet allocated. */
    int appendAvl;		/* Total amount of space available at
				 * partialResult. */
    int appendUsed;		/* Number of non-null bytes currently
				 * stored at partialResult. */

    /*
     * Information related to files.  See haxUnixAZ.c and haxUnixUtil.c
     * for details.
     */

    int numFiles;		/* Number of entries in filePtrArray
				 * below.  0 means array hasn't been
				 * created yet. */
    OpenFile **filePtrArray;	/* Pointer to malloc-ed array of pointers
				 * to information about open files.  Entry
				 * N corresponds to the file with fileno N.
				 * If an entry is NULL then the corresponding
				 * file isn't open.  If filePtrArray is NULL
				 * it means no files have been used, so even
				 * stdin/stdout/stderr entries haven't been
				 * setup yet. */
    /*
     * A cache of compiled regular expressions.  See HaxCompileRegexp
     * in haxUtil.c for details.
     */

#define NUM_REGEXPS 5
    char *patterns[NUM_REGEXPS];/* Strings corresponding to compiled
				 * regular expression patterns.  NULL
				 * means that this slot isn't used.
				 * Malloc-ed. */
    int patLengths[NUM_REGEXPS];/* Number of non-null characters in
				 * corresponding entry in patterns.
				 * -1 means entry isn't used. */
    regexp *regexps[NUM_REGEXPS];
				/* Compiled forms of above strings.  Also
				 * malloc-ed, or NULL if not in use yet. */


    /*
     * Miscellaneous information:
     */

    int cmdCount;		/* Total number of times a command procedure
				 * has been called for this interpreter. */
    int noEval;			/* Non-zero means no commands should actually
				 * be executed:  just parse only.  Used in
				 * expressions when the result is already
				 * determined. */
    char *scriptFile;		/* NULL means there is no nested source
				 * command active;  otherwise this points to
				 * the name of the file being sourced (it's
				 * not malloc-ed:  it points to an argument
				 * to Hax_EvalFile. */
    int flags;			/* Various flag bits.  See below. */
    Trace *tracePtr;		/* List of traces for this interpreter. */
    char resultSpace[HAX_RESULT_SIZE+1];
				/* Static space for storing small results. */
} Interp;

/*
 * Flag bits for Interp structures:
 *
 * DELETED:		Non-zero means the interpreter has been deleted:
 *			don't process any more commands for it, and destroy
 *			the structure as soon as all nested invocations of
 *			Hax_Eval are done.
 * ERR_IN_PROGRESS:	Non-zero means an error unwind is already in progress.
 *			Zero means a command proc has been invoked since last
 *			error occured.
 * ERR_ALREADY_LOGGED:	Non-zero means information has already been logged
 *			in $errorInfo for the current Hax_Eval instance,
 *			so Hax_Eval needn't log it (used to implement the
 *			"error message log" command).
 * ERROR_CODE_SET:	Non-zero means that Hax_SetErrorCode has been
 *			called to record information for the current
 *			error.  Zero means Hax_Eval must clear the
 *			errorCode variable if an error is returned.
 */

#define DELETED			1
#define ERR_IN_PROGRESS		2
#define ERR_ALREADY_LOGGED	4
#define ERROR_CODE_SET		8

/*
 *----------------------------------------------------------------
 * Data structures related to command parsing.   These are used in
 * haxParse.c and its clients.
 *----------------------------------------------------------------
 */

/*
 * The following data structure is used by various parsing procedures
 * to hold information about where to store the results of parsing
 * (e.g. the substituted contents of a quoted argument, or the result
 * of a nested command).  At any given time, the space available
 * for output is fixed, but a procedure may be called to expand the
 * space available if the current space runs out.
 */

typedef struct ParseValue {
    char *buffer;		/* Address of first character in
				 * output buffer. */
    char *next;			/* Place to store next character in
				 * output buffer. */
    char *end;			/* Address of the last usable character
				 * in the buffer. */
    void (*expandProc) (struct ParseValue *pvPtr, int needed);
				/* Procedure to call when space runs out;
				 * it will make more space. */
    ClientData clientData;	/* Arbitrary information for use of
				 * expandProc. */
} ParseValue;

/*
 * A table used to classify input characters to assist in parsing
 * Hax commands.  The table should be indexed with a signed character
 * using the CHAR_TYPE macro.  The character may have a negative
 * value.
 */

extern char haxTypeTable[];
#define CHAR_TYPE(c) (haxTypeTable+128)[c]

/*
 * Possible values returned by CHAR_TYPE:
 *
 * HAX_NORMAL -		All characters that don't have special significance
 *			to the Hax language.
 * HAX_SPACE -		Character is space, tab, or return.
 * HAX_COMMAND_END -	Character is newline or null or semicolon or
 *			close-bracket.
 * HAX_QUOTE -		Character is a double-quote.
 * HAX_OPEN_BRACKET -	Character is a "[".
 * HAX_OPEN_BRACE -	Character is a "{".
 * HAX_CLOSE_BRACE -	Character is a "}".
 * HAX_BACKSLASH -	Character is a "\".
 * HAX_DOLLAR -		Character is a "$".
 */

#define HAX_NORMAL		0
#define HAX_SPACE		1
#define HAX_COMMAND_END		2
#define HAX_QUOTE		3
#define HAX_OPEN_BRACKET	4
#define HAX_OPEN_BRACE		5
#define HAX_CLOSE_BRACE		6
#define HAX_BACKSLASH		7
#define HAX_DOLLAR		8

/*
 * Additional flags passed to Hax_Eval.  See hax.h for other flags to
 * Hax_Eval;  these ones are only used internally by Hax.
 *
 * HAX_RECORD_BOUNDS	Tells Hax_Eval to record information in the
 *			evalFirst and evalLast fields for each command
 *			executed directly from the string (top-level
 *			commands and those from command substitution).
 */

#define HAX_RECORD_BOUNDS	0x100

/*
 * Maximum number of levels of nesting permitted in Hax commands.
 */

#define MAX_NESTING_DEPTH	100

/*
 * Variables shared among Hax modules but not used by the outside
 * world:
 */

extern char *		haxRegexpError;

/*
 *----------------------------------------------------------------
 * Procedures shared among Hax modules but not used by the outside
 * world:
 *----------------------------------------------------------------
 */

extern void		Hax_Panic(char *format, ...);
extern regexp *		HaxCompileRegexp (Hax_Interp *interp,
			    char *string);
extern void		HaxCopyAndCollapse (int count, char *src,
			    char *dst);
extern void		HaxDeleteVars (Interp *iPtr,
			    Hax_HashTable *tablePtr);
extern void		HaxExpandParseValue (ParseValue *pvPtr,
			    int needed);
extern int		HaxFindElement (Hax_Interp *interp,
			    char *list, char **elementPtr, char **nextPtr,
			    int *sizePtr, int *bracePtr);
extern Proc *		HaxFindProc (Interp *iPtr,
			    char *procName);
extern int		HaxGetFrame (Hax_Interp *interp,
			    char *string, CallFrame **framePtrPtr);
extern int		HaxGetListIndex (Hax_Interp *interp,
			    char *string, int *indexPtr);
extern int		HaxGetOpenFile (Hax_Interp *interp,
			    char *string, OpenFile **filePtrPtr);
extern Proc *		HaxIsProc (Command *cmdPtr);
extern void		HaxMakeFileTable (Interp *iPtr,
			    int index);
extern int		HaxParseBraces (Hax_Interp *interp,
			    char *string, char **termPtr, ParseValue *pvPtr);
extern int		HaxParseNestedCmd (Hax_Interp *interp,
			    char *string, int flags, char **termPtr,
			    ParseValue *pvPtr);
extern int		HaxParseQuotes (Hax_Interp *interp,
			    char *string, int termChar, int flags,
			    char **termPtr, ParseValue *pvPtr);
extern int		HaxParseWords (Hax_Interp *interp,
			    char *string, int flags, int maxWords,
			    char **termPtr, int *argcPtr, char **argv,
			    ParseValue *pvPtr);
extern void		HaxSetupEnv (Hax_Interp *interp);
extern char *		HaxWordEnd (char *start, int nested);

/*
 *----------------------------------------------------------------
 * Command procedures in the generic core:
 *----------------------------------------------------------------
 */

extern int	Hax_AppendCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ArrayCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_BreakCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_CaseCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_CatchCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ConcatCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ContinueCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ErrorCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_EvalCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ExprCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ForCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ForeachCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_FormatCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_GlobalCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_HistoryCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_IfCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_IncrCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_InfoCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_JoinCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LappendCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LindexCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LinsertCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LlengthCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ListCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LrangeCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LreplaceCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LsearchCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_LsortCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ProcCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_RegexpCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_RegsubCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_RenameCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ReturnCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ScanCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_SetCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_SplitCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_StringCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_TraceCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_UnsetCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_UplevelCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_UpvarCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_WhileCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_Cmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_Cmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);

/*
 *----------------------------------------------------------------
 * Command procedures in the UNIX core:
 *----------------------------------------------------------------
 */

extern int	Hax_CdCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_CloseCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_EofCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ExecCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ExitCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_FileCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_FlushCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_GetsCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_GlobCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_OpenCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_PutsCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_PwdCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_ReadCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_SeekCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_SourceCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_TellCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);
extern int	Hax_TimeCmd (ClientData clientData,
		    Hax_Interp *interp, int argc, char **argv);

#endif /* _HAXINT */