/*
 * Copyright (c) 1992,1993,1994 Hellmuth Michaelis, Brian Dunford-Shore
 *                              and Joerg Wunsch.
 *
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz and Don Ahn.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Hellmuth Michaelis,
 *	Brian Dunford-Shore and Joerg Wunsch.
 * 4. The name authors may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @(#)pcvt_out.c, 3.00, Last Edit-Date: [Thu Feb 17 08:57:11 1994]
 *
 */

/*---------------------------------------------------------------------------*
 *
 *	pcvt_out.c	VT220 Terminal Emulator
 *	---------------------------------------
 *	-hm	fixes for monochrome environments
 *	-jw	include rather primitive X stuff
 *	-hm	debugging netbsd-current
 *	-hm	converting to memory mapped virtual screens
 *	-hm	132 columns mode operation
 *	-jw	SCROLL_SLEEP patch
 *	-hm	cursor on/off implemented (DECTCEM)
 *	-hm	added Bruce Evans' set color escape sequence
 *	-hm	keyboard repeat (DECARM) activated
 *	-hm	just malloc as much memory as is really needed
 *	-hm	keyboard initialization for ddb activists ...
 *	-hm	add a volatile to color detection code for netbsd 0.9
 *	-hm	trident cursor start setting - finally ....
 *	-jw	USL VT compatibility
 *	-jw	patches to set winsize struct for VT/HP and real pixelsizes
 *	-hm	made SCROLL_SLEEP patch working
 *	-hm	HP emulation debugging
 *	-jw/hm	all ifdef's converted to if's 
 *	-hm	patch from Joerg: PCVT_INHIBIT_NUMLOCK and support for LCD's
 *	-hm	patch from Michael Havemester, bcopyb -> bcopy in check_scroll
 *	-hm	protected bcopy in check_scroll from being interrupted
 *
 *---------------------------------------------------------------------------*/

#include "vt.h"
#if NVT > 0

#include "pcvt_hdr.h"		/* global include */

static void check_scroll ( struct video_state *svsp );
static void hp_entry ( U_char ch, struct video_state *svsp );
static void update_hp ( struct video_state *svsp );
static void vt_coldinit ( void );
static void wrfkl ( int num, u_char *string, struct video_state *svsp );
static void writefkl ( int num, u_char *string, struct video_state *svsp );
static void write_char ( struct video_state *svsp, U_char kernel, U_char ch );

/*---------------------------------------------------------------------------*
 *	emulator main entry
 *---------------------------------------------------------------------------*/
