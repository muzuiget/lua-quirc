BASE_SRC = \
	quirc/lib/decode.c \
	quirc/lib/identify.c \
	quirc/lib/quirc.c \
	quirc/lib/version_db.c
LUALIB_SRC = $(BASE_SRC) src/lua-quirc.c
READRAW_SRC = $(BASE_SRC) src/readraw.c
QRDUMP_SRC = $(BASE_SRC) quirc/tests/dbgutil.c src/qrdump.c
LUA_INCLUDE=/usr/include/lua5.3

all: quirc.so readraw.so qrdump

quirc.so: $(LUALIB_SRC)
	gcc -O2 -Wall --shared -fPIC -I$(LUA_INCLUDE) -Iquirc/lib -o $@ $(LUALIB_SRC) -lm

readraw.so: $(READRAW_SRC)
	gcc -O2 -Wall --shared -fPIC -I$(LUA_INCLUDE) -Iquirc/lib -o $@ $(READRAW_SRC) -lm

qrdump: $(QRDUMP_SRC)
	gcc -O2 -Wall -Iquirc/lib -Iquirc/tests -o $@ $(QRDUMP_SRC) -lm -ljpeg -lpng

clean:
	rm -f quirc.so
	rm -f readraw.so
	rm -f qrdump
