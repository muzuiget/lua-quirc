BASE_SRC = \
	quirc/lib/decode.c \
	quirc/lib/identify.c \
	quirc/lib/quirc.c \
	quirc/lib/version_db.c
LUALIB_SRC = $(BASE_SRC) src/lua-quirc.c
READRAW_SRC = $(BASE_SRC) src/readraw.c
QRDUMP_SRC = $(BASE_SRC) quirc/tests/dbgutil.c src/qrdump.c

all: quirc.so readraw.so qrdump

quirc.so: $(LUALIB_SRC)
	gcc -Wall -g --shared -fPIC -Iquirc/lib/ -o $@ -lm $(LUALIB_SRC)

readraw.so: $(READRAW_SRC)
	gcc -Wall -g --shared -fPIC -Iquirc/lib/ -o $@ -lm $(READRAW_SRC)

qrdump: $(QRDUMP_SRC)
	gcc -Wall -g -Iquirc/lib/ -Iquirc/tests/ -o $@ -lm -ljpeg -lpng $(QRDUMP_SRC)

clean:
	rm -f quirc.so
	rm -f readraw.so
	rm -f qrdump
