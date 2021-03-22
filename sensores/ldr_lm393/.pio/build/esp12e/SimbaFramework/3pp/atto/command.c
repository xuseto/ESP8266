/*
 * command.c, Atto Emacs, Hugh Barney, Public Domain, 2015
 * Derived from: Anthony's Editor January 93, (Public Domain 1991, 1993 by Anthony Howe)
 */

#include "header.h"

void top()
{
	curbp->b_point = 0;
	curwp->w_point = curbp->b_point;
}

void bottom()
{
	curbp->b_epage = curbp->b_point = pos(curbp, curbp->b_ebuf);
	curwp->w_point = curbp->b_point;
}

void quit_ask()
{
	if (modified_buffers() > 0) {
		mvaddstr(MSGLINE, 0, "Modified buffers exist; really exit (y/n) ?");
		clrtoeol();
		if (!yesno(FALSE))
			return;
	}
	quit();
}

/* flag = default answer, FALSE=n, TRUE=y */
int yesno(int flag)
{
	int ch;

	addstr(flag ? " y\b" : " n\b");
	refresh();
	ch = getch();
	if (ch == '\r' || ch == '\n')
		return (flag);
	return (tolower(ch) == 'y');
}

void quit()
{
	done = 1;
}

void redraw()
{
	window_t *wp;
	
	clear();
	for (wp=wheadp; wp != NULL; wp = wp->w_next)
		wp->w_update = TRUE;
	update_display();
}

void left()
{
	if (0 < curbp->b_point)
		--curbp->b_point;
}

void right()
{
	if (curbp->b_point < pos(curbp, curbp->b_ebuf))
		++curbp->b_point;
}

void up()
{
	curbp->b_point = lncolumn(curbp, upup(curbp, curbp->b_point),curbp->b_col);
}

void down()
{
	curbp->b_point = lncolumn(curbp, dndn(curbp, curbp->b_point),curbp->b_col);
}

void lnbegin()
{
	curbp->b_point = segstart(curbp, lnstart(curbp,curbp->b_point), curbp->b_point);
}

void lnend()
{
	curbp->b_point = dndn(curbp, curbp->b_point);
	left();
}

void wleft()
{
	char_t *p;
	while (!isspace(*(p = ptr(curbp, curbp->b_point))) && curbp->b_buf < p)
		--curbp->b_point;
	while (isspace(*(p = ptr(curbp, curbp->b_point))) && curbp->b_buf < p)
		--curbp->b_point;
}

void pgdown()
{
	curbp->b_page = curbp->b_point = upup(curbp, curbp->b_epage);
	while (0 < curbp->b_row--)
		down();
	curbp->b_epage = pos(curbp, curbp->b_ebuf);
}

void pgup()
{
	int i = curwp->w_rows;
	while (0 < --i) {
		curbp->b_page = upup(curbp, curbp->b_page);
		up();
	}
}

void wright()
{
	char_t *p;
	while (!isspace(*(p = ptr(curbp, curbp->b_point))) && p < curbp->b_ebuf)
		++curbp->b_point;
	while (isspace(*(p = ptr(curbp, curbp->b_point))) && p < curbp->b_ebuf)
		++curbp->b_point;
}

void insert()
{
	ASSERTRV(curbp->b_gap <= curbp->b_egap);
	if (curbp->b_gap == curbp->b_egap && !growgap(curbp, CHUNK))
		return;
	curbp->b_point = movegap(curbp, curbp->b_point);

	/* overwrite if mid line, not EOL or EOF, CR will insert as normal */
	if ((curbp->b_flags & B_OVERWRITE) && input != '\r' && *(ptr(curbp, curbp->b_point)) != '\n' && curbp->b_point < pos(curbp,curbp->b_ebuf) ) {
		*(ptr(curbp, curbp->b_point)) = input;
		if (curbp->b_point < pos(curbp, curbp->b_ebuf))
			++curbp->b_point;
	} else {
		*curbp->b_gap++ = input == '\r' ? '\n' : input;
		curbp->b_point = pos(curbp, curbp->b_egap);
	}
	curbp->b_flags |= B_MODIFIED;
}

