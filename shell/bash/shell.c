/* shell.c -- GNU's idea of the POSIX shell specification.

   This file is part of Bash, the Bourne Again SHell.  Bash is free
   software; no one can prevent you from reading the source code, or
   giving it to someone else.  This file is copyrighted under the GNU
   General Public License, which can be found in the file called
   COPYING.

   Copyright (C) 1988, 1991 Free Software Foundation, Inc.

   This file is part of GNU Bash.

   Bash is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves
   any particular purpose or works at all, unless he says so in
   writing.  Refer to the GNU Emacs General Public License for full
   details.

   Everyone is granted permission to copy, modify and redistribute
   Bash, but only under the conditions described in the GNU General
   Public License.  A copy of this license is supposed to have been
   given to you along with GNU Emacs so you can know your rights and
   responsibilities.  It should be in a file named COPYING.

   Among other things, the copyright notice and this notice must be
   preserved on all copies.

  Birthdate:
  Sunday, January 10th, 1988.
  Initial author: Brian Fox
*/
#define INSTALL_DEBUG_MODE

#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/file.h>
#include <pwd.h>
#include "posixstat.h"
#include "filecntl.h"
#include "bashansi.h"

#if defined (HAVE_VFPRINTF)
#include <varargs.h>
#endif

#include "shell.h"
#include "flags.h"

#if defined (JOB_CONTROL)
#include "jobs.h"
#endif /* JOB_CONTROL */

#if defined (BUFFERED_INPUT)
#  include "input.h"
#endif /* BUFFERED_INPUT */

#if defined (USG) && !defined (isc386) && !defined (sgi) && !defined (Linux)
extern struct passwd *getpwuid ();
#endif

extern char *dist_version;
extern int build_version;

#if defined (HISTORY)
extern void using_history ();
#endif /* HISTORY */

extern char *find_path_file ();
extern char *tilde_expand ();
extern char *decode_prompt_string ();

extern int yydebug;
#if !defined (errno)
extern int errno;
#endif

extern int subshell_environment; /* Found in execute_cmd.c. */

#if defined (RESTRICTED_SHELL)
/* Non-zero means this shell is restricted. */
extern int restricted;		/* in flags.c */
#endif

/* Non-zero means that this shell has already been run; i.e. you should
   call shell_reinitialize () if you need to start afresh. */
static int shell_initialized = 0;

/* The current maintainer of the shell.  You change this in the
   Makefile. */
#if !defined (MAINTAINER)
#define MAINTAINER "deliberately-anonymous"
#endif

char *the_current_maintainer = MAINTAINER;

#if !defined (PPROMPT)
#  define PPROMPT "bash\\$ "
#endif
char *primary_prompt = PPROMPT;

#if !defined (SPROMPT)
#  define SPROMPT "> "
#endif
char *secondary_prompt = SPROMPT;

COMMAND *global_command = (COMMAND *)NULL;

/* Non-zero after SIGINT. */
int interrupt_state = 0;

/* Information about the current user. */
struct user_info current_user =
{
  -1, -1, -1, -1, (char *)NULL, (char *)NULL, (char *)NULL
};

/* The current host's name. */
char *current_host_name = (char *)NULL;

/* Non-zero means that this shell is a login shell.
   Specifically:
   0 = not login shell.
   1 = login shell from getty (or equivalent fake out)
  -1 = login shell from "-login" flag.
  -2 = both from getty, and from flag.
 */
int login_shell = 0;

/* Non-zero means that at this moment, the shell is interactive. */
int interactive = 0;

/* Non-zero means that the shell was started as an interactive shell. */
int interactive_shell = 0;

/* Tells what state the shell was in when it started:
	0 = non-interactive shell script
	1 = interactive
	2 = -c command
   This is a superset of the information provided by interactive_shell.
*/
int startup_state = 0;

#if defined (HISTORY)
/* Non-zero means to remember lines typed to the shell on the history
   list.  This is different than the user-controlled behaviour; this
   becomes zero when we read lines from a file, for example. */
int remember_on_history = 1;
#endif /* HISTORY */

/* Special debugging helper. */
int debugging_login_shell = 0;

/* The environment that the shell passes to other commands. */
char **shell_environment;

/* Non-zero when we are executing a top-level command. */
int executing = 0;

/* The number of commands executed so far. */
int current_command_number = 1;

/* The environment at the top-level REP loop.  We use this in the case of
   error return. */
jmp_buf top_level, catch;

#if defined (JOB_CONTROL) || defined (_POSIX_VERSION)
/* The signal masks that this shell runs with. */
sigset_t top_level_mask;
#endif /* JOB_CONTROL */

/* Non-zero is the recursion depth for commands. */
int indirection_level = 0;

/* The number of times BASH has been executed.  This is set
   by initialize_variables () in variables.c. */
int shell_level = 0;

/* The name of this shell, as taken from argv[0]. */
char *shell_name = (char *)NULL;

/* time in seconds when the shell was started */
time_t shell_start_time;

/* The name of the .(shell)rc file. */
static char *bashrc_file = "~/.bashrc";

/* Non-zero means to act more like the Bourne shell on startup. */
static int act_like_sh = 0;

/* Values for the long-winded argument names. */
static int debugging = 0;		/* Do debugging things. */
static int no_rc = 0;			/* Don't execute ~/.bashrc */
static int no_profile = 0;		/* Don't execute .profile */
static int do_version = 0;		/* Display interesting version info. */
static int quiet = 0;			/* Be quiet when starting up. */
static int make_login_shell = 0;	/* Make this shell be a `-bash' shell. */

int no_line_editing = 0;	/* Don't do fancy line editing. */
int no_brace_expansion = 0;	/* Non-zero means no foo{a,b} -> fooa foob. */

int posixly_correct = 0;	/* Non-zero means posix.2 superset. */

/* Some long-winded argument names.  These are obviously new. */
#define Int 1
#define Charp 2
struct {
  char *name;
  int type;
  int *int_value;
  char **char_value;
} long_args[] = {
  { "debug", Int, &debugging, (char **)0x0 },
  { "norc", Int, &no_rc, (char **)0x0 },
  { "noprofile", Int, &no_profile, (char **)0x0 },
  { "rcfile", Charp, (int *)0x0, &bashrc_file },
  { "version", Int, &do_version, (char **)0x0 },
  { "quiet", Int, &quiet, (char **)0x0 },
  { "login", Int, &make_login_shell, (char **)0x0 },
  { "nolineediting", Int, &no_line_editing, (char **)0x0 },
  { "nobraceexpansion", Int, &no_brace_expansion, (char **)0x0 },
  { "posix", Int, &posixly_correct, (char **)0x0 },
  { (char *)0x0, Int, (int *)0x0, (char **)0x0 }
};

#if defined (HISTORY)
/* The number of lines that Bash has added to this history session. */
int history_lines_this_session = 0;

