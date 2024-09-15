all: gfx.h

gfx.h: *
	python rescomp.py c > gfx.c
	python rescomp.py pal ship_pal ship.png > gfx.h
	python rescomp.py tileset font font.png >> gfx.h
	python rescomp.py sprite ship ship.tmj >> gfx.h
	python rescomp.py sprite enemy enemy.tmj >> gfx.h
	python rescomp.py sprite beam bigcore.tmj >> gfx.h
clean:
	rm -rf gfx.h gfx.c
