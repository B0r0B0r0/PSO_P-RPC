CC = gcc


LIBS_DUPLICATE = -L . -lStergereDuplicate -lpthread
LIBS_HASH = -L . -lmyhash -lpthread
LIBS_BASE64 = -L . -lmyBase64 -lm -lpthread
LIBS_FRECVENTA = -L . -lmyFFrecventa -lm -lpthread
LIBS_RSA= -L . -lmyrsa -lm -pthread

SRCS_DUPLICATE = proces2.c
SRCS_HASH = proces1.c
SRCS_BASE64 = proces3.c
SRCS_FRECVENTA = proces4.c
SRCS_RSA = proces5.c

OBJS_DUPLICATE = $(SRCS_DUPLICATE:.c=.o)
OBJS_HASH = $(SRCS_HASH:.c=.o)
OBJS_BASE64 = $(SRCS_BASE64:.c=.o)
OBJS_FRECVENTA = $(SRCS_FRECVENTA:.c=.o)
OBJS_RSA = $(SRCS_RSA:.c=.o)	

TARGET_DUPLICATE = proces2
TARGET_HASH = proces1
TARGET_BASE64 = proces3
TARGET_FRECVENTA = proces4
TARGET_RSA = proces5

all: $(TARGET_DUPLICATE) $(TARGET_HASH) $(TARGET_BASE64) $(TARGET_FRECVENTA) $(TARGET_RSA)

$(TARGET_DUPLICATE): $(OBJS_DUPLICATE)
	$(CC)  -o $@ $^ $(LIBS_DUPLICATE)

$(TARGET_HASH): $(OBJS_HASH)
	$(CC)  -o $@ $^ $(LIBS_HASH)

$(TARGET_BASE64): $(OBJS_BASE64)
	$(CC)  -o $@ $^ $(LIBS_BASE64)

$(TARGET_FRECVENTA): $(OBJS_FRECVENTA)
	$(CC)  -o $@ $^ $(LIBS_FRECVENTA)

$(TARGET_RSA): $(OBJS_RSA)
	$(CC)  -o $@ $^ $(LIBS_RSA)

Per_Process: Per_Process.c
	$(CC) -c Per_Process.c

clean:
	rm -f $(OBJS_DUPLICATE) $(OBJS_HASH) $(OBJS_BASE64) $(OBJS_FRECVENTA) $(OBJS_RSA) $(TARGET_DUPLICATE) $(TARGET_HASH) $(TARGET_BASE64) $(TARGET_FRECVENTA) $(TARGET_RSA)