/* The number of lines that Bash has read from the history file. */
int history_lines_in_file = 0;
#endif /* HISTORY */

/* These are extern so execute_simple_command can set them, and then
   longjmp back to main to execute a shell script, instead of calling
   main () again and resulting in indefinite, possibly fatal, stack
   growth. */
jmp_buf subshell_top_level;
int subshell_argc;
char **subshell_argv;
char **subshell_envp;

#if defined (BUFFERED_INPUT)
/* The file descriptor from which the shell is reading input. */
int default_buffered_input = -1;
#endif

static char *local_pending_command = (char *)NULL;

static int issock ();
static void run_startup_files ();

main (argc, argv, env)
     int argc;
     char **argv, **env;
{
  extern int last_command_exit_value;
  register int i;
  int arg_index, locally_skip_execution;
  int top_level_arg_index, read_from_stdin;
  FILE *default_input;
#if defined (JOB_CONTROL)
  extern int job_control;
#endif /* JOB_CONTROL */

#if defined (AUX)
#include <compat.h>
  set42sig ();
  setcompat (getcompat() | COMPAT_BSDGROUPS | COMPAT_BSDSIGNALS |
	     COMPAT_BSDTTY | COMPAT_EXEC | COMPAT_SYSCALLS);
#endif /* AUX */

  /* There is a bug in the NeXT 2.1 rlogind that causes opens
     of /dev/tty to fail. */
#if defined (NeXT)
  {
    int tty_fd;

    tty_fd = open ("/dev/tty", O_RDWR);

    if (tty_fd < 0)
      {
	char *tty;
	tty = (char *)ttyname (fileno (stdin));
	tty_fd = open (tty, O_RDWR);
      }
    close (tty_fd);
  }
#endif /* NeXT */

  /* Wait forever if we are debugging a login shell. */
  while (debugging_login_shell);

  current_user.uid = getuid ();
  current_user.gid = getgid ();
  current_user.euid = geteuid ();
  current_user.egid = getegid ();

  /* See whether or not we are running setuid or setgid. */
  privileged_mode = (current_user.uid != current_user.euid) ||
		    (current_user.gid != current_user.egid);

  posixly_correct = getenv ("POSIXLY_CORRECT") != (char *)NULL;

#if defined (USE_GNU_MALLOC_LIBRARY)
  {
    extern void programming_error ();

    mcheck (programming_error, (void (*) ())0);
  }
#endif /* USE_GNU_MALLOC_LIBRARY */

  if (setjmp (subshell_top_level))
    {
      argc = subshell_argc;
      argv = subshell_argv;
      env = subshell_envp;
    }

  /* Initialize local variables for all `invocations' of main (). */
  arg_index = 1;
  local_pending_command = (char *)NULL;
  locally_skip_execution = 0;
  read_from_stdin = 0;
  default_input = stdin;
#if defined (BUFFERED_INPUT)
  default_buffered_input = -1;
#endif

  /* Fix for the `infinite process creation' bug when running shell scripts
     from startup files on System V. */
  login_shell = make_login_shell = 0;

  /* If this shell has already been run, then reinitialize it to a
     vanilla state. */
  if (shell_initialized || shell_name)
    {
      /* Make sure that we do not infinitely recurse as a login shell. */
      if (*shell_name == '-')
	shell_name++;

      shell_reinitialize ();
      if (setjmp (top_level))
	exit (2);
    }

  /* Here's a hack.  If the name of this shell is "sh", then don't do
     any startup files; just try to be more like /bin/sh. */
  {
    char *tshell_name = base_pathname (argv[0]);

    if (*tshell_name == '-')
      tshell_name++;

    if (strcmp (tshell_name, "sh") == 0)
      act_like_sh++;
  }

  yydebug = 0;

  shell_environment = env;
  shell_name = argv[0];
  dollar_vars[0] = savestring (shell_name);

  if (*shell_name == '-')
    {
      shell_name++;
      login_shell++;
    }

#if defined (JOB_CONTROL)
  if (act_like_sh)
    job_control = 0;
#endif /* JOB_CONTROL */

  shell_start_time = NOW;	/* NOW now defined in general.h */

  /* A program may start an interactive shell with
	  "execl ("/bin/bash", "-", NULL)".
     If so, default the name of this shell to our name. */
  if (!shell_name || !*shell_name || (strcmp (shell_name, "-") == 0))
    shell_name = "bash";

  /* Parse argument flags from the input line. */

  /* Find full word arguments first. */
  while ((arg_index != argc) && *(argv[arg_index]) == '-')
    {
      for (i = 0; long_args[i].name; i++)
	{
	  if (strcmp (&(argv[arg_index][1]), long_args[i].name) == 0)
	    {
	      if (long_args[i].type == Int)
		*(long_args[i].int_value) = 1;
	      else
		{
		  if (!argv[++arg_index])
		    {
		      report_error ("%s: Flag `%s' expected an argument",
				    shell_name, long_args[i].name);
		      exit (1);
		    }
		  else
		    *long_args[i].char_value = argv[arg_index];
		}
	      goto handle_next_arg;
	    }
	}
      break;			/* No such argument.  Maybe flag arg. */
    handle_next_arg:
      arg_index++;
    }

  /* If we're in a strict Posix.2 mode, turn on interactive comments. */
  if (posixly_correct)
    interactive_comments = 1;

  /* If user supplied the "-login" flag, then set and invert LOGIN_SHELL. */
  if (make_login_shell)
    {
      login_shell++;
      login_shell = -login_shell;
    }

  /* All done with full word args; do standard shell arg parsing.*/
  while (arg_index != argc && argv[arg_index] &&
	 (*(argv[arg_index]) == '-' || (*argv[arg_index] == '+')))
    {
      /* There are flag arguments, so parse them. */
      int arg_character;
      int on_or_off = (*argv[arg_index]);
      char *o_option;
      int next_arg = arg_index + 1;

      i = 1;

      /* A single `-' signals the end of options.  From the 4.3 BSD sh.
	 An option `--' means the same thing; this is the standard
	 getopt () meaning. */
      if (((argv[arg_index][0] == '-') && (argv[arg_index][1] == '\0')) ||
	  (strcmp (argv[arg_index], "--") == 0))
	{
	  arg_index++;
	  goto after_flags;
	}

      while (arg_character = (argv[arg_index])[i++])
	{
	  switch (arg_character)
	    {
	    case 'c':
	      /* The next arg is a command to execute, and the following args
		 are $1 .. $n respectively. */
		local_pending_command = argv[++arg_index];
		if (!local_pending_command)
		  {
		    report_error ("`%cc' requires an argument", on_or_off);
		    exit (1);
		  }
		arg_index++;
		goto after_flags;

	    case 's':
		read_from_stdin = 1;
		break;

	    case 'o':
		o_option = argv[next_arg++];
		if (!o_option)
		  {
		    report_error ("`%co' requires an argument", on_or_off);
		    exit (1);
		  }

		if (set_minus_o_option (on_or_off, o_option) != EXECUTION_SUCCESS)
		  exit (1);
		break;

	    default:
	      if (change_flag_char (arg_character, on_or_off) == FLAG_ERROR)
		{
		  report_error ("%c%c: bad option", on_or_off, arg_character);
		  exit (1);
		}

	    }
	}
      /* Can't do just a simple increment anymore -- what about
	 "bash -abouo emacs ignoreeof -hO"? */
      arg_index = next_arg;
    }

 after_flags:

  /* First, let the outside world know about our interactive status.
     A shell is interactive if the `-i' flag was given, or if all of
     the following conditions are met:
	no -c command
	no arguments remaining or the -s flag given
	standard input is a terminal
	standard output is a terminal
     Refer to Posix.2, the description of the `sh' utility. */

  if (forced_interactive ||		/* -i flag */
      (!local_pending_command &&	/* No -c command and ... */
       ((arg_index == argc) ||		/*   no remaining args or... */
	read_from_stdin) &&		/*   -s flag with args, and */
       isatty (fileno (stdin)) &&	/* Input is a terminal and */
       isatty (fileno (stdout))))	/* output is a terminal. */
    {
      interactive_shell = startup_state = 1;
      interactive = 1;
    }
  else
    {
#if defined (HISTORY)
      history_expansion = 0;
      remember_on_history = 0;
#endif /* HISTORY */
      interactive_shell = startup_state = 0;
      interactive = 0;
      no_line_editing = 1;
#if defined (JOB_CONTROL)
      job_control = 0;
#endif /* JOB_CONTROL */
    }

#define CLOSE_FDS_AT_LOGIN

#if defined (CLOSE_FDS_AT_LOGIN)
  /*
   * Some systems have the bad habit of starting login shells with lots of open
   * file descriptors.  For instance, most systems that have picked up the
   * pre-4.0 Sun YP code leave a file descriptor open each time you call one
   * of the getpw* functions, and it's set to be open across execs.  That
   * means one for login, one for xterm, one for shelltool, etc.
   */
  if (login_shell && interactive_shell)
    {
      for (i = 3; i < 20; i++)
	close (i);
    }
#endif /* CLOSE_FDS_AT_LOGIN */

  /* From here on in, the shell must be a normal functioning shell.
     Variables from the environment are expected to be set, etc. */
  shell_initialize ();

  if (interactive_shell)
    {
      char *term = getenv ("TERM");
      if (term && (strcmp (term, "emacs") == 0))
	no_line_editing = 1;
    }

  top_level_arg_index = arg_index;

  if (!quiet && do_version)
    show_shell_version ();

  /* Give this shell a place to longjmp to before executing the
     startup files.  This allows users to press C-c to abort the
     lengthy startup. */
  {
    int code;

    code = setjmp (top_level);

    if (code)
      {
	if (code == EXITPROG)
	  goto exit_shell;
	else
	  locally_skip_execution++;
      }
  }

  arg_index = top_level_arg_index;

  /* Execute the start-up scripts. */

  if (!interactive_shell)
    {
      makunbound ("PS1", shell_variables);
      makunbound ("PS2", shell_variables);
      interactive = 0;
    }
  else
    {
      change_flag_char ('i', FLAG_ON);
      interactive = 1;
    }

  if (!locally_skip_execution)
    run_startup_files ();

#if defined (RESTRICTED_SHELL)
      /* I turn on the restrictions afterwards because it is explictly
	 stated in the POSIX spec that PATH cannot be set in a restricted
	 shell, except in .profile. */
    maybe_make_restricted (shell_name);
#endif /* RESTRICTED_SHELL */

    if (local_pending_command)
      {
	/* Bind remaining args to $1 ... $n */
	WORD_LIST *args = (WORD_LIST *)NULL;
	while (arg_index != argc)
	  args = make_word_list (make_word (argv[arg_index++]), args);
	if (args)
	  {
	    args = (WORD_LIST *)reverse_list (args);
	    if (posixly_correct)
	      {
		/* Posix.2 4.56.3 says that the first argument after
		   sh -c command becomes $0, and the rest of the arguments
		   are bound to $1 ... $N. */
		shell_name = savestring (args->word->word);
		dollar_vars[0] = savestring (shell_name);
		remember_args (args->next, 1);
	      }
	    else
	      remember_args (args, 1);
	    dispose_words (args);
	  }

	startup_state = 2;
#if defined (ONESHOT)
	run_one_command (local_pending_command);
	goto exit_shell;
#else /* ONESHOT */
	with_input_from_string (local_pending_command, "-c");
	goto read_and_execute;
#endif /* !ONESHOT */
      }

  /* Do the things that should be done only for interactive shells. */
  if (interactive_shell)
    {
      /* Set up for checking for presence of mail. */
#if defined (USG)
      /* Under System V, we can only tell if you have mail if the
	 modification date has changed.  So remember the current
	 modification dates. */
      remember_mail_dates ();
#else
      /* Under 4.x, you have mail if there is something in your inbox.
	 I set the remembered mail dates to 1900.  */
      reset_mail_files ();
#endif /* USG */

      /* If this was a login shell, then assume that /bin/login has already
	 taken care of informing the user that they have new mail.  Otherwise,
	 we want to check right away. */
      if (login_shell == 1)
	{
#if !defined (USG)
	  remember_mail_dates ();
#endif
	}

      reset_mail_timer ();

      change_flag_char ('i', FLAG_ON);

#if defined (HISTORY)
      /* Initialize the interactive history stuff. */
      if (!shell_initialized)
	load_history ();
#endif /* HISTORY */

      /* Initialize terminal state for interactive shells after the
	 .bash_profile and .bashrc are interpreted. */
      get_tty_state ();
    }

  /* Get possible input filename. */
  if ((arg_index != argc) && !read_from_stdin)
    {
      int fd;
      char *filename;

      free (dollar_vars[0]);
      dollar_vars[0] = savestring (argv[arg_index]);
      filename = savestring (argv[arg_index]);

      fd = open (filename, O_RDONLY);
      if ((fd < 0) && (errno == ENOENT))
	{
	  char *path_filename;
	  /* If it's not in the current directory, try looking through PATH
	     for it. */
	  path_filename = find_path_file (argv[arg_index]);
	  if (path_filename)
	    {
	      free (filename);
	      filename = path_filename;
	      fd = open (filename, O_RDONLY);
	    }
	}

      arg_index++;
      if (fd < 0)
	{
	  file_error (filename);
	  exit (1);
	}

      /* Only do this with file descriptors we can seek on. */
      if (lseek (fd, 0L, 1) != -1)
	{
	  unsigned char sample[80];
	  int sample_len;

	  /* Check to see if the `file' in `bash file' is a binary file
	     according to the same tests done by execute_simple_command (),
	     and report an error and exit if it is. */
	  sample_len = read (fd, sample, sizeof (sample));
	  if (sample_len > 0)
	    if (check_binary_file (sample, sample_len))
	      {
		report_error ("%s: cannot execute binary file", filename);
		exit (EX_BINARY_FILE);
	      }
	  /* Now rewind the file back to the beginning. */
	  lseek (fd, 0L, 0);
	}

#if defined (BUFFERED_INPUT)
      default_buffered_input = fd;
      if (default_buffered_input == -1)
	{
	  file_error (filename);
	  exit (127);
	}
      SET_CLOSE_ON_EXEC (default_buffered_input);

#else /* !BUFFERED_INPUT */

      /* Open the script.  But try to move the file descriptor to a randomly
	 large one, in the hopes that any descriptors used by the script will
	 not match with ours. */
      {
	int script_fd, nfds;

	nfds = getdtablesize ();
	if (nfds <= 0)
	  nfds = 20;
	if (nfds > 256)
	  nfds = 256;
	script_fd = dup2 (fd, nfds - 1);
	if (script_fd)
	  {
	    close (fd);
	    fd = script_fd;
	  }
      }

      default_input = fdopen (fd, "r");

      if (!default_input)
	{
	  file_error (filename);
	  exit (127);
	}

      SET_CLOSE_ON_EXEC (fd);
      if (fileno (default_input) != fd)
	SET_CLOSE_ON_EXEC (fileno (default_input));

#endif /* !BUFFERED_INPUT */

      if (!interactive_shell || (!isatty (fd)))
	{
#if defined (HISTORY)
	  history_expansion = 0;
	  remember_on_history = 0;
#endif /* HISTORY */
	  interactive = interactive_shell = 0;
	  no_line_editing = 1;
#if defined (JOB_CONTROL)
	  set_job_control (0);
#endif /* JOB_CONTROL */
	}
      else
	{
	  /* I don't believe that this code is ever executed, even in
	     the presence of /dev/fd. */
	  dup2 (fd, 0);
	  close (fd);
	  fclose (default_input);
	}
    }
  else if (!interactive)
    /* In this mode, bash is reading a script from stdin, which is a
       pipe or redirected file. */
#if defined (BUFFERED_INPUT)
    default_buffered_input = fileno (stdin);	/* == 0 */
#else      
    setbuf (default_input, (char *)NULL);
#endif /* !BUFFERED_INPUT */

  /* Bind remaining args to $1 ... $n */
  {
    WORD_LIST *args = (WORD_LIST *)NULL;
    while (arg_index != argc)
      args = make_word_list (make_word (argv[arg_index++]), args);
    args = (WORD_LIST *)reverse_list (args);
    remember_args (args, 1);
    dispose_words (args);
  }

#if defined (BUFFERED_INPUT)
  if (!interactive)
    unset_nodelay_mode (default_buffered_input);
  else
    unset_nodelay_mode (fileno (stdin));
#else
  unset_nodelay_mode (fileno (stdin));
#endif /* !BUFFERED_INPUT */

  /* with_input_from_stdin really means `with_input_from_readline' */
  if (interactive && !no_line_editing)
    with_input_from_stdin ();
  else
#if defined (BUFFERED_INPUT)
    {
      if (!interactive)
	with_input_from_buffered_stream (default_buffered_input, dollar_vars[0]);
      else
	with_input_from_stream (default_input, dollar_vars[0]);
    }
#else /* !BUFFERED_INPUT */
    with_input_from_stream (default_input, dollar_vars[0]);
#endif /* !BUFFERED_INPUT */

#if !defined (ONESHOT)
 read_and_execute:
#endif /* !ONESHOT */

  shell_initialized = 1;

  /* Read commands until exit condition. */
  reader_loop ();

  exit_shell:
  /* Do trap[0] if defined. */
  run_exit_trap ();

#if defined (PROCESS_SUBSTITUTION)
  unlink_fifo_list ();
#endif /* PROCESS_SUBSTITUTION */

#if defined (HISTORY)
  if (interactive && remember_on_history)
    maybe_save_shell_history ();
#endif /* HISTORY */

#if defined (JOB_CONTROL)
  /* If this shell is interactive, terminate all stopped jobs and
     restore the original terminal process group. */
  end_job_control ();
#endif /* JOB_CONTROL */

  /* Always return the exit status of the last command to our parent. */
  exit (last_command_exit_value);
}

