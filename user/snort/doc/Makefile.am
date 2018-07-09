## $Id: Makefile.am,v 1.9 2003/05/16 20:01:26 chrisgreen Exp $
AUTOMAKE_OPTIONS=foreign no-dependencies

EXTRA_DIST = AUTHORS NEWS README.csv BUGS README README.database \
CREDITS README.FLEXRESP RULES.todo FAQ \
SnortUsersManual.pdf INSTALL README.PLUGINS USAGE snortman.tex \
signatures/${*.txt}

doc_DIR = $(prefix)/doc

SUFFIXES = .tex .dvi .ps

.tex.dvi:
	pslatex $< && pslatex $< && pslatex $<

.dvi.ps:
	dvips $< -o

.ps.pdf:
	ps2pdf $<
