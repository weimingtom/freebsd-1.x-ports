/* sccsdata.c - sccs data file for ups generated by mksccs */

char ups_sccsdata_c_sccsid[] = "@(#)sccsdata.c	2.45 19/9/92";

#include "sccsdata.h"

#ifndef __STDC__
#define const
#endif

/*  Changes since the last version:
 *
 *  Fix use-after-free bug that caused all source editing to fail - grrr.
 *  
 *  Link against ukcprog version that returns storage aligned for doubles.
 */

 const char *_ups_sccsdata[] = {
	"ups version 2.45 (build date: Sat Sep 19 14:45:26 BST 1992)",
	"@{$ups$}: sccsdata.c 2.45 ",
	"@{$ups$}: as.h 1.4 ",
	"@{$ups$}: breakpoint.h 1.4 ",
	"@{$ups$}: ci.h 1.14 ",
	"@{$ups$}: ci_compile.h 1.7 ",
	"@{$ups$}: ci_compile_expr.h 1.3 ",
	"@{$ups$}: ci_constexpr.h 1.3 ",
	"@{$ups$}: ci_decl.h 1.4 ",
	"@{$ups$}: ci_expr.h 1.6 ",
	"@{$ups$}: ci_func.h 1.2 ",
	"@{$ups$}: ci_init.h 1.2 ",
	"@{$ups$}: ci_lex.h 1.3 ",
	"@{$ups$}: ci_machine.h 1.12 ",
	"@{$ups$}: ci_parse.h 1.7 ",
	"@{$ups$}: ci_showexpr.h 1.2 ",
	"@{$ups$}: ci_stm.h 1.2 ",
	"@{$ups$}: ci_tokens.h 1.2 ",
	"@{$ups$}: ci_types.h 1.3 ",
	"@{$ups$}: ci_util.h 1.6 ",
	"@{$ups$}: core.h 1.5 ",
	"@{$ups$}: cursors.h 1.5 ",
	"@{$ups$}: cx_builtins.h 1.2 ",
	"@{$ups$}: data.h 1.2 ",
	"@{$ups$}: debug.h 1.5 ",
	"@{$ups$}: exec.h 1.9 ",
	"@{$ups$}: expr.h 1.4 ",
	"@{$ups$}: menudata.h 1.2 ",
	"@{$ups$}: mreg.h 1.12 ",
	"@{$ups$}: obj_bpt.h 1.8 ",
	"@{$ups$}: obj_buildf.h 1.8 ",
	"@{$ups$}: obj_env.h 1.4 ",
	"@{$ups$}: obj_misc.h 1.7 ",
	"@{$ups$}: obj_signal.h 1.8 ",
	"@{$ups$}: obj_stack.h 1.6 ",
	"@{$ups$}: obj_target.h 1.7 ",
	"@{$ups$}: objtypes.h 1.9 ",
	"@{$ups$}: output.h 1.3 ",
	"@{$ups$}: preamble.h 1.7 ",
	"@{$ups$}: printf.h 1.3 ",
	"@{$ups$}: proc.h 1.14 ",
	"@{$ups$}: reg.h 1.9 ",
	"@{$ups$}: sccsdata.h 1.1 ",
	"@{$ups$}: src.h 1.7 ",
	"@{$ups$}: st_priv.h 1.16 ",
	"@{$ups$}: stack.h 1.8 ",
	"@{$ups$}: state.h 1.7 ",
	"@{$ups$}: symtab.h 1.19 ",
	"@{$ups$}: tdr.h 1.3 ",
	"@{$ups$}: text.h 1.5 ",
	"@{$ups$}: textwin.h 1.6 ",
	"@{$ups$}: trun.h 1.7 ",
	"@{$ups$}: ui.h 1.18 ",
	"@{$ups$}: ui_priv.h 1.11 ",
	"@{$ups$}: ups.h 1.6 ",
	"@{$ups$}: va.h 1.7 ",
	"@{$ups$}: va_priv.h 1.8 ",
	"@{$ups$}: mips_frame.h 1.2 ",
	"@{$ups$}: ui_iploop.c 1.31 ",
	"@{$ups$}: ui_layout.c 1.25 ",
	"@{$ups$}: ui_menu.c 1.24 ",
	"@{$ups$}: ui_misc.c 1.11 ",
	"@{$ups$}: ui_src.c 1.30 ",
	"@{$ups$}: st_lookup.c 1.2 ",
	"@{$ups$}: st_stab.c 1.31 ",
	"@{$ups$}: st_shlib.c 1.18 ",
	"@{$ups$}: st_fmap.c 1.17 ",
	"@{$ups$}: st_parse.c 1.32 ",
	"@{$ups$}: st_read.c 1.25 ",
	"@{$ups$}: st_skim.c 1.39 ",
	"@{$ups$}: st_cb.c 1.19 ",
	"@{$ups$}: st_te.c 1.15 ",
	"@{$ups$}: va_decl.c 1.22 ",
	"@{$ups$}: va_edit.c 1.19 ",
	"@{$ups$}: va_menu.c 1.24 ",
	"@{$ups$}: va_type.c 1.17 ",
	"@{$ups$}: va_val.c 1.29 ",
	"@{$ups$}: va_expr.c 1.15 ",
	"@{$ups$}: obj_bpt.c 1.31 ",
	"@{$ups$}: obj_signal.c 1.17 ",
	"@{$ups$}: obj_misc.c 1.20 ",
	"@{$ups$}: obj_buildf.c 1.23 ",
	"@{$ups$}: obj_target.c 1.19 ",
	"@{$ups$}: obj_env.c 1.9 ",
	"@{$ups$}: obj_stack.c 1.12 ",
	"@{$ups$}: state.c 1.9 ",
	"@{$ups$}: reg.c 1.14 ",
	"@{$ups$}: cursors.c 1.13 ",
	"@{$ups$}: menudata.c 1.15 ",
	"@{$ups$}: tdr.c 1.6 ",
	"@{$ups$}: bp.c 1.15 ",
	"@{$ups$}: text.c 1.21 ",
	"@{$ups$}: core.c 1.23 ",
	"@{$ups$}: proc.c 1.34 ",
	"@{$ups$}: exec.c 1.34 ",
	"@{$ups$}: trun_ss.c 1.17 ",
	"@{$ups$}: trun_j.c 1.25 ",
	"@{$ups$}: stack.c 1.32 ",
	"@{$ups$}: data.c 1.8 ",
	"@{$ups$}: expr.c 1.31 ",
	"@{$ups$}: src.c 1.27 ",
	"@{$ups$}: output.c 1.12 ",
	"@{$ups$}: printf.c 1.17 ",
	"@{$ups$}: textwin.c 1.16 ",
	"@{$ups$}: ups.c 1.42 ",
	"@{$ups$}: as_mi.c 1.18 ",
	"@{$ups$}: as_vax.c 1.17 ",
	"@{$ups$}: as_68020.c 1.16 ",
	"@{$ups$}: as_mips.c 1.8 ",
	"@{$ups$}: as_clipper.c 1.9 ",
	"@{$ups$}: as_sparc.c 1.11 ",
	"@{$ups$}: ci_types.c 1.13 ",
	"@{$ups$}: ci_lex.c 1.16 ",
	"@{$ups$}: ci_stm.c 1.12 ",
	"@{$ups$}: ci_decl.c 1.19 ",
	"@{$ups$}: ci_func.c 1.15 ",
	"@{$ups$}: ci_util.c 1.18 ",
	"@{$ups$}: ci_expr.c 1.18 ",
	"@{$ups$}: ci_showexpr.c 1.12 ",
	"@{$ups$}: ci_constexpr.c 1.14 ",
	"@{$ups$}: ci_compile.c 1.17 ",
	"@{$ups$}: ci_compile_expr.c 1.21 ",
	"@{$ups$}: ci_opcodes.c 1.18 ",
	"@{$ups$}: ci_execute.c 1.20 ",
	"@{$ups$}: ci_init.c 1.14 ",
	"@{$ups$}: cx.c 1.13 ",
	"@{$ups$}: cx_builtins.c 1.19 ",
	"@{$ups$}: ci_parse.y 1.12 ",
	"@{$ups$}: doc/ups.man 1.11 ",
	"@{$ups$}: doc/porting.ms 1.1 ",
	"@{$ups$}: Makefile 1.41 ",
};