#if !defined (SYS_PROFILE)
#  define SYS_PROFILE "/etc/profile"
#endif /* !SYS_PROFILE */

/* Source the bash startup files.  If POSIXLY_CORRECT is non-zero, we obey
   the Posix.2 startup file rules:  $ENV is expanded, and if the file it
   names exists, that file is sourced.  The Posix.2 rules are in effect
   for both interactive and non-interactive shells (section 4.56.5.3) */
static void
run_startup_files ()
{
  static int sourced_env = 0;

  if (!posixly_correct)
    {
      if (login_shell)
	{
	  /* We don't execute .bashrc for login shells. */
          no_rc++;
          maybe_execute_file (SYS_PROFILE);
        }

      if (login_shell && !no_profile)
        {
          if (act_like_sh)
            maybe_execute_file ("~/.profile");
          else
            {
              if (maybe_execute_file ("~/.bash_profile") == 0)
	        if (maybe_execute_file ("~/.bash_login") == 0)
	          maybe_execute_file ("~/.profile");
            }
        }

      /* Execute ~/.bashrc for most shells.  Never execute it if
         ACT_LIKE_SH is set, or if NO_RC is set.

         If the executable file "/usr/gnu/src/bash/foo" contains:

           #!/usr/gnu/bin/bash
           echo hello

         then:

         COMMAND	    EXECUTE BASHRC
         --------------------------------
         bash -c foo		NO
         bash foo		NO
         foo			NO
         rsh machine ls		YES (for rsh, which calls `bash -c')
         rsh machine foo	YES (for shell started by rsh) NO (for foo!)
         echo ls | bash		NO
         login			NO
         bash			YES
      */
#if defined (S_ISSOCK)
      if (!act_like_sh && !no_rc &&
          (interactive_shell || (issock (fileno (stdin)) &&
			         local_pending_command)))
#else /* !S_ISSOCK */
      if (!act_like_sh && !no_rc &&
          (interactive_shell || (!isatty (fileno (stdin)) &&
			         local_pending_command)))
#endif /* !S_ISSOCK */
        maybe_execute_file (bashrc_file);
    }

   /* Try a TMB suggestion.  If running a script, then execute the
      file mentioned in the ENV variable. */
   if (!privileged_mode && sourced_env++ == 0 &&
       (posixly_correct || !interactive_shell))
    {
      char *env_file = (char *)NULL;

      if (!posixly_correct)
	env_file = getenv ("BASH_ENV");
      if (!env_file)
	env_file = getenv ("ENV");

      if (env_file && *env_file)
	{
	  WORD_LIST *list;
	  char *expanded_file_name;

	  list = expand_string_unsplit (env_file, 1);
	  if (list)
	    {
	      expanded_file_name = string_list (list);
	      dispose_words (list);

	      if (expanded_file_name && *expanded_file_name)
		maybe_execute_file (expanded_file_name);

	      if (expanded_file_name)
		free (expanded_file_name);
	    }
	}
    }
}

