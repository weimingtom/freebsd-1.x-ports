#include "LYCurses.h"
#include "HTUtils.h"
#include "LYSignal.h"
#include "LYUtils.h"
#include "LYClean.h"
#include "LYStrings.h"
#include "GridText.h"
#include "LYSystem.h"
#include "LYGlobalDefs.h"

/*
 * mailmsg sends a message to the owner of the
 * file if one is defined telling of errors. (ie link not available)
 */

PUBLIC void mailmsg ARGS4(int,cur, char *,owner_address, 
		char *,filename, char *,linkname)
{
	FILE *fd;
	char cmd[256], *tmptr;
#ifdef VMS
	char tmpfile[80];
	char *address_ptr1, *address_ptr2;
	BOOLEAN first=TRUE;
#endif /* VMS */

	if((tmptr = (char *)strchr(owner_address,'\n')) != NULL)
		*tmptr = '\0';

#ifdef UNIX
#ifdef MMDF
	sprintf(cmd, "%s -mlrxto,cc*",SYSTEM_MAIL);
#else
	sprintf(cmd, "%s -t -oi", SYSTEM_MAIL);
#endif /* MMDF */

	if ((fd = popen(cmd, "w")) == NULL) 
			return;

	fprintf(fd,"To: %s\n", owner_address);
	fprintf(fd,"Subject: Lynx Error in %s\n\n",filename);
	fprintf(fd,"X-URL: %s\n",filename);
	fprintf(fd,"X-Mailer: Lynx, Version %s\n",LYNX_VERSION);
#endif /* UNIX */
#ifdef VMS
	sprintf(tmpfile,"%s%s",TEMP_SPACE, "temp_mail.");
	if((fd = fopen(tmpfile,"w")) == NULL)
	    return;

#endif /* VMS */

	fprintf(fd,"The link   %s :?: %s \n",
                links[cur].lname, links[cur].target);
	fprintf(fd,"called \"%s\"\n",links[cur].hightext);
	fprintf(fd,"in the file \"%s\" called \"%s\"",filename,linkname);

	fputs("\nwas requested but was not available.",fd);
	fputs("\n\nThought you might want to know.",fd);

	fputs("\n\nThis message was automatically generated by\n",fd);
	fprintf(fd,"Lynx ver. %s",LYNX_VERSION);
#ifdef UNIX
	pclose(fd);
#endif /* UNIX */
#ifdef VMS
	fclose(fd);
	sprintf(cmd, "%s /subject=\"Lynx Error in %s\" %s ",SYSTEM_MAIL,
							filename,tmpfile);


	address_ptr1 = owner_address;
	do {
	    if((tmptr = strchr(address_ptr1,',')) != NULL) {
	        address_ptr2=tmptr+1;
	        *tmptr = '\0';
	    } else
		address_ptr2=NULL;
	
	    if(strlen(owner_address) > 3) {
		if(!first)
		    strcat(cmd, ", ");  /* add a comma and a space */
	        sprintf( &cmd[strlen(cmd)], MAIL_ADRS, address_ptr1);
		first = FALSE;
	    }

	    address_ptr1=address_ptr2;
	} while(address_ptr1!=NULL);

	system(cmd);
	remove(tmpfile);
#endif /* VMS */

#ifdef TRAVERSAL
	{
	    int ofp;
	    if((ofp = fopen(TRAVERSE_ERRORS,"a+")) == NULL) {
		perror("unable to open traversal errors output file");
		exit(1);
	    }

	    fprintf(ofp,"%s	%s 	in %s\n",links[cur].lname, 
						links[cur].target, filename);
	    fclose(ofp);
	}
#endif /* TRAVERSAL */

	return;
}

/*
 * this procedure only works on unix & VMS 
 * reply by mail invokes sendmail or mail on VMS to send a comment 
 * from the users to the owner 
 */

/* global variable for async i/o */
BOOLEAN term_letter;
PRIVATE void terminate_letter  PARAMS((int sig));
PRIVATE void remove_tildes PARAMS((char *string));