void backsp()
{
	curbp->b_point = movegap(curbp, curbp->b_point);
	undoset();
	if (curbp->b_buf < curbp->b_gap) {
		--curbp->b_gap;
		curbp->b_flags |= B_MODIFIED;
	}
	curbp->b_point = pos(curbp, curbp->b_egap);
}

void delete()
{
	curbp->b_point = movegap(curbp, curbp->b_point);
	undoset();
	if (curbp->b_egap < curbp->b_ebuf) {
		curbp->b_point = pos(curbp, ++curbp->b_egap);
		curbp->b_flags |= B_MODIFIED;
	}
}

void gotoline()
{
	temp[0] = '\0';
	int line;
	point_t p;
	result = getinput("Goto line: ", (char*)temp, STRBUF_S);

	if (temp[0] != '\0' && result) {
		line = atoi(temp);
		p = line_to_point(line);
		if (p != -1) {
			curbp->b_point = p;
			msg("Line %d", line);
		} else {
			msg("Line %d, not found", line);
		}
	}
}

void insertfile()
{
	temp[0] = '\0';
	result = getinput("Insert file: ", (char*) temp, NAME_MAX);
	if (temp[0] != '\0' && result)
		(void) insert_file(temp, TRUE);
}

void readfile()
{
	buffer_t *bp;
	
	temp[0] = '\0';

	/* result = getfilename("Find file: ", (char*)temp, NAME_MAX); */
        result = getinput("Find file: ", (char*)temp, NAME_MAX);

	if (result) {
		bp = find_buffer(temp, TRUE);
		disassociate_b(curwp); /* we are leaving the old buffer for a new one */
		curbp = bp;
		associate_b2w(curbp, curwp);

		/* load the file if not already loaded */
		if (bp != NULL && bp->b_fname[0] == '\0') {
			if (!load_file(temp)) {
				msg("New file %s", temp);
			}
			strncpy(curbp->b_fname, temp, NAME_MAX);
			curbp->b_fname[NAME_MAX] = '\0'; /* truncate if required */
		}
	}
}

void savebuffer()
{
	if (curbp->b_fname[0] != '\0') {
		save(curbp->b_fname);
		return;
	} else {
		writefile();
	}
	refresh();
}

void writefile()
{
	strncpy(temp, curbp->b_fname, NAME_MAX);
	result = getinput("Write file: ", (char*)temp, NAME_MAX);
	if (temp[0] != '\0' && result)
		if (save(temp) == TRUE)
			strncpy(curbp->b_fname, temp, NAME_MAX);
}

void killbuffer()
{
	buffer_t *kill_bp = curbp;
	buffer_t *bp;
	int bcount = count_buffers();

	/* do nothing if only buffer left is the scratch buffer */
	if (bcount == 1 && 0 == strcmp(get_buffer_name(curbp), "*scratch*"))
		return;
	
	if (curbp->b_flags & B_MODIFIED) {
		mvaddstr(MSGLINE, 0, "Discard changes (y/n) ?");
		clrtoeol();
		if (!yesno(FALSE))
			return;
	}

	if (bcount == 1) {
		/* create a scratch buffer */
		bp = find_buffer("*scratch*", TRUE);
		strcpy(bp->b_bname, "*scratch*");
	}

	next_buffer();
	ASSERTRV(kill_bp != curbp);
	delete_buffer(kill_bp);
}

void iblock()
{
	block();
	msg("Mark set");
}

void block()
{
	curbp->b_mark = curbp->b_point;
}

void toggle_overwrite_mode() {
	if (curbp->b_flags & B_OVERWRITE)
		curbp->b_flags &= ~B_OVERWRITE;
	else
		curbp->b_flags |= B_OVERWRITE;
}