#if defined (RESTRICTED_SHELL)
/* Perhaps make this shell a `restricted' one, based on NAME.
   If the basename of NAME is "rbash", then this shell is restricted.
   In a restricted shell, PATH and SHELL are read-only and non-unsettable.
   Do this also if `restricted' is already set to 1; maybe the shell was
   started with -r. */
maybe_make_restricted (name)
     char *name;
{
  char *temp;

  temp = base_pathname (shell_name);
  if (restricted || (strcmp (temp, "rbash") == 0))
    {
      set_var_read_only ("PATH");
      non_unsettable ("PATH");
      set_var_read_only ("SHELL");
      non_unsettable ("SHELL");
      restricted++;
    }
}
#endif /* RESTRICTED_SHELL */

#if defined (HISTORY)
/* If this is an interactive shell, then append the lines executed
   this session to the history file. */
int
maybe_save_shell_history ()
{
  int result = 0;

  if (interactive && history_lines_this_session)
    {
      void using_history ();
      char *hf = get_string_value ("HISTFILE");

      if (hf && *hf)
	{
	  struct stat buf;

	  /* If the file doesn't exist, then create it. */
	  if (stat (hf, &buf) == -1)
	    {
	      int file = open (hf, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	      if (file != -1)
		close (file);
	    }

	  /* Now actually append the lines if the history hasn't been
	     stifled. */
	  using_history ();
	  if (history_lines_this_session <= where_history ())
	    {
	      result = append_history (history_lines_this_session, hf);
	      history_lines_in_file += history_lines_this_session;
	      history_lines_this_session = 0;
	    }
	}
    }
  return (result);
}

/* Load the history list from the history file. */
load_history ()
{
  char *hf;

  /* Truncate history file for interactive shells which desire it.
     Note that the history file is automatically truncated to the
     size of HISTSIZE if the user does not explicitly set the size
     differently. */
  set_if_not ("HISTFILESIZE", get_string_value ("HISTSIZE"));
  stupidly_hack_special_variables ("HISTFILESIZE");

  /* Read the history in HISTFILE into the history list. */
  hf = get_string_value ("HISTFILE");

  if (hf && *hf)
    {
      struct stat buf;

      if (stat (hf, &buf) == 0)
	{
	  read_history (hf);
	  using_history ();
	  history_lines_in_file = where_history ();
	}
    }
}

/* Write the existing history out to the history file. */
save_history ()
{
  char *hf = get_string_value ("HISTFILE");

  if (hf && *hf)
    {
      struct stat buf;

      if (stat (hf, &buf) == 0)
	{
	  /* Append only the lines that occurred this session to
	     the history file. */
	  using_history ();

	  if (history_lines_this_session < where_history ())
	    append_history (history_lines_this_session, hf);
	  else
	    write_history (hf);
	}
    }
}
#endif /* HISTORY */

/* Try to execute the contents of FNAME.  If FNAME doesn't exist,
   that is not an error, but other kinds of errors are.  Returns
   -1 in the case of an error, 0 in the case that the file was not
   found, and 1 if the file was found and executed. */
maybe_execute_file (fname)
     char *fname;
{
  extern int return_catch_flag;
  extern jmp_buf return_catch;
  jmp_buf old_return_catch;
  int return_val, fd, tresult;
  char *filename, *string;
  struct stat file_info;

  filename = tilde_expand (fname);
  fd = open (filename, O_RDONLY);

  if (fd < 0)
    {
file_error_and_exit:
      if (errno != ENOENT)
	file_error (filename);
      free (filename);
      return ((errno == ENOENT) ? 0 : -1);
    }

  if (fstat (fd, &file_info) == -1)
    goto file_error_and_exit;

  string = (char *)xmalloc (1 + (int)file_info.st_size);
  tresult = read (fd, string, file_info.st_size);

  {
    int tt = errno;
    close (fd);
    errno = tt;
  }

  if (tresult != file_info.st_size)
    {
      free (string);
      goto file_error_and_exit;
    }
  string[file_info.st_size] = '\0';

  return_catch_flag++;
  bcopy ((char *)return_catch, (char *)old_return_catch, sizeof (jmp_buf));

  return_val = setjmp (return_catch);

  /* If `return' was seen outside of a function, but in the script, then
     force parse_and_execute () to clean up. */
  if (return_val)
    parse_and_execute_cleanup ();
  else
    tresult = parse_and_execute (string, filename);

  return_catch_flag--;
  bcopy ((char *)old_return_catch, (char *)return_catch, sizeof (jmp_buf));

  free (filename);

  return (1);
}

#if defined (ONESHOT)
/* Run one command, given as the argument to the -c option.  Tell
   parse_and_execute not to fork for a simple command. */
run_one_command (command)
     char *command;
{
  sighandler sigint_sighandler ();
  int code;

  code = setjmp (top_level);

  if (code != NOT_JUMPED)
    {
#if defined (PROCESS_SUBSTITUTION)
      unlink_fifo_list ();
#endif /* PROCESS_SUBSTITUTION */
      switch (code)
	{
	  /* Some kind of throw to top_level has occured. */
	case FORCE_EOF:
	case EXITPROG:
	case DISCARD:
	  return 0;
	default:
	  programming_error ("Bad jump %d", code);
	}
    }
   return (parse_and_execute (savestring (command), "-c"));
}
#endif /* ONESHOT */

reader_loop ()
{
  int our_indirection_level;
  COMMAND *current_command = (COMMAND *)NULL;

  our_indirection_level = ++indirection_level;

  while (!EOF_Reached)
    {
      extern char *trap_list[];
      sighandler sigint_sighandler ();
      int code;

      code = setjmp (top_level);

#if defined (PROCESS_SUBSTITUTION)
      unlink_fifo_list ();
#endif /* PROCESS_SUBSTITUTION */

      if (interactive_shell)
	set_signal_handler (SIGINT, sigint_sighandler);

      if (code != NOT_JUMPED)
	{
	  indirection_level = our_indirection_level;

	  switch (code)
	    {
	      /* Some kind of throw to top_level has occured. */
	    case FORCE_EOF:
	    case EXITPROG:
	      current_command = (COMMAND *)NULL;
	      EOF_Reached = EOF;
	      goto exec_done;

	    case DISCARD:
	      /* Obstack free command elements, etc. */
	      if (current_command)
		{
		  dispose_command (current_command);
		  current_command = (COMMAND *)NULL;
		}
	      break;

	    default:
	      programming_error ("Bad jump %d", code);
	    }
	}

      executing = 0;
      dispose_used_env_vars ();

#if (defined (Ultrix) && defined (mips)) || !defined (HAVE_ALLOCA)
      /* Attempt to reclaim memory allocated with alloca (). */
      (void) alloca (0);
#endif

      if (read_command () == 0)
	{
	  if (global_command)
	    {
	      current_command = global_command;

	      current_command_number++;

	      /* POSIX spec: "-n: The shell reads commands but does
		 not execute them; this can be used to check for shell
		 script syntax errors.  The shell ignores the -n option
		 for interactive shells. " */
	      if (interactive_shell || !read_but_dont_execute)
		{
		  executing = 1;
		  execute_command (current_command);
		}

	    exec_done:
	      if (current_command)
	        {
		  dispose_command (current_command);
		  current_command = (COMMAND *)NULL;
	        }
	      QUIT;
	    }
	}
      else
	{
	  /* Parse error, maybe discard rest of stream if not interactive. */
	  if (!interactive)
	    EOF_Reached = EOF;
	}
      if (just_one_command)
	EOF_Reached = EOF;
    }
  indirection_level--;
}

/* Return a string denoting what our indirection level is. */
static char indirection_string[100];

char *
indirection_level_string ()
{
  register int i, j;
  char *ps4 = get_string_value ("PS4");

  if (!ps4)
    ps4 = savestring ("+ ");
  else
    ps4 = decode_prompt_string (ps4);

  for (i = 0; i < indirection_level && i < 99; i++)
    indirection_string[i] = *ps4;

  for (j = 1; ps4[j] && i < 99; i++, j++)
    indirection_string[i] = ps4[j];

  indirection_string[i] = '\0';
  free (ps4);
  return (indirection_string);
}

static sighandler 
alrm_catcher(i)
     int i;
{
  printf ("%ctimed out waiting for input: auto-logout\n", '\07');
  longjmp (top_level, EXITPROG);
#if !defined (VOID_SIGHANDLER)
  return (0);
#endif /* !VOID_SIGHANDLER */
}

parse_command ()
{
  extern int need_here_doc, current_command_line_count;
  extern REDIRECT *redirection_needing_here_doc;
  int r;

  need_here_doc = 0;
  redirection_needing_here_doc = (REDIRECT *)NULL;

  run_pending_traps ();

  current_command_line_count = 0;
  r = yyparse ();

  if (need_here_doc)
    make_here_document (redirection_needing_here_doc);
  need_here_doc = 0;

  return (r);
}

read_command ()
{
  extern char *ps1_prompt, **prompt_string_pointer;
  extern int current_command_line_count;
  SHELL_VAR *tmout_var = (SHELL_VAR *)NULL;
  int tmout_len = 0, result;
  SigHandler *old_alrm = (SigHandler *)NULL;

  prompt_string_pointer = &ps1_prompt;
  global_command = (COMMAND *)NULL;

  /* Only do timeouts if interactive. */
  if (interactive)
    {
      tmout_var = find_variable ("TMOUT");

      if (tmout_var && tmout_var->value)
	{
	  tmout_len = atoi (tmout_var->value);
	  if (tmout_len > 0)
	    {
	      old_alrm = set_signal_handler (SIGALRM, alrm_catcher);
	      alarm (tmout_len);
	    }
	}
    }

  QUIT;

  current_command_line_count = 0;
  result = parse_command ();

  if (interactive && tmout_var && (tmout_len > 0))
    {
      alarm(0);
      set_signal_handler (SIGALRM, old_alrm);
    }
  return (result);
}

/* Cause STREAM to buffer lines as opposed to characters or blocks. */
static void
line_buffer_stream (stream)
     FILE *stream;
{
  /* If your machine doesn't have either of setlinebuf or setvbuf,
     you can just comment out the buffering commands, and the shell
     will still work.  It will take more cycles, though. */
#if defined (HAVE_SETLINEBUF)
  setlinebuf (stream);
#else
#  if defined (_IOLBF)
#    if defined (REVERSED_SETVBUF_ARGS)
  setvbuf (stream, _IOLBF, (char *)NULL, BUFSIZ);
#    else /* !REVERSED_SETVBUF_ARGS */
  setvbuf (stream, (char *)NULL, _IOLBF, BUFSIZ);
#    endif /* !REVERSED_SETVBUF_ARGS */
#  endif /* _IOLBF */
#endif /* !HAVE_SETLINEBUF */
}

/* Do whatever is necessary to initialize the shell.
   Put new initializations in here. */
shell_initialize ()
{
  /* Line buffer output for stderr and stdout. */
  line_buffer_stream (stderr);
  line_buffer_stream (stdout);

  /* Sort the array of shell builtins so that the binary search in
     find_shell_builtin () works correctly. */
  initialize_shell_builtins ();

  /* Initialize the trap signal handlers before installing our own
     signal handlers.  traps.c:restore_original_signals () is responsible
     for restoring the original default signal handlers.  That function
     is called from jobs.c when we make a new child. */
  initialize_traps ();
  initialize_signals ();

  /* Initialize current_user.name and current_host_name. */
  {
    struct passwd *entry = getpwuid (current_user.uid);
    char hostname[256];

    if (gethostname (hostname, 255) < 0)
      current_host_name = "??host??";
    else
      current_host_name = savestring (hostname);

    if (entry)
      {
	current_user.user_name = savestring (entry->pw_name);
	if (entry->pw_shell && entry->pw_shell[0])
	  current_user.shell = savestring (entry->pw_shell);
	else
	  current_user.shell = savestring ("/bin/sh");
	current_user.home_dir = savestring (entry->pw_dir);
      }
    else
      {
	current_user.user_name = savestring ("I have no name!");
	current_user.shell = savestring ("/bin/sh");
	current_user.home_dir = savestring ("/");
      }

    endpwent ();
  }

  /* Initialize our interface to the tilde expander. */
  tilde_initialize ();

  /* Initialize internal and environment variables. */
  initialize_shell_variables (shell_environment);

  /* Initialize filename hash tables. */
  initialize_filename_hashing ();

  /* Initialize the data structures for storing and running jobs. */
  initialize_jobs ();

  /* Initialize input streams to null. */
  initialize_bash_input ();
}

/* Function called by main () when it appears that the shell has already
   had some initialization preformed.  This is supposed to reset the world
   back to a pristine state, as if we had been exec'ed. */
shell_reinitialize ()
{
  extern int line_number, last_command_exit_value;

  /* The default shell prompts. */
  primary_prompt = PPROMPT;
  secondary_prompt = SPROMPT;

  /* Things that get 1. */
  current_command_number = 1;

  /* We have decided that the ~/.bashrc file should not be executed
     for the invocation of each shell script.  Perhaps some other file
     should.  */
  act_like_sh = 1;

  /* Things that get 0. */
  login_shell = make_login_shell = interactive = executing = 0;
  debugging = no_rc = no_profile = do_version = line_number = 0;
  forced_interactive = interactive_shell = subshell_environment = 0;
  last_command_exit_value = 0;

#if defined (HISTORY)
  remember_on_history = history_expansion = 0;
#endif /* HISTORY */

#if defined (RESTRICTED_SHELL)
  restricted = 0;
#endif /* RESTRICTED_SHELL */

  /* Ensure that the default startup file is used.  (Except that we don't
     execute this file for reinitialized shells). */
  bashrc_file = "~/.bashrc";

  /* Delete all variables and functions.  They will be reinitialized when
     the environment is parsed. */

  delete_all_variables (shell_variables);
  delete_all_variables (shell_functions);

  /* Pretend the PATH variable has changed. */
  sv_path ("PATH");
}

initialize_signals ()
{
  initialize_terminating_signals ();
  initialize_job_signals ();
#if defined (INITIALIZE_SIGLIST)
  initialize_siglist ();
#endif
}

/* A structure describing a signal that terminates the shell if not
   caught.  The orig_handler member is present so children can reset
   these signals back to their original handlers. */
struct termsig {
     int signum;
     SigHandler *orig_handler;
};

#define NULL_HANDLER (SigHandler *)SIG_DFL

/* The list of signals that would terminate the shell if not caught.
   We catch them, but just so that we can write the history file,
   and so forth. */
static struct termsig terminating_signals[] = {
#ifdef SIGHUP
  SIGHUP, NULL_HANDLER,
#endif

#ifdef SIGINT
  SIGINT, NULL_HANDLER,
#endif

#ifdef SIGILL
  SIGILL, NULL_HANDLER,
#endif

#ifdef SIGTRAP
  SIGTRAP, NULL_HANDLER,
#endif

#ifdef SIGIOT
  SIGIOT, NULL_HANDLER,
#endif

#ifdef SIGDANGER
  SIGDANGER, NULL_HANDLER,
#endif

#ifdef SIGEMT
  SIGEMT, NULL_HANDLER,
#endif

#ifdef SIGFPE
  SIGFPE, NULL_HANDLER,
#endif

#ifdef SIGBUS
  SIGBUS, NULL_HANDLER,
#endif

#ifdef SIGSEGV
  SIGSEGV, NULL_HANDLER,
#endif

#ifdef SIGSYS
  SIGSYS, NULL_HANDLER,
#endif

#ifdef SIGPIPE
  SIGPIPE, NULL_HANDLER,
#endif

#ifdef SIGALRM
  SIGALRM, NULL_HANDLER,
#endif

#ifdef SIGTERM
  SIGTERM, NULL_HANDLER,
#endif

#ifdef SIGXCPU
  SIGXCPU, NULL_HANDLER,
#endif

#ifdef SIGXFSZ
  SIGXFSZ, NULL_HANDLER,
#endif

#ifdef SIGVTALRM
  SIGVTALRM, NULL_HANDLER,
#endif

#ifdef SIGPROF
  SIGPROF, NULL_HANDLER,
#endif

#ifdef SIGLOST
  SIGLOST, NULL_HANDLER,
#endif

#ifdef SIGUSR1
  SIGUSR1, NULL_HANDLER,
#endif

#ifdef SIGUSR2
  SIGUSR2, NULL_HANDLER,
#endif
};

#define TERMSIGS_LENGTH (sizeof (terminating_signals) / sizeof (struct termsig))

/* This function belongs here? */
sighandler
termination_unwind_protect (sig)
     int sig;
{
  if (sig == SIGINT && signal_is_trapped (SIGINT))
    run_interrupt_trap ();

#if defined (HISTORY)
  if (interactive && remember_on_history)
    maybe_save_shell_history ();
#endif /* HISTORY */

#if defined (JOB_CONTROL)
  if (interactive && login_shell && sig == SIGHUP)
    {
      extern void hangup_all_jobs ();

      hangup_all_jobs ();
    }
  end_job_control ();
#endif /* JOB_CONTROL */

#if defined (PROCESS_SUBSTITUTION)
  unlink_fifo_list ();
#endif /* PROCESS_SUBSTITUTION */

  run_exit_trap ();
  signal (sig, SIG_DFL);
  kill (getpid (), sig);

#if !defined (VOID_SIGHANDLER)
  return (0);
#endif /* VOID_SIGHANDLER */
}

/* Initialize signals that will terminate the shell to do some
   unwind protection. */
initialize_terminating_signals ()
{
  register int i;

  /* The following code is to avoid an expensive call to
     set_signal_handler () for each terminating_signals.  Fortunately,
     this is possible in Posix.  Unfortunately, we have to call signal ()
     on non-Posix systems for each signal in terminating_signals. */
#if defined (_POSIX_VERSION)
  struct sigaction act, oact;

  act.sa_handler = termination_unwind_protect;
  act.sa_flags = 0;
  sigemptyset (&act.sa_mask);
  for (i = 0; i < TERMSIGS_LENGTH; i++)
    sigaddset (&act.sa_mask, terminating_signals[i].signum);
  for (i = 0; i < TERMSIGS_LENGTH; i++)
    {
      sigaction (terminating_signals[i].signum, &act, &oact);
      terminating_signals[i].orig_handler = oact.sa_handler;
    }

#else /* !_POSIX_VERSION */

  for (i = 0; i < TERMSIGS_LENGTH; i++)
    terminating_signals[i].orig_handler =
      signal (terminating_signals[i].signum, termination_unwind_protect);

#endif /* !_POSIX_VERSION */

#if defined (JOB_CONTROL) || defined (_POSIX_VERSION)
  /* All shells use the signal mask they inherit, and pass it along
     to child processes.  Children will never block SIGCHLD, though. */
  sigemptyset (&top_level_mask);
  sigprocmask (SIG_BLOCK, (sigset_t *)NULL, &top_level_mask);
  sigdelset (&top_level_mask, SIGCHLD);
#endif /* JOB_CONTROL || _POSIX_VERSION */

  /* And, some signals that are specifically ignored by the shell. */
  signal (SIGQUIT, SIG_IGN);

  if (interactive)
    {
      extern sighandler sigint_sighandler ();

      set_signal_handler (SIGINT, sigint_sighandler);
      signal (SIGTERM, SIG_IGN);
    }
}

void
reset_terminating_signals ()
{
  register int i;

#if defined (__POSIX_VERSION)
  struct sigaction act;

  act.sa_flags = 0;
  sigemptyset (&act.sa_mask);
  for (i = 0; i < TERMSIGS_LENGTH; i++)
    {
      /* Skip a signal if it's trapped or handled specially, because the
	 trap code will restore the correct value. */
      if (signal_is_trapped (i) || signal_is_special (i))
	continue;

      act.sa_handler = terminating_signals[i].orig_handler;
      sigaction (terminating_signals[i].signum, &act, (struct sigaction *) NULL);
    }
#else
  for (i = 0; i < TERMSIGS_LENGTH; i++)
    {
      if (signal_is_trapped (i) || signal_is_special (i))
	continue;

      signal (terminating_signals[i].signum,
	      terminating_signals[i].orig_handler);
    }
#endif
}
  
/* What to do when we've been interrupted, and it is safe to handle it. */
void
throw_to_top_level ()
{
  extern int last_command_exit_value, loop_level, continuing, breaking;
  extern int return_catch_flag;
  extern int parse_and_execute_level;
  int print_newline = 0;

  if (interrupt_state)
    {
      print_newline = 1;
      interrupt_state--;
    }

  if (interrupt_state)
    return;

  last_command_exit_value |= 128;

  /* Run any traps set on SIGINT. */
  run_interrupt_trap ();

  /* Cleanup string parser environment. */
  while (parse_and_execute_level)
    parse_and_execute_cleanup ();

#if defined (JOB_CONTROL)
  give_terminal_to (shell_pgrp);
#endif /* JOB_CONTROL */

#if defined (JOB_CONTROL) || defined (_POSIX_VERSION)
  sigprocmask (SIG_SETMASK, &top_level_mask, (sigset_t *)NULL);
#endif

  reset_parser ();

#if defined (READLINE)
  if (interactive)
    bashline_reinitialize ();
#endif /* READLINE */

#if defined (PROCESS_SUBSTITUTION)
  unlink_fifo_list ();
#endif /* PROCESS_SUBSTITUTION */

  run_unwind_protects ();
  loop_level = continuing = breaking = 0;
  return_catch_flag = 0;

  if (interactive && print_newline)
    {
      fflush (stdout);
      fprintf (stderr, "\n");
      fflush (stderr);
    }

  /* An interrupted `wait' command in a script does not exit the script. */
  if (interactive || (print_newline && signal_is_trapped (SIGINT)))
    longjmp (top_level, DISCARD);
  else
    longjmp (top_level, EXITPROG);
}

/* When non-zero, we throw_to_top_level (). */
int interrupt_immediately = 0;

/* What we really do when SIGINT occurs. */
sighandler
sigint_sighandler (sig)
     int sig;
{
#if defined (USG) && !defined (_POSIX_VERSION)
  signal (sig, sigint_sighandler);
#endif

  /* interrupt_state needs to be set for the stack of interrupts to work
     right.  Should it be set unconditionally? */
  if (!interrupt_state)
    interrupt_state++;

  if (interrupt_immediately)
    {
      interrupt_immediately = 0;
      throw_to_top_level ();
    }
#if !defined (VOID_SIGHANDLER)
  return (0);
#endif /* VOID_SIGHANDLER */
}

#if defined (MAKE_BUG_REPORTS)
/* Make a bug report, even to the extent of mailing it.  Hope that it
   gets where it is supposed to go.  If not, maybe the user will send
   it back to me. */
#if defined (HISTORY)
#  include <readline/history.h>

  /* Number of commands to place in the bug report. */
#  define LAST_INTERESTING_HISTORY_COUNT 6
#endif /* HISTORY */

#if defined (HAVE_VFPRINTF)
make_bug_report (va_alist)
     va_dcl
#else
make_bug_report (reason, arg1, arg2)
     char *reason;
#endif /* HAVE_VFPRINTF */
{
  extern char *current_host_name, *the_current_maintainer;
  extern int interactive, login_shell;
  FILE *stream, *popen ();
#if defined (HISTORY)
  register int len = where_history ();
  register int i = len - LAST_INTERESTING_HISTORY_COUNT;
  HIST_ENTRY **list = history_list ();
#endif /* HISTORY */

#if defined (HAVE_VFPRINTF)
  char *reason;
  va_list args;
#endif /* HAVE_VFPRINTF */

  stream = popen ("/bin/rmail bash-maintainers@ai.mit.edu", "w");

#if defined (HISTORY)
  save_history ();
  if (i < 0) i = 0;
#endif /* HISTORY */

  if (stream)
    {
      fprintf (stream, "To: bash-maintainers@ai.mit.edu\n");
      fprintf (stream, "Subject: Bash-%s.%d bug-report: ",
	       dist_version, build_version);

#if defined (HAVE_VFPRINTF)
      va_start (args);
      reason = va_arg (args, char *);
      vfprintf (stream, reason, args);
      va_end (args);
#else
      fprintf (stream, reason, arg1, arg2);
#endif /* HAVE_VFPRINTF */

      fprintf (stream, "\n");

      /* Write the history into the mail file.  Maybe we can recreate
	 the bug? */
      fprintf (stream,
	       "This is a Bash bug report.  Bash maintainers should be getting this report.\n\
If this mail has bounced, for right now please send it to:\n\
\n\
	%s\n\
\n\
since he is the current maintainer of this version of the shell.\n\
\n\
This is %s (invoked as `%s'), version %s.%d, on host %s, used by %s.\n\
This shell is %sinteractive, and it is %sa login shell.\n\
\n\
The host is a %s running %s.\n\
\n\
The current environment is:\n",
	       the_current_maintainer,
	       get_string_value ("BASH"), full_pathname (dollar_vars[0]),
	       dist_version, build_version, current_host_name,
	       current_user.user_name, interactive ? "" : "not ",
	       login_shell ? "" : "not ", SYSTEM_NAME, OS_NAME);

      {
	SHELL_VAR **vlist, *var;
	register int i;

	vlist = all_shell_variables ();

	for (i = 0; vlist && var = vlist[i]; i++)
	  {
	    if (!invisible_p (var) && exported_p (var))
	      {
		fprintf (stream, "%s=%s", var->name, value_cell (var));
		fprintf (stream, "\n");
	      }
	  }
      }

#if defined (HISTORY)
      fprintf (stream, "\nAnd here are the last %d commands.\n\n",
	       LAST_INTERESTING_HISTORY_COUNT);

      for (; i < len; i++)
	fprintf (stream, "%s\n", list[i]->line);
#endif /* HISTORY */

      pclose (stream);
    }
  else
    {
      fprintf (stderr, "Can't mail bug report!\n");
    }
}
#endif /* MAKE_BUG_REPORTS */

/* Give version information about this shell. */
show_shell_version ()
{
  extern char *shell_name;
  extern int version;

  printf ("GNU %s, version %s.%d-CWRU\n", base_pathname (shell_name),
	  dist_version, build_version);
}

/* Is FD a socket? */
static int
issock (fd)
     int fd;
{
#if defined (S_ISSOCK)
  struct stat sb;

  if (fstat(fd, &sb) < 0)
    return 0;
  return (S_ISSOCK (sb.st_mode) != 0);
#else
  return (0);
#endif
}
