ifeq ($(shell uname -s),Darwin)
	CFLAGS = -std=gnu17 -pthread -Wall -O0 -pipe -fno-plt -fPIC -I. -I/opt/homebrew/include
	LDFLAGS = -pthread -L$(shell brew --prefix)/lib -largp
else
	CFLAGS = -std=gnu17 -pthread -Wall -O0 -pipe -fno-plt -fPIC -I.
	LDFLAGS = -lrt -pthread -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now
endif
TSAN = -fsanitize=thread -fsanitize=undefined -g3 -Og
ASAN = -fsanitize=address -fsanitize=undefined -g3 -Og
FIX_FOR_SEASNET_LINKER_ISSUES_COMMON_SEASNET_L = -Wl,--rpath=/usr/local/cs/lib64 -L/usr/local/cs/lib64
# ##########################################################
# Run `make SAN=tsan` to detect data races, `make SAN=asan` to detect memory leaks
#
# Thanks to Vincent Lin for writing the conditional section below!
# His Piazza comment: https://piazza.com/class/lcjl27z4agp66l/post/315_f3
ifeq ($(SAN),tsan)
  SANITIZE = $(TSAN)
else ifeq ($(SAN),asan)
  SANITIZE = $(ASAN)
endif
# ##########################################################
CFLAGS += $(SANITIZE)
LDFLAGS += $(SANITIZE) $(FIX_FOR_SEASNET_LINKER_ISSUES_COMMON_SEASNET_L)

OBJS = \
  hash-table-common.o \
  hash-table-base.o \
  hash-table-v1.o \
  hash-table-v2.o \
  hash-table-tester.o

FILENAME=README.md
UID = $(shell cat ${FILENAME} | grep '## UID'| grep -oe '\([0-9.]*\)')
SUBMISSION_FILES = hash-table-v1.c hash-table-v2.c README.md
.PHONY: all
all: hash-table-tester

hash-table-tester: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
tar:
	@tar -cf ${UID}-lab3-submission.tar ${SUBMISSION_FILES}

.PHONY: clean
clean:
	rm -f $(OBJS) hash-table-tester