void
sput(U_char ch, U_char attrib, int page)
{
	register struct video_state *svsp;

	if(page >= PCVT_NSCREENS)
		page = 0;
	
	svsp = &vs[page];

	if(do_initialization)
		vt_coldinit();

	if(svsp->sevenbit)
		ch &= 0x7f;

		

	if(svsp == vsp)			/* on current displayed page ?	*/
	{
		cursor_pos_valid = 0;		/* do not update cursor */
#if PCVT_SCREENSAVER
		if(scrnsv_active)		/* screen blanked ?	*/
			pcvt_scrnsv_reset();	/* unblank NOW !	*/
		else
			reset_screen_saver = 1;	/* do it asynchronously	*/
#endif /* PCVT_SCREENSAVER */
	}


	if((ch <= 0x1f) && (svsp->transparent == 0))
	{
	
	/* always process control-chars in the range 0x00..0x1f !!! */

		if(svsp->dis_fnc)
		{
			if(svsp->lastchar && svsp->m_awm
			   && (svsp->lastrow == svsp->row))
			{
				svsp->cur_offset++;
				svsp->col = 0;
				svsp->lastchar = 0;
				check_scroll(svsp);
			}

			if(svsp->irm)
				ovbcopy((svsp->Crtat + svsp->cur_offset),
					(svsp->Crtat + svsp->cur_offset) + 1,
					(((svsp->maxcol)-1) - svsp->col)*CHR);
					
			write_char(svsp, attrib, ch);

			vt_selattr(svsp);

			if(svsp->col >= ((svsp->maxcol)-1)
			   && ch != 0x0a && ch != 0x0b && ch != 0x0c)
			{
				svsp->lastchar = 1;
				svsp->lastrow = svsp->row;
			}
			else if(ch == 0x0a || ch == 0x0b || ch == 0x0c)
			{
				svsp->cur_offset -= svsp->col;
				svsp->cur_offset += svsp->maxcol;
				svsp->col = 0;
				svsp->lastchar = 0;
			}
			else
			{
				svsp->cur_offset++;
				svsp->col++;
				svsp->lastchar = 0;
			}
		}			
		else
		{
			switch(ch)	
			{
				case 0x00:	/* NUL */
				case 0x01:	/* SOH */
				case 0x02:	/* STX */
				case 0x03:	/* ETX */
				case 0x04:	/* EOT */
				case 0x05:	/* ENQ */
				case 0x06:	/* ACK */
					break;
					
				case 0x07:	/* BEL */
					if(svsp->bell_on)
 					  sysbeep(PCVT_SYSBEEPF/1500, hz/4);
					break;
					
				case 0x08:	/* BS */
					if(svsp->col > 0)
					{
						svsp->cur_offset--;
						svsp->col--;
					}
					break;
					
				case 0x09:	/* TAB */
					while(svsp->col < ((svsp->maxcol)-1))
					{
						svsp->cur_offset++;
						if(svsp->
						   tab_stops[++svsp->col])
							break;
					}
					break;
					
				case 0x0a:	/* LF */
				case 0x0b:	/* VT */
				case 0x0c:	/* FF */
					if(svsp->lnm)
					{
						svsp->cur_offset -= svsp->col;
						svsp->cur_offset +=
							svsp->maxcol;
						svsp->col = 0;
					}
					else
					{
						svsp->cur_offset +=
							svsp->maxcol;
					}
					break;
					
				case 0x0d:	/* CR */
					svsp->cur_offset -= svsp->col;
					svsp->col = 0;
					break;
					
				case 0x0e:	/* SO */
					svsp->GL = svsp->G1;
					break;
					
				case 0x0f:	/* SI */
					svsp->GL = svsp->G0;
					break;
	
				case 0x10:	/* DLE */
				case 0x11:	/* DC1/XON */
				case 0x12:	/* DC2 */
				case 0x13:	/* DC3/XOFF */
				case 0x14:	/* DC4 */
				case 0x15:	/* NAK */
				case 0x16:	/* SYN */
				case 0x17:	/* ETB */		
					break;
					
				case 0x18:	/* CAN */
					svsp->state = STATE_INIT;
					clr_parms(svsp);
					break;
					
				case 0x19:	/* EM */
					break;
					
				case 0x1a:	/* SUB */
					svsp->state = STATE_INIT;
					clr_parms(svsp);
					break;
					
				case 0x1b:	/* ESC */
					svsp->state = STATE_ESC;
					clr_parms(svsp);
					break;
					
				case 0x1c:	/* FS */
				case 0x1d:	/* GS */
				case 0x1e:	/* RS */
				case 0x1f:	/* US */
				break;
			}
		}
	}
	else
	{
		
	/* char range 0x20...0xff processing depends on current state */
		
		switch(svsp->state)
		{
			case STATE_INIT:
				if(svsp->lastchar && svsp->m_awm &&
				   (svsp->lastrow == svsp->row))
				{
					svsp->cur_offset++;
					svsp->col = 0;
					svsp->lastchar = 0;
					check_scroll(svsp);
				}

				if(svsp->irm)
					ovbcopy((svsp->Crtat
						 + svsp->cur_offset),
						(svsp->Crtat 
						 + svsp->cur_offset) + 1,
						(((svsp->maxcol)-1)
						 - svsp->col) * CHR);
					
				write_char(svsp, attrib, ch);

				vt_selattr(svsp);

				if(svsp->col >= ((svsp->maxcol)-1))
				{
					svsp->lastchar = 1;
					svsp->lastrow = svsp->row;
				}
				else
				{
					svsp->lastchar = 0;
					svsp->cur_offset++;
					svsp->col++;
				}
				break;

			case STATE_ESC:
				switch(ch)
				{
	
					case '#':	/* ESC # family */
						svsp->state = STATE_HASH;
						break;
	
					case '&':	/* ESC & family (HP) */
						if(svsp->vt_pure_mode ==
						   M_HPVT)
						{
							svsp->state =
								STATE_AMPSND;
							svsp->hp_state =
								SHP_INIT;
						}
						else
							svsp->state =
								STATE_INIT;
						break;
	
					case '(':	/* ESC ( family */
						svsp->state = STATE_BROPN;
						break;
	
					case ')':	/* ESC ) family */
						svsp->state = STATE_BRCLO;
						break;
	
					case '*':	/* ESC * family */
						svsp->state = STATE_STAR;
						break;
	
					case '+':	/* ESC + family */
						svsp->state = STATE_PLUS;
						break;
	
					case '-':	/* ESC - family */
						svsp->state = STATE_MINUS;
						break;
	
					case '.':	/* ESC . family */
						svsp->state = STATE_DOT;
						break;
	
					case '/':	/* ESC / family */
						svsp->state = STATE_SLASH;
						break;
	
					case '7':	/* SAVE CURSOR */
						vt_sc(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case '8':	/* RESTORE CURSOR */
						vt_rc(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case '=': /* keypad application mode */
#if !PCVT_INHIBIT_NUMLOCK
						vt_keyappl(svsp);
#endif
						svsp->state = STATE_INIT;
						break;
	
					case '>': /* keypad numeric mode */
#if !PCVT_INHIBIT_NUMLOCK
						vt_keynum(svsp);
#endif
						svsp->state = STATE_INIT;
						break;
	
					case 'D':	/* INDEX */
						vt_ind(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'E':	/* NEXT LINE */
						vt_nel(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'H': /* set TAB at current col */
						svsp->tab_stops[svsp->col] = 1;
						svsp->state = STATE_INIT;
						break;
	
					case 'M':	/* REVERSE INDEX */
						vt_ri(svsp);
						svsp->state = STATE_INIT;
						break;
 	
					case 'N':	/* SINGLE SHIFT G2 */
						svsp->ss2 = 1;
						svsp->state = STATE_INIT;
						break;
 	
					case 'O':	/* SINGLE SHIFT G3 */
						svsp->ss3 = 1;
						svsp->state = STATE_INIT;
						break;

					case 'P':	/* DCS detected */
						svsp->dcs_state = DCS_INIT;
						svsp->state = STATE_DCS;
						break;

					case 'Z': /* What are you = ESC [ c */
						vt_da(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case '[':	/* CSI detected */
						clr_parms(svsp);
						svsp->state = STATE_CSI;
						break;
	
					case '\\':	/* String Terminator */
						svsp->state = STATE_INIT;
						break;
 	
					case 'c':	/* hard reset */
						vt_ris(svsp);
						svsp->state = STATE_INIT;
						break;
	
#if PCVT_SETCOLOR
					case 'd':	/* set color sgr */
						if(color)
						{
							/* set shiftwidth=4 */
							sgr_tab_color
								[svsp->
								 vtsgr] =
								 svsp->c_attr
								 >> 8;
							user_attr =
								sgr_tab_color
								[0] << 8;
						}
						svsp->state = STATE_INIT;
						break;
#endif /* PCVT_SETCOLOR */
					case 'n': /* Lock Shift G2 -> GL */
						svsp->GL = svsp->G2;
						svsp->state = STATE_INIT;
						break;
	
					case 'o': /* Lock Shift G3 -> GL */
						svsp->GL = svsp->G3;
						svsp->state = STATE_INIT;
						break;
	
					case '}': /* Lock Shift G2 -> GR */
						svsp->GR = svsp->G2;
						svsp->state = STATE_INIT;
						break;
	
					case '|': /* Lock Shift G3 -> GR */
						svsp->GR = svsp->G3;
						svsp->state = STATE_INIT;
						break;
	
					case '~': /* Lock Shift G1 -> GR */
						svsp->GR = svsp->G1;
						svsp->state = STATE_INIT;
						break;
	
					default:
						svsp->state = STATE_INIT;
						break;
				}
				break;
	
			case STATE_HASH:
				switch(ch)
				{
					case '3': /* double height top half */
					case '4': /*double height bottom half*/
					case '5': /*single width sngle height*/
					case '6': /*double width sngle height*/
						svsp->state = STATE_INIT;
						break;
						
					case '8': /* fill sceen with 'E's */
						vt_aln(svsp);
						svsp->state = STATE_INIT;
						break;
						
					default: /* anything else */
						svsp->state = STATE_INIT;
						break;
				}
				break;
				
			case STATE_BROPN:	/* designate G0 */
			case STATE_BRCLO:	/* designate G1 */
			case STATE_STAR:	/* designate G2 */
			case STATE_PLUS:	/* designate G3 */
			case STATE_MINUS:	/* designate G1 (96) */
			case STATE_DOT:		/* designate G2 (96) */
			case STATE_SLASH:	/* designate G3 (96) */
				svsp->which[svsp->whichi++] = ch;
				if(ch >= 0x20 && ch <= 0x2f
				   && svsp->whichi <= 2)
					break;
				else if(ch >=0x30 && ch <= 0x7e)
				{
					svsp->which[svsp->whichi] = '\0';
					vt_designate(svsp);
				}
				svsp->whichi = 0;
				svsp->state = STATE_INIT;
				break;

			case STATE_CSIQM:	/* DEC private modes */
				switch(ch)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':	/* parameters */
						svsp->parms[svsp->parmi] *= 10;
						svsp->parms[svsp->parmi] +=
							(ch -'0');
						break;
	
					case ';':	/* next parameter */
						svsp->parmi = 
						 (svsp->parmi+1 < MAXPARMS) ?
						 svsp->parmi+1 : svsp->parmi;
						break;
	
					case 'h':	/* set mode */
						vt_set_dec_priv_qm(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'l':	/* reset mode */
						vt_reset_dec_priv_qm(svsp);
						svsp->state = STATE_INIT;
						break;

					case 'n':	/* Reports */
						vt_dsr(svsp);
						svsp->state = STATE_INIT;
						break;

					case 'K': /* selective erase in line */
						vt_sel(svsp);
						svsp->state = STATE_INIT;
						break;

					case 'J':/*selective erase in display*/
						vt_sed(svsp);
						svsp->state = STATE_INIT;
						break;

					default:
						svsp->state = STATE_INIT;
						break;

				}
				break;
				
			case STATE_CSI:
				switch(ch)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':	/* parameters */
						svsp->parms[svsp->parmi] *= 10;
						svsp->parms[svsp->parmi] +=
							(ch -'0');
						break;
	
					case ';':	/* next parameter */
						svsp->parmi = 
						 (svsp->parmi+1 < MAXPARMS) ?
						 svsp->parmi+1 : svsp->parmi;
						break;
	
					case '?':	/* ESC [ ? family */
						svsp->state = STATE_CSIQM;
						break;
						
					case '@':	/* insert char */
						vt_ic(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case '"': /* select char attribute */
						svsp->state = STATE_SCA;
						break;
	
					case '!': /* soft terminal reset */
						svsp->state = STATE_STR;
						break;
	
					case 'A':	/* cursor up */
						vt_cuu(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'B':	/* cursor down */
						vt_cud(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'C':	/* cursor forward */
						vt_cuf(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'D':	/* cursor backward */
						vt_cub(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'H': /* direct cursor addressing*/
						vt_curadr(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'J':	/* erase screen */
						vt_clreos(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'K':	/* erase line */
						vt_clreol(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'L':	/* insert line */
						vt_il(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'M':	/* delete line */
						vt_dl(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'P':	/* delete character */
						vt_dch(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'S':	/* scroll up */
						vt_su(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'T':	/* scroll down */
						vt_sd(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'X':	/* erase character */
						vt_ech(svsp);
						svsp->state = STATE_INIT;
						break;

					case 'c':	/* device attributes */
						vt_da(svsp); 
						svsp->state = STATE_INIT;
						break;
	
					case 'f': /* direct cursor addressing*/
						vt_curadr(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'g':	/* clear tabs */
						vt_clrtab(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'h':	/* set mode(s) */
						vt_set_ansi(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'i':	/* media copy */
						vt_mc(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'l':	/* reset mode(s) */
						vt_reset_ansi(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'm': /* select graphic rendition*/
						vt_sgr(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'n':	/* reports */
						vt_dsr(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'r': /* set scrolling region */
						vt_stbm(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'x': /*request/report parameters*/
						vt_reqtparm(svsp);
						svsp->state = STATE_INIT;
						break;
	
					case 'y': /* invoke selftest(s) */
						vt_tst(svsp);
						svsp->state = STATE_INIT;
						break;
	
					default:
						svsp->state = STATE_INIT;
						break;
				}
				break;

			case STATE_AMPSND:
				hp_entry(ch,svsp);
				break;

			case STATE_DCS:
				vt_dcsentry(ch,svsp);
				break;

			case STATE_SCA:
				switch(ch)
				{
					case 'q':
						vt_sca(svsp);
						svsp->state = STATE_INIT;
						break;
						
					default:
						svsp->state = STATE_INIT;
						break;
				}
				break;

			case STATE_STR:
				switch(ch)
				{
					case 'p': /* soft terminal reset */
						vt_str(svsp);
						svsp->state = STATE_INIT;
						break;

					default:
						svsp->state = STATE_INIT;
						break;
				}
				break;

			default:		/* failsafe */
				svsp->state = STATE_INIT;
				break;

		}
	}

	check_scroll(svsp);				/* check scroll up */

	svsp->row = (((svsp->Crtat + svsp->cur_offset) - svsp->Crtat)
		     / svsp->maxcol);	/* current row update */

	if(svsp == vsp)			/* on current displayed page ?	*/
		cursor_pos_valid = 1;	/* position is valid now */

	/* take care of last character on line behaviour */
	
	if(svsp->lastchar && (svsp->col < ((svsp->maxcol)-1)))
		svsp->lastchar = 0;
}

/*---------------------------------------------------------------------------*
 *	do character set transformation and write to display memory
 *---------------------------------------------------------------------------*/
static void
write_char(struct video_state *svsp, U_char kernel, U_char ch)
{
	register u_short result;
	register u_short *gsave;	/* gcc warning: "may be used
					 * uninitialized" = don't care ! */
	if(kernel)
		result = kern_attr;
	else
		result = svsp->c_attr;

	if(svsp->ss2)				/* single shift G2 -> GL ? */
	{
		gsave = svsp->GL;		/* save GL */
		svsp->GL = svsp->G2;		/* G2 -> GL */
	}
	else if(svsp->ss3)			/* single shift G3 -> GL ? */
	{
		gsave = svsp->GL;		/* save GL */
		svsp->GL = svsp->G3;		/* G3 -> GL */
	}
	
	if(ch >= 0xA0)				/* use GR if ch >= 0xA0 */
		result |= svsp->GR[ch-0xA0];

	else if(ch >= 0x80)			/* display controls C1 */
	{
		if(vgacs[svsp->vga_charset].secondloaded)
		{
			result |= ((ch-0x60) | CSH);
		}
		else	/* use normal ibm charset for control display */
		{
			result |= ch;
		}
	}

	else if (ch >= 0x20)			/* use GL if ch >= 0x20 */
		result |= svsp->GL[ch-0x20];

	else					/* display controls C0 */
	{
		if(vgacs[svsp->vga_charset].secondloaded)
		{
			result |= (ch | CSH);
		}
		else	/* use normal ibm charset for control display */
		{
			result |= ch;
		}
	}
	
	*(svsp->Crtat + svsp->cur_offset) = result;  /* write char to screen */

	if(svsp->ss2)				/* was single shift G2->GL ? */
	{
		svsp->GL = gsave;		/* restore GL */
		svsp->ss2 = 0;			/* reset flag */
	}
	else if(svsp->ss3)			/* was single shift G3->GL ? */
	{
		svsp->GL = gsave;		/* restore GL */
		svsp->ss3 = 0;			/* reset flag */
	}
	
}

/*---------------------------------------------------------------------------*
 *	this is the absolute cold initialization of the emulator
 *---------------------------------------------------------------------------*/
static void
vt_coldinit(void)
{
	extern u_short csd_ascii[];		/* pcvt_tbl.h */
	extern u_short csd_supplemental[];
	
	u_short volatile *cp = Crtat + (CGA_BUF-MONO_BUF)/CHR;  /* gcc 2.4.5 */
	u_short was;
	int nscr, charset;
	int equipment;
	u_short *SaveCrtat = Crtat;

	do_initialization = 0;
	
	equipment = ((rtcin(RTC_EQUIPMENT)) >> 4) & 0x03;
	
	switch(equipment)
	{
		default:
		case EQ_EGAVGA:

			/* set memory start to CGA == B8000 */
			
			Crtat = Crtat + (CGA_BUF-MONO_BUF)/CHR;
		
			/* find out, what monitor is connected */
			
			was = *cp;
			*cp = (u_short) 0xA55A;
			if (*cp != 0xA55A)
			{
				addr_6845 = MONO_BASE;
				color = 0;
			}
			else
			{
				*cp = was;
				addr_6845 = CGA_BASE;
				color = 1;
			}

			if(vga_test())
			{
				adaptor_type = VGA_ADAPTOR;
				totalfonts = 8;
				if(color == 0)
				{

					/*
					 * if a mono monitor is
					 * attached to a vga, it comes
					 * up with a mda emulation.
					 */

					/*
					 * program sequencer to access
					 * video ram
					 */

					/* synchronous reset */
					outb(TS_INDEX, TS_SYNCRESET);
					outb(TS_DATA, 0x01);

					/* write to map 0 & 1 */
					outb(TS_INDEX, TS_WRPLMASK);
					outb(TS_DATA, 0x03);
				
					/* odd-even addressing */
					outb(TS_INDEX, TS_MEMMODE);
					outb(TS_DATA, 0x03);
				
					/* clear synchronous reset */
					outb(TS_INDEX, TS_SYNCRESET);
					outb(TS_DATA, 0x03);

					/*
					 * program graphics controller
					 * to access character
					 * generator
					 */

					/* select map 0 for cpu reads */
					outb(GDC_INDEX, GDC_RDPLANESEL);
					outb(GDC_DATA, 0x00);
				
					/* enable odd-even addressing */
					outb(GDC_INDEX, GDC_MODE);
					outb(GDC_DATA, 0x10);
				
					/* map starts at 0xb000 */
					outb(GDC_INDEX, GDC_MISC);
					outb(GDC_DATA, 0x0a);

					Crtat = SaveCrtat; /* mono start */
				}
				/* find out which chipset we are running on */
				vga_type = vga_chipset();
			}
			else
			{
				if(color)
				{
					adaptor_type = EGA_ADAPTOR;
					totalfonts = 4;
				}
				else
				{	/* mono ega -> MDA .... */
					/* NOT TESTED !!!!!!!!! */
					addr_6845 = MONO_BASE;
					adaptor_type = MDA_ADAPTOR;
					totalfonts = 0;
					Crtat = SaveCrtat;
					break;
				}
					
			}
	
			/* decouple vga charsets and intensity */
			set_2ndcharset();

			break;

		case EQ_40COLOR:
		case EQ_80COLOR:
			Crtat = Crtat + (CGA_BUF-MONO_BUF)/CHR;
			addr_6845 = CGA_BASE;
			adaptor_type = CGA_ADAPTOR;
			totalfonts = 0;
			break;

		case EQ_80MONO:
			addr_6845 = MONO_BASE;
			adaptor_type = MDA_ADAPTOR;
			totalfonts = 0;			
			break;
	}

	/* establish default colors */
	
	if(color)
	{
		kern_attr = (COLOR_KERNEL_FG | COLOR_KERNEL_BG) << 8;
		user_attr = sgr_tab_color[0] << 8;
	}
	else
	{
		kern_attr = (MONO_KERNEL_FG | MONO_KERNEL_BG) << 8;
		if(adaptor_type == MDA_ADAPTOR)
			user_attr = sgr_tab_imono[0] << 8;
		else
			user_attr = sgr_tab_mono[0] << 8;		
	}

	totalscreens = 1;	/* for now until malloced */
	
	for(nscr = 0; nscr < PCVT_NSCREENS; nscr++)
	{
		vs[nscr].Crtat = Crtat;		/* all same until malloc'ed */
		vs[nscr].Memory = Crtat;	/* until malloc'ed */
		vs[nscr].cur_offset = 0;	/* cursor offset */
		vs[nscr].c_attr = user_attr;	/* non-kernel attributes */
		vs[nscr].bell_on = 1;		/* enable bell */
		vs[nscr].sevenbit = 0;		/* set to 8-bit path */
		vs[nscr].dis_fnc = 0;		/* disable display functions */
		vs[nscr].transparent = 0;    /* disable internal tranparency */
		vs[nscr].lastchar = 0;		/* VTxxx behaviour of last
						 * char on line */
		vs[nscr].report_chars = NULL;	/* VTxxx reports init */
		vs[nscr].report_count = 0;	/* VTxxx reports init */
		vs[nscr].state = STATE_INIT;	/* main state machine init */
		vs[nscr].m_awm = 1;		/* enable auto wrap mode */
		vs[nscr].m_om = 0;		/* origin mode = absolute */
		vs[nscr].sc_flag = 0;		/* init saved cursor flag */
		vs[nscr].which_fkl = SYS_FKL;  /* display system fkey-labels */
		vs[nscr].labels_on = 1;		/* if in HP-mode, display
						 * fkey-labels */
		vs[nscr].attribute = 0;		/* HP mode init */
		vs[nscr].key = 0;		/* HP mode init */
		vs[nscr].l_len = 0;		/* HP mode init */
		vs[nscr].s_len = 0;		/* HP mode init */
		vs[nscr].m_len = 0;		/* HP mode init */
		vs[nscr].i = 0;			/* HP mode init */
		vs[nscr].vt_pure_mode = M_PUREVT; /* initial mode: pure VT220*/
		vs[nscr].vga_charset = CH_SET0;	/* use bios default charset */

#if PCVT_24LINESDEF				/* true compatibility */
		vs[nscr].screen_rows = 24;	/* default 24 rows on screen */
#else						/* full screen */
		vs[nscr].screen_rows = 25;	/* default 25 rows on screen */
#endif /* PCVT_24LINESDEF */

		vs[nscr].screen_rowsize = 25;	/* default 25 rows on screen */
		vs[nscr].scrr_beg = 0;		/* scrolling region begin row*/
		vs[nscr].scrr_len=vs[nscr].screen_rows-1; /* scrolling region
							   * end */
		if(vga_family == VGA_F_TRI)
			vs[nscr].cursor_start = 1; /* cursor upper scanline */
		else
			vs[nscr].cursor_start = 0; /* cursor upper scanline */
		vs[nscr].cursor_end = 15;	/* cursor lower scanline */
		vs[nscr].cursor_on = 1;		/* cursor is on */
		vs[nscr].ckm = 1;		/* normal cursor key mode */
		vs[nscr].irm = 0;		/* replace mode */
		vs[nscr].lnm = 0;		/* CR only */
		vs[nscr].selchar = 0;		/* selective attribute off */
		vs[nscr].G0 = csd_ascii;	/* G0 = ascii	*/
		vs[nscr].G1 = csd_ascii;	/* G1 = ascii	*/
		vs[nscr].G2 = csd_supplemental;	/* G2 = supplemental */
		vs[nscr].G3 = csd_supplemental;	/* G3 = supplemental */
		vs[nscr].GL = vs[nscr].G0;	/* GL = G0 */
		vs[nscr].GR = vs[nscr].G2;	/* GR = G2 */
		vs[nscr].whichi = 0;		/* char set designate init */
		vs[nscr].which[0] = '\0';	/* char set designate init */
		vs[nscr].hp_state = SHP_INIT;	/* init HP mode state machine*/
		vs[nscr].dcs_state = DCS_INIT; /* init DCS mode state machine*/
		vs[nscr].ss2 = 0;		/* init single shift 2 */
		vs[nscr].ss3 = 0;		/* init single shift 3 */
		vs[nscr].maxcol = SCR_COL80;	/* 80 columns now (you MUST!!!
						 * start with 80!)
						 * see et4000_col() for
						 * reason ... */
		vs[nscr].wd132col = 0;		/* help good old WD .. */
		vs[nscr].scroll_lock = 0;	/* scrollock off */
#if PCVT_INHIBIT_NUMLOCK
		vs[nscr].num_lock = 0; 		/* numlock off */
#else
		vs[nscr].num_lock = 1;		/* numlock on */
#endif
		vs[nscr].caps_lock = 0;		/* capslock off */
		vs[nscr].shift_lock = 0;	/* shiftlock off */

#if PCVT_24LINESDEF				/* true compatibility */
		vs[nscr].force24 = 1;		/* force 24 lines */
#else						/* maximum screen size */
		vs[nscr].force24 = 0;		/* no 24 lines force yet */
#endif /* PCVT_24LINESDEF */

		vt_clearudk(&(vs[nscr]));	/* clear vt220 udk's */

		vt_str(&vs[nscr]);		/* init emulator */

		if(nscr == 0)
		{
			fillw(user_attr | ' ',
				vs[nscr].Crtat,
				vs[nscr].maxcol * vs[nscr].screen_rowsize);
		}

#if PCVT_USL_VT_COMPAT
		vs[nscr].smode.mode = VT_AUTO;
		vs[nscr].smode.relsig = vs[nscr].smode.acqsig =
			vs[nscr].smode.frsig = 0;
		vs[nscr].proc = 0;
		vs[nscr].pid = vs[nscr].vt_status = 0;
#endif /* PCVT_USL_VT_COMPAT */

	}

 	for(charset = 0;charset < NVGAFONTS;charset++)
	{
		vgacs[charset].loaded = 0;		/* not populated yet */
		vgacs[charset].secondloaded = 0;	/* not populated yet */
		switch(adaptor_type)
		{
			case VGA_ADAPTOR:

				/*
				 * for a VGA, do not assume any
				 * constant - instead, read the actual
				 * values. This avoid problems with
				 * LCD displays that apparently happen
				 * to use font matrices up to 19
				 * scan lines and 475 scan lines 
				 * total in order to make use of the
				 * whole screen area
				 */

				outb(addr_6845, CRTC_VDE);
				vgacs[charset].scr_scanlines = 
					inb(addr_6845 + 1);
				outb(addr_6845, CRTC_MAXROW);
				vgacs[charset].char_scanlines =
					inb(addr_6845 + 1);
				break;

			case EGA_ADAPTOR:
				/* 0x5D for 25 lines */
				vgacs[charset].scr_scanlines = 0x5D;
				/* 0x4D for 25 lines */
				vgacs[charset].char_scanlines = 0x4D;
				break;

			case CGA_ADAPTOR:
			case MDA_ADAPTOR:
			default:
				/* These shouldn't be used for CGA/MDA */
				vgacs[charset].scr_scanlines = 0;
				vgacs[charset].char_scanlines = 0;
				break;
		}
		vgacs[charset].screen_size = SIZ_25ROWS; /* set screen size */
 	}

 	vgacs[0].loaded = 1; /* The BIOS loaded this at boot */

	/* set cursor for first screen */
	
	outb(addr_6845,CRTC_CURSTART);	/* cursor start reg */
	outb(addr_6845+1,vs[0].cursor_start);	
	outb(addr_6845,CRTC_CUREND);	/* cursor end reg */
	outb(addr_6845+1,vs[0].cursor_end);

	/* this is to satisfy ddb */
	
	if(!keyboard_is_initialized)
		kbd_code_init1();

	/* update keyboard led's */
	
	update_led();
}

/*---------------------------------------------------------------------------*
 *	get kernel memory for virtual screens
 *
 *	CAUTION: depends on "can_do_132col" being set properly, or
 *	depends on vga_type() being run before calling this !!!
 *
 *---------------------------------------------------------------------------*/
void
vt_coldmalloc(void)
{
	int nscr;
	int screen_max_size;

	switch(adaptor_type)
	{
		default:
		case MDA_ADAPTOR:
		case CGA_ADAPTOR:
			screen_max_size = MAXROW_MDACGA * MAXCOL_MDACGA * CHR;
			break;

		case EGA_ADAPTOR:
			screen_max_size = MAXROW_EGA * MAXCOL_EGA * CHR;
			break;

		case VGA_ADAPTOR:
			if(can_do_132col)
				screen_max_size =
					MAXROW_VGA * MAXCOL_SVGA * CHR;
			else
				screen_max_size =
					MAXROW_VGA * MAXCOL_VGA * CHR;
	}
	
	for(nscr = 0; nscr < PCVT_NSCREENS; nscr++)
	{
		if((vs[nscr].Memory =
		    (u_short *)malloc(screen_max_size, M_DEVBUF, M_WAITOK))
		   == NULL)
		{
			printf("pcvt: screen memory malloc failed, "
			       "NSCREEN=%d, nscr=%d\n",
			       PCVT_NSCREENS, nscr);
			break;
		}
		if(nscr != 0)
		{
			vs[nscr].Crtat = vs[nscr].Memory;
			fillw(user_attr | ' ',
				vs[nscr].Crtat,
				vs[nscr].maxcol * vs[nscr].screen_rowsize);
			totalscreens++;
		}
	}
}

/*---------------------------------------------------------------------------*
 *	check if we must scroll up screen
 *---------------------------------------------------------------------------*/
static void
check_scroll(struct video_state *svsp)
{
	if(svsp->cur_offset >=
	   ((svsp->scrr_beg + svsp->scrr_len + 1) * svsp->maxcol)) 
	{

/* XXX */	critical_scroll = 1;
		
		bcopy (	svsp->Crtat + (svsp->scrr_beg * svsp->maxcol)
			+ svsp->maxcol,
			svsp->Crtat + (svsp->scrr_beg * svsp->maxcol),
			svsp->maxcol * svsp->scrr_len * CHR );

		fillw(	user_attr | ' ',
			svsp->Crtat
			+ ((svsp->scrr_beg + svsp->scrr_len) * svsp->maxcol),
			svsp->maxcol);
			
		svsp->cur_offset -= svsp->maxcol;

/* XXX */	if(switch_page != -1)
/* XXX */	{
/* XXX */		vgapage(switch_page);
/* XXX */		switch_page = -1;			
/* XXX */	}

/* XXX */	critical_scroll = 0;
		
  
/*XXX*/  	if(svsp->scroll_lock && svsp->openf && curproc)
		     tsleep((caddr_t)&(svsp->scroll_lock), PUSER, "scrlck", 0);
  	}
}

/*---------------------------------------------------------------------------*
 *	write to one user function key label
 *---------------------------------------------------------------------------*/
static void
writefkl(int num, u_char *string, struct video_state *svsp)
{
	if((num < 0) || (num > 7))	/* range ok ? */
		return;
		
	strncpy(svsp->ufkl[num], string, 16); /* save string in static array */

	if(svsp->which_fkl == USR_FKL)
		wrfkl(num,string,svsp);
}

/*---------------------------------------------------------------------------*
 *	write to one system function key label
 *---------------------------------------------------------------------------*/
void
swritefkl(int num, u_char *string, struct video_state *svsp)
{
	if((num < 0) || (num > 7))	/* range ok ? */
		return;
		
	strncpy(svsp->sfkl[num], string, 16); /* save string in static array */

	if(svsp->which_fkl == SYS_FKL)
		wrfkl(num,string,svsp);
}

/*---------------------------------------------------------------------------*
 *	write function key label onto screen
 *---------------------------------------------------------------------------*/
static void
wrfkl(int num, u_char *string, struct video_state *svsp)
{
	register u_short *p;
	register u_short *p1;	
	register int cnt = 0;
		
	if(!svsp->labels_on || (svsp->vt_pure_mode == M_PUREVT))
		return;

	p = (svsp->Crtat
	     + (svsp->screen_rows * svsp->maxcol)); /* screen_rows+1 line */

	if(svsp->maxcol == SCR_COL80)
	{
		if(num < 4)	/* labels 1 .. 4 */
			p += (num * LABEL_LEN);
		else		/* labels 5 .. 8 */
			p += ((num * LABEL_LEN) + LABEL_MID + 1);
	}
	else
	{
		if(num < 4)	/* labels 1 .. 4 */
			p += (num * (LABEL_LEN + 6));
		else		/* labels 5 .. 8 */
			p += ((num * (LABEL_LEN + 6)) + LABEL_MID + 11);

	}		
	p1 = p + svsp->maxcol;	/* second label line */
	
	while((*string != '\0') && (cnt < 8))
	{
		*p = ((0x70 << 8) + (*string & 0xff));
		p++;
		string++;
		cnt++;
	}
	while(cnt < 8)
	{
		*p = ((0x70 << 8) + ' ');
		p++;
		cnt++;
	}

	while((*string != '\0') && (cnt < 16))
	{
		*p1 = ((0x70 << 8) + (*string & 0xff));
		p1++;
		string++;
		cnt++;
	}
	while(cnt < 16)
	{
		*p1 = ((0x70 << 8) + ' ');
		p1++;
		cnt++;
	}
}

/*---------------------------------------------------------------------------*
 *	remove (blank) function key labels, row/col and status line
 *---------------------------------------------------------------------------*/
void
fkl_off(struct video_state *svsp)
{
	register u_short *p;
	register int num;
		
	svsp->labels_on = 0;

	p = (svsp->Crtat+(svsp->screen_rows*svsp->maxcol));

	for(num = 0; num < (3*svsp->maxcol); num++)
		*p++ = ' ';
}

/*---------------------------------------------------------------------------*
 *	(re-) display function key labels, row/col and status line
 *---------------------------------------------------------------------------*/
void
fkl_on(struct video_state *svsp)
{
	svsp->labels_on = 1;

	if(svsp->which_fkl == SYS_FKL)
		sw_sfkl(svsp);
	else if(svsp->which_fkl == USR_FKL)
		sw_ufkl(svsp);
}

/*---------------------------------------------------------------------------*
 *	set emulation mode, switch between pure VTxxx mode and HP/VTxxx mode
 *---------------------------------------------------------------------------*/
void
set_emulation_mode(struct video_state *svsp, int mode)
{
	if(svsp->vt_pure_mode == mode)
		return;
	
	fillw(user_attr | ' ',
		svsp->Crtat,
		svsp->maxcol * svsp->screen_rowsize);

	clr_parms(svsp);		/* escape parameter init */
	svsp->state = STATE_INIT;	/* initial state */

	svsp->col = 0;			/* init row */
	svsp->row = 0;			/* init col */
	svsp->cur_offset = 0;		/* cursor offset init */
	svsp->scrr_beg = 0;		/* start of scrolling region */
	svsp->sc_flag = 0;		/* invalidate saved cursor position */
	svsp->transparent = 0;		/* disable control code processing */

	if(mode == M_HPVT)		/* vt-pure -> hp/vt-mode */
	{
		svsp->vt_pure_mode = M_HPVT;

		svsp->vs_tty->t_winsize.ws_row
			= svsp->screen_rows = svsp->screen_rowsize - 3;

		svsp->scrr_len = svsp->screen_rows - 1;

		update_hp(svsp);
	}
	else if(mode == M_PUREVT)	/* hp/vt-mode -> vt-pure */
	{
		svsp->vt_pure_mode = M_PUREVT;

		svsp->screen_rows = svsp->screen_rowsize;

		svsp->vs_tty->t_winsize.ws_row
			= svsp->screen_rows = svsp->screen_rowsize;

		svsp->scrr_len = svsp->screen_rows - 1;
	}		

#if PCVT_SIGWINCH
	pgsignal(svsp->vs_tty->t_pgrp, SIGWINCH, 1);
#endif /* PCVT_SIGWINCH */			

}

/*---------------------------------------------------------------------------*
 *	initialize user function key labels
 *---------------------------------------------------------------------------*/
void
init_ufkl(struct video_state *svsp)
{
	writefkl(0,(u_char *)"   f1",svsp);	/* init fkey labels */
	writefkl(1,(u_char *)"   f2",svsp);
	writefkl(2,(u_char *)"   f3",svsp);
	writefkl(3,(u_char *)"   f4",svsp);
	writefkl(4,(u_char *)"   f5",svsp);
	writefkl(5,(u_char *)"   f6",svsp);
	writefkl(6,(u_char *)"   f7",svsp);
	writefkl(7,(u_char *)"   f8",svsp);
}

/*---------------------------------------------------------------------------*
 *	initialize system user function key labels
 *---------------------------------------------------------------------------*/
void
init_sfkl(struct video_state *svsp)
{
			    /* 1234567812345678 */
	if(can_do_132col)
				    /* 1234567812345678 */
		swritefkl(0,(u_char *)"132     COLUMNS ",svsp);
	else
		swritefkl(0,(u_char *)" ",svsp);	

			    /* 1234567812345678 */
	swritefkl(1,(u_char *)"SOFT-RSTTERMINAL",svsp);
	if(svsp->force24)
		swritefkl(2,(u_char *)"FORCE24 ENABLE *",svsp);
	else
		swritefkl(2,(u_char *)"FORCE24 ENABLE  ",svsp);	

#if PCVT_SHOWKEYS	    /* 1234567812345678 */
	if(svsp == &vs[0])
		swritefkl(3,(u_char *)"KEYBSCANDISPLAY ",svsp);
	else
		swritefkl(3,(u_char *)" ",svsp);
#else
	swritefkl(3,(u_char *)" ",svsp);
#endif /* PCVT_SHOWKEYS */

			    /* 1234567812345678 */
	swritefkl(4,(u_char *)"BELL    ENABLE *",svsp);
	swritefkl(5,(u_char *)"8-BIT   ENABLE *",svsp);
	swritefkl(6,(u_char *)"DISPLAY FUNCTNS ",svsp);
	swritefkl(7,(u_char *)"AUTOWRAPENABLE *",svsp);
			    /* 1234567812345678 */	
}
 
/*---------------------------------------------------------------------------*
 *	switch display to user function key labels
 *---------------------------------------------------------------------------*/
void
sw_ufkl(struct video_state *svsp)
{
	int i;
	svsp->which_fkl = USR_FKL;
	for(i = 0; i < 8; i++)
		wrfkl(i,svsp->ufkl[i],svsp);
}

/*---------------------------------------------------------------------------*
 *	switch display to system function key labels
 *---------------------------------------------------------------------------*/
void
sw_sfkl(struct video_state *svsp)
{
	int i;
	svsp->which_fkl = SYS_FKL;
	for(i = 0; i < 8; i++)
		wrfkl(i,svsp->sfkl[i],svsp);
}

/*---------------------------------------------------------------------------*
 *	toggle force 24 lines
 *---------------------------------------------------------------------------*/
void
toggl_24l(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->force24)
		{
			svsp->force24 = 0;
			swritefkl(2,(u_char *)"FORCE24 ENABLE  ",svsp);	
		}
		else
		{
			svsp->force24 = 1;			
			swritefkl(2,(u_char *)"FORCE24 ENABLE *",svsp);
		}
		set_screen_size(svsp, vgacs[(svsp->vga_charset)].screen_size);
	}
}

#if PCVT_SHOWKEYS
/*---------------------------------------------------------------------------*
 *	toggle keyboard scancode display
 *---------------------------------------------------------------------------*/
void
toggl_kbddbg(struct video_state *svsp)
{
	if((svsp->which_fkl == SYS_FKL) && (svsp == &vs[0]))
	{
		if(keyboard_show)
		{
			keyboard_show = 0;
			swritefkl(3,(u_char *)"KEYBSCANDISPLAY ",svsp);
		}
		else
		{
			keyboard_show = 1;
			swritefkl(3,(u_char *)"KEYBSCANDISPLAY*",svsp);
		}
	}
}
#endif /* PCVT_SHOWKEYS */

/*---------------------------------------------------------------------------*
 *	toggle display functions
 *---------------------------------------------------------------------------*/
void
toggl_dspf(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->dis_fnc)
		{
			svsp->dis_fnc = 0;
			swritefkl(6,(u_char *)"DISPLAY FUNCTNS ",svsp);
		}
		else
		{
			svsp->dis_fnc = 1;
			swritefkl(6,(u_char *)"DISPLAY FUNCTNS*",svsp);
		}
	}
}

/*---------------------------------------------------------------------------*
 *	auto wrap on/off
 *---------------------------------------------------------------------------*/
void
toggl_awm(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->m_awm)
		{
			svsp->m_awm = 0;
			swritefkl(7,(u_char *)"AUTOWRAPENABLE  ",svsp);
		}
		else
		{
			svsp->m_awm = 1;
			swritefkl(7,(u_char *)"AUTOWRAPENABLE *",svsp);
		}
	}
}

/*---------------------------------------------------------------------------*
 *	bell on/off
 *---------------------------------------------------------------------------*/
void
toggl_bell(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->bell_on)
		{
			svsp->bell_on = 0;
			swritefkl(4,(u_char *)"BELL    ENABLE  ",svsp);
		}
		else
		{
			svsp->bell_on = 1;
			swritefkl(4,(u_char *)"BELL    ENABLE *",svsp);
		}
	}
}

/*---------------------------------------------------------------------------*
 *	7/8 bit usage
 *---------------------------------------------------------------------------*/
void
toggl_sevenbit(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->sevenbit)
		{
			svsp->sevenbit = 0;
			swritefkl(5,(u_char *)"8-BIT   ENABLE *",svsp);
		}
		else
		{
			svsp->sevenbit = 1;
			swritefkl(5,(u_char *)"8-BIT   ENABLE  ",svsp);
		}
	}
}

/*---------------------------------------------------------------------------*
 *	80 / 132 columns
 *---------------------------------------------------------------------------*/
void
toggl_columns(struct video_state *svsp)
{
	if(svsp->which_fkl == SYS_FKL)
	{
		if(svsp->maxcol == SCR_COL132)
		{
			if(vt_col(svsp, SCR_COL80))
				svsp->maxcol = 80;
		}
		else
		{
			if(vt_col(svsp, SCR_COL132))
				svsp->maxcol = 132;
		}
	}
}

/*---------------------------------------------------------------------------*
 *	toggle vga 80/132 column operation
 *---------------------------------------------------------------------------*/
int
vt_col(struct video_state *svsp, int cols)
{
	if(vga_col(svsp, cols) == 0)
		return(0);
		
	if(cols == SCR_COL80)
		swritefkl(0,(u_char *)"132     COLUMNS ",svsp);
	else
		swritefkl(0,(u_char *)"132     COLUMNS*",svsp);

	fillw(user_attr | ' ',
		svsp->Crtat,
		svsp->maxcol * svsp->screen_rowsize);
	
	clr_parms(svsp);		/* escape parameter init */
	svsp->state = STATE_INIT;	/* initial state */
	svsp->col = 0;			/* init row */
	svsp->row = 0;			/* init col */
	svsp->cur_offset = 0;		/* cursor offset init */
	svsp->sc_flag = 0;		/* invalidate saved cursor position */
	svsp->scrr_beg = 0;		/* reset scrolling region */
	svsp->scrr_len = svsp->screen_rows-1; /*reset scrolling region legnth*/
	svsp->transparent = 0;		/* disable control code processing */
	svsp->selchar = 0;		/* selective attr off */
	vt_initsel(svsp);		/* re-init sel attr */

	update_hp(svsp);		/* update labels, row/col, page ind */

	/* Update winsize struct to reflect screen size */

	if(svsp->vt_pure_mode == M_HPVT)
		svsp->vs_tty->t_winsize.ws_row = svsp->screen_rowsize - 3;
	else
		svsp->vs_tty->t_winsize.ws_row = svsp->screen_rowsize;
	svsp->vs_tty->t_winsize.ws_col = svsp->maxcol;

	svsp->vs_tty->t_winsize.ws_xpixel = (cols == SCR_COL80)? 720: 1056;
	svsp->vs_tty->t_winsize.ws_ypixel = 400;

#if PCVT_SIGWINCH
	pgsignal(svsp->vs_tty->t_pgrp, SIGWINCH, 1);
#endif /* PCVT_SIGWINCH */			

	return(1);
}	

/*---------------------------------------------------------------------------*
 *	update HP stuff on screen
 *---------------------------------------------------------------------------*/
static void
update_hp(struct video_state *svsp)
{
	if(svsp->vt_pure_mode != M_HPVT || (!svsp->labels_on))
		return;

	/* update fkey labels */

	if(svsp->which_fkl == SYS_FKL)
		sw_sfkl(svsp);
	else if(svsp->which_fkl == USR_FKL)
		sw_ufkl(svsp);

	if(vsp == svsp)
	{
		/* update current displayed screen indicator */

		*((svsp->Crtat + ((svsp->screen_rows + 2) * svsp->maxcol))
		  + svsp->maxcol - 3) = user_attr | '[';
		*((svsp->Crtat + ((svsp->screen_rows + 2) * svsp->maxcol))
		  + svsp->maxcol - 2) = user_attr | current_video_screen + '0';
		*((svsp->Crtat + ((svsp->screen_rows + 2) * svsp->maxcol))
		  + svsp->maxcol - 1) = user_attr | ']';
	}
}

/*---------------------------------------------------------------------------*
 *	initialize ANSI escape sequence parameter buffers
 *---------------------------------------------------------------------------*/
void
clr_parms(struct video_state *svsp)
{
	register int i;
	for(i=0; i < MAXPARMS; i++)
		svsp->parms[i] = 0;
	svsp->parmi = 0;
}


/*---------------------------------------------------------------------------*
 *
 *	partial HP 2392 ANSI mode Emulator
 *	==================================
 *
 *	this part tooks over the emulation of some escape sequences
 *	needed to handle the function key labels
 *
 *	They are modeled after the corresponding escape sequences
 *	introduced with the HP2392 terminals from Hewlett-Packard.
 *
 *	see:
 *	"HP2392A, Display Terminal Reference Manual",
 *	HP Manual Part Number 02390-90001
 *	and:
 *	Reference Manual Supplement
 *	"2392A Display Terminal Option 049, ANSI Operation"
 *	HP Manual Part Number 02390-90023EN
 *
 *---------------------------------------------------------------------------*/

static void
hp_entry(U_char ch, struct video_state *svsp)
{
	switch(svsp->hp_state)
	{
		case SHP_INIT:
			switch(ch)
			{
				case 'f':
					svsp->hp_state = SHP_AND_F;
					svsp->attribute = 0;
					svsp->key = 0;
					svsp->l_len = 0;
					svsp->s_len = 0;
					svsp->i = 0;
					break;

				case 'j':
					svsp->m_len = 0;
					svsp->hp_state = SHP_AND_J;
					break;

				case 's':
					svsp->hp_state = SHP_AND_ETE;
					break;

				default:
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					break;
			}
			break;
	
		case SHP_AND_F:
			if((ch >= '0') && (ch <= '8'))
			{
				svsp->attribute = ch;
				svsp->hp_state = SHP_AND_Fa;
			}
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;

		case SHP_AND_Fa:
			if(ch == 'a')
				svsp->hp_state = SHP_AND_Fak;
			else if(ch == 'k')
			{
				svsp->key = svsp->attribute;
				svsp->hp_state = SHP_AND_Fakd;
			}
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;

		case SHP_AND_Fak:
			if((ch >= '1') && (ch <= '8'))
			{
				svsp->key = ch;
				svsp->hp_state = SHP_AND_Fak1;
			}
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;

		case SHP_AND_Fak1:			
			if(ch == 'k')
				svsp->hp_state = SHP_AND_Fakd;
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;
			
		case SHP_AND_Fakd:
			if(svsp->l_len > 16)
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			else if(ch >= '0' && ch <= '9')
			{
				svsp->l_len *= 10;
				svsp->l_len += (ch -'0');
			}
			else if(ch == 'd')
				svsp->hp_state = SHP_AND_FakdL;
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;

		case SHP_AND_FakdL:
			if(svsp->s_len > 80)
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			else if(ch >= '0' && ch <= '9')
			{
				svsp->s_len *= 10;
				svsp->s_len += (ch -'0');
			}
			else if(ch == 'L')
			{
				svsp->hp_state = SHP_AND_FakdLl;
				svsp->transparent = 1;
			}
			else
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
			}
			break;

		case SHP_AND_FakdLl:
			svsp->l_buf[svsp->i] = ch;
			if(svsp->i >= svsp->l_len-1)
			{
				svsp->hp_state = SHP_AND_FakdLls;
				svsp->i = 0;
				if(svsp->s_len == 0)
				{
					svsp->state = STATE_INIT;
					svsp->hp_state = SHP_INIT;
					svsp->transparent = 0;
					svsp->i = 0;
					svsp->l_buf[svsp->l_len] = '\0';
					svsp->s_buf[svsp->s_len] = '\0';
					writefkl((svsp->key - '0' -1),
						 svsp->l_buf, svsp);
				}
			}
			else
				svsp->i++;
			break;
			
		case SHP_AND_FakdLls:
			svsp->s_buf[svsp->i] = ch;
			if(svsp->i >= svsp->s_len-1)
			{
				svsp->state = STATE_INIT;
				svsp->hp_state = SHP_INIT;
				svsp->transparent = 0;
				svsp->i = 0;
				svsp->l_buf[svsp->l_len] = '\0';
				svsp->s_buf[svsp->s_len] = '\0';
				writefkl((svsp->key - '0' -1), svsp->l_buf,
					 svsp);
			}
			else
				svsp->i++;
			break;

		case SHP_AND_J:
			switch(ch)
			{
				case '@':	/* enable user keys, remove */
						/* all labels & status from */
						/* screen 		    */
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					fkl_off(svsp);
					break;

				case 'A':	/* enable & display "modes" */
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					fkl_on(svsp);
					sw_sfkl(svsp);
					break;

				case 'B':	/* enable & display "user"  */
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					fkl_on(svsp);
					sw_ufkl(svsp);
					break;

				case 'C':	/* remove (clear) status line*/
						/* and restore current labels*/
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					fkl_on(svsp);
					break;

				case 'R':	/* enable usr/menu keys */
						/* and fkey label modes */
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					break;

				case 'S':	/* disable usr/menu keys */
						/* and fkey label modes */
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					break;

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': /* parameters for esc & j xx L mm */
					svsp->m_len *= 10;
					svsp->m_len += (ch -'0');
					break;
	
				case 'L':
					svsp->hp_state = SHP_AND_JL;
					svsp->i = 0;
					svsp->transparent = 1;
					break;

				default:
					svsp->hp_state = SHP_INIT;
					svsp->state = STATE_INIT;
					break;

			}
			break;


		case SHP_AND_JL:
			svsp->m_buf[svsp->i] = ch;
			if(svsp->i >= svsp->m_len-1)
			{
				svsp->state = STATE_INIT;
				svsp->hp_state = SHP_INIT;
				svsp->transparent = 0;
				svsp->i = 0;
				svsp->m_buf[svsp->m_len] = '\0';
				/* display status line */
				/* needs to be implemented */
				/* see 2392 man, 3-14 */
			
			}
			else
				svsp->i++;
			break;

		case SHP_AND_ETE:	/* eat chars until uppercase */
			if(ch >= '@' && ch <= 'Z')
			{
				svsp->hp_state = SHP_INIT;
				svsp->state = STATE_INIT;
				svsp->transparent = 0;
			}
			break;
		
		default:
			svsp->hp_state = SHP_INIT;
			svsp->state = STATE_INIT;
			svsp->transparent = 0;
			break;
	}
}

#endif	/* NVT > 0 */

/* ------------------------- E O F ------------------------------------------*/