PUBLIC void reply_by_mail ARGS2(char *,mail_address, char *,filename)
{
	char subject[256];
	char user_input[1000];
	FILE *fd;
	char c;  /* user input */
	char tmpfile[100], cmd[256];
#ifdef VMS
	char *tmptr, *address_ptr1, *address_ptr2;
	BOOLEAN first=TRUE;
#endif /* VMS */

	term_letter=FALSE;

	clear();
	move(2,0);

	tempname(tmpfile,NEW_FILE);
	if((fd = fopen(tmpfile,"w")) == NULL)
	    return;

	addstr("You are now sending a comment to:");
	addstr("\n	");
	if(*mail_address != '\0') {
	    addstr(mail_address);
	}
	addstr("\n\nUse Ctrl-G to cancel if you do not want to send a message\n\n");

	/* Use ^G to cancel mailing of comment */
	/* and don't let sigints exit lynx     */
        signal(SIGINT, terminate_letter);

#ifdef UNIX
	/* put the to: line in the temp file */
	fprintf(fd,"To: %s\n", mail_address);
	fprintf(fd,"X-URL: %s\n",filename);
	fprintf(fd,"X-Mailer: Lynx, Version %s\n",LYNX_VERSION);
#endif /* UNIX */


	addstr(" Please enter your name, or leave it blank if you wish to remain anonymous\n");
	strcpy(user_input,"Personal_Name: ");
	if (LYgetstr(user_input, VISIBLE) < 0 || term_letter) {
	    statusline("Comment request cancelled!!!");
	    fclose(fd);  /* close the temp file */
	    sleep(1);
	    goto cleanup;
	}

	remove_tildes(user_input);

	term_letter=FALSE;
	fprintf(fd,"%s\n",user_input);

	addstr("\n\n Please enter a mail address or some other\n");
	addstr(" means to contact you, if you desire a response.\n");
	/* add the mail address if there is one */
	sprintf(user_input,"From: %s",personal_mail_address);

	if (LYgetstr(user_input, VISIBLE) < 0 || term_letter) {
	    statusline("Comment request cancelled!!!");
	    sleep(1);
	    fclose(fd);  /* close the temp file */
	    goto cleanup;
	}
	remove_tildes(user_input);

	fprintf(fd,"%s\n",user_input);

	addstr("\n\n Please enter a subject line\n");
	strcpy(user_input,"Subject: ");
	if (LYgetstr(user_input, VISIBLE) < 0 || term_letter) {
	    statusline("Comment request cancelled!!!");
	    sleep(1);
	    fclose(fd);  /* close the temp file */
	    goto cleanup;
	}
	if(*filename == '\0')
	    strncpy(subject, user_input,255);

	remove_tildes(user_input);

	fprintf(fd,"%s\n\n",user_input);

	if(!no_editor && *editor != '\0') {
            /* ask if the user wants to include the original message */
            statusline("Do you wish to inlude the original message? (y/n) ");
            c=0;
            while((c = toupper(LYgetch())) != 'Y' && c != 'N')
                ; /* null body */
            if(toupper(c) == 'Y')
                /* the 1 will add the reply ">" in front of every line */
                print_wwwfile_to_fd(fd,1);
        
	    fclose(fd);

	    /* spawn the users editor on the mail file */
	    sprintf(user_input,"%s %s",editor,tmpfile);
	    statusline("Spawning your selected editor to edit mail message");
	    stop_curses();
	    if(system(user_input)) {
	        start_curses();
		statusline("Error spawning editor, check your editor definition in the options menu");
	  	sleep(1);
	    } else {
	        start_curses();
	    }

	} else {
	
	    addstr("\n\n Please enter your message below.");
	    addstr("\n When you are done, press enter and put a single period (.)");
	    addstr("\n on a line and press enter again.");
	    addstr("\n\n");
	    scrollok(stdscr,TRUE);
	    refresh();
    	    *user_input = '\0';
	    if (LYgetstr(user_input, VISIBLE) < 0 || term_letter) {
	        statusline("Comment request cancelled!!!");
	        sleep(1);
	        fclose(fd);  /* close the temp file */
	        goto cleanup;
	    }


	    while(!STREQ(user_input,".") && !term_letter) { 
	       addch('\n');
	       remove_tildes(user_input);
	       fprintf(fd,"%s\n",user_input);
	       *user_input = '\0';
	       if (LYgetstr(user_input, VISIBLE) < 0) {
	          statusline("Comment request cancelled!!!");
	          sleep(1);
	          fclose(fd);  /* close the temp file */
	          goto cleanup;
	       }
	    }

	    fprintf(fd,"\n");

	    fclose(fd);  /* close the temp file */
	    scrollok(stdscr,FALSE);  /* stop scrolling */
	}

	statusline("Send this comment? (y/n) ");
	while((c = toupper(LYgetch())) != 'Y' && c != 'N')
	    ; /* null body */

	clear();  /* clear the screen */

	if(c == 'N') {
	   goto cleanup;
	}

#ifdef VMS
	sprintf( cmd, "%s /subject=\"Commenting from file %s\" %s ",
		  SYSTEM_MAIL, filename, tmpfile);

	/* now add all the people in the mail_address field */
	address_ptr1 = mail_address;
	do {
	    if((tmptr = strchr(address_ptr1,',')) != NULL) {
	        address_ptr2= (tmptr+1);
	        *tmptr = '\0';
	    } else
		address_ptr2=NULL;
	    
	    /* 4 letters is arbitrarily the smallest posible mail address,
	     * at least for lynx.  That way extra spaces won't
	     * confuse the mailer and give a blank address
	     */
	    if(strlen(address_ptr1) > 3) {	
	        if(!first)
		    strcat(cmd, ", ");  /* add a comma and a space */
	        sprintf( &cmd[strlen(cmd)], MAIL_ADRS, address_ptr1);
		first=FALSE;
	    }

	    address_ptr1=address_ptr2;
	} while(address_ptr1!=NULL);

        stop_curses();
	printf("Sending your comment:\n\n$ %s\n\nPlease wait...", cmd);
	system(cmd);
	sleep(1);
	start_curses();
	goto cleandown;
#else
	/* send the tmpfile into sendmail.  
	 */
	statusline("Sending your message....");
#ifdef MMDF
	sprintf(cmd, "%s -mlrxto,cc* < %s",SYSTEM_MAIL, tmpfile);
#else
	sprintf(cmd,"%s -t -oi < %s", SYSTEM_MAIL, tmpfile);
#endif /* MMDF */

	signal(SIGINT, SIG_IGN);
	system(cmd);

	if(TRACE)
	    printf("%s\n",cmd);

#endif /* VMS */

	/* come here to cleanup and exit */
cleanup:
	signal(SIGINT, cleanup_sig);
cleandown:
	term_letter = FALSE;

	scrollok(stdscr,FALSE);  /* stop scrolling */
	remove(tmpfile);
}

PRIVATE void terminate_letter ARGS1(int,sig)
{
	term_letter=TRUE;
#ifdef VMS
	/* Reassert the AST */
	signal(SIGINT, terminate_letter);
        /* Refresh the screen to get rid of the "interrupt" message */
	if (!dump_output_immediately) {
	    clearok(curscr, TRUE);
	    refresh();
	}
#endif /* VMS */
}

PRIVATE void remove_tildes ARGS1(char *,string)
{
       /* change the first character to a space if
	* it is a '~'
	*/
    if(*string == '~')
	*string = ' ';
}
