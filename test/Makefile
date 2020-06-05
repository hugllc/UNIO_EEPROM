BASEDIR:= $(abspath ../)

TEST_OBJECTS:=main.o test_unio_eeprom.o UNIO_EEPROM.o

HEADER_FILES:=main.h
TEST_TARGET:=UNIO_EEPROM
TEST_NAME:=

TARGET:=HUGnetNGDev


CFLAGS_TARGET+=-fprofile-arcs -ftest-coverage -Wall -Werror -Wextra -Wno-unused-parameter -std=gnu++11 -gdwarf-2
CFLAGS_TARGET+=-Werror=float-equal
CFLAGS+=-D_TEST_ \
        -m32 \
        -I$(shell pwd) \
        -I$(BASEDIR) \
        -DPROGMEM= \
		-DEEPROM_SIZE=128 \
        -fsanitize=address

CFLAGS_TEST+= -fprofile-arcs -ftest-coverage -Wall -Werror -Wextra -Wno-unused-parameter -gdwarf-2
CFLAGS_TEST+= -Werror=float-equal
LDFLAGS+=
GPP:=g++ $(CFLAGS)

ifeq ($(INTERACTIVE),1)
    CFLAGS_TEST += -DINTERACTIVE
endif

VPATH = $(BASEDIR) $(abspath $(shell pwd))

all: clean test coverage

coverage: run_test junit

interactive:
	$(MAKE) test INTERACTIVE=1

test: run_test
	./run_test -l standard

junit: run_test
	@echo "Test output is in $(TEST_TARGET)$(TEST_NAME)-Results.xml"
	rm -f *-Results.xml
	-./run_test -l junit > $(TEST_TARGET)$(TEST_NAME)-Results.xml

run_test: $(TEST_OBJECTS) $(HUGNETCANMOCK_OBJECTS) $(HEADER_FILES) $(HUGNETCANMOCK_HEADER_FILES)
	$(GPP) $(LDFLAGS) $(CFLAGS_TARGET) -o $@ $(TEST_OBJECTS) $(HUGNETCANMOCK_OBJECTS)

$(TARGET).o : $(TARGET).cpp $(TARGET).h
	$(GPP) $(CFLAGS_TARGET) -c $< -o $@

%.o : %.cpp %.h
	$(GPP) $(CFLAGS_TEST) -c $< -o $@

%.o : %.cpp
	$(GPP) $(CFLAGS_TEST) -c $< -o $@

clean:
	rm -f *~ *.o run_test *.gcda *.gcno *Results.xml *.orig

distclean: clean