void killtoeol()
{
	/* point = start of empty line or last char in file */
	if (*(ptr(curbp, curbp->b_point)) == 0xa || (curbp->b_point + 1 == ((curbp->b_ebuf - curbp->b_buf) - (curbp->b_egap - curbp->b_gap))) ) {
		delete();
	} else {
		curbp->b_mark = curbp->b_point;
		lnend();
		copy_cut(TRUE);
	}
}

void copy() {
	copy_cut(FALSE);
}

void cut() {
	copy_cut(TRUE);
}

void copy_cut(int cut)
{
	char_t *p;
	/* if no mark or point == marker, nothing doing */
	if (curbp->b_mark == NOMARK || curbp->b_point == curbp->b_mark)
		return;
	if (scrap != NULL) {
		FREE(scrap);
		scrap = NULL;
	}
	if (curbp->b_point < curbp->b_mark) {
		/* point above marker: move gap under point, region = marker - point */
		p = ptr(curbp, curbp->b_point);
		(void) movegap(curbp, curbp->b_point);
		nscrap = curbp->b_mark - curbp->b_point;
	} else {
		/* if point below marker: move gap under marker, region = point - marker */
		p = ptr(curbp, curbp->b_mark);
		(void) movegap(curbp, curbp->b_mark);
		nscrap = curbp->b_point - curbp->b_mark;
	}
	if ((scrap = (char_t*) MALLOC(nscrap)) == NULL) {
		msg("No more memory available.");
	} else {
		undoset();
		(void) memcpy(scrap, p, nscrap * sizeof (char_t));
		if (cut) {
			curbp->b_egap += nscrap; /* if cut expand gap down */
			curbp->b_point = pos(curbp, curbp->b_egap); /* set point to after region */
			curbp->b_flags |= B_MODIFIED;
			msg("%ld bytes cut.", nscrap);
		} else {
			msg("%ld bytes copied.", nscrap);
		}
		curbp->b_mark = NOMARK;  /* unmark */
	}
}

void paste()
{
	if(curbp->b_flags & B_OVERWRITE)
		return;
	if (nscrap <= 0) {
		msg("Scrap is empty.  Nothing to paste.");
	} else if (nscrap < curbp->b_egap - curbp->b_gap || growgap(curbp, nscrap)) {
		curbp->b_point = movegap(curbp, curbp->b_point);
		undoset();
		memcpy(curbp->b_gap, scrap, nscrap * sizeof (char_t));
		curbp->b_gap += nscrap;
		curbp->b_point = pos(curbp, curbp->b_egap);
		curbp->b_flags |= B_MODIFIED;
	}
}

void showpos()
{
	int current, lastln;
	point_t end_p = pos(curbp, curbp->b_ebuf);
    
	get_line_stats(&current, &lastln);

	if (curbp->b_point == end_p) {
		msg("[EOB] Line = %d/%d  Point = %d/%d", current, lastln,
			curbp->b_point, ((curbp->b_ebuf - curbp->b_buf) - (curbp->b_egap - curbp->b_gap)));
	} else {
		msg("Char = %s 0x%x  Line = %d/%d  Point = %d/%d", unctrl(*(ptr(curbp, curbp->b_point))), *(ptr(curbp, curbp->b_point)), 
			current, lastln,
			curbp->b_point, ((curbp->b_ebuf - curbp->b_buf) - (curbp->b_egap - curbp->b_gap)));
	}
}

void version()
{
	msg(ATTO_VERSION);
}

char* get_temp_file()
{
#if ATTO_CONFIG_TAB_COMPLETION == 1
	static char temp_file[] = TEMPFILE;
	strcpy(temp_file, TEMPFILE);

	if (-1 == mkstemp(temp_file)) {
            fatal("%s: Failed to create temp file\n");
            return (NULL);
        }

	return temp_file;
#else
        return (NULL);
#endif
}
