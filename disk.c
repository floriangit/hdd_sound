#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/def.h>

/* WAV files */
#include "short.h"
#include "medium.h"

#define WAV_HEADER_LEN (44)

enum {
    SHORT,
    MEDIUM
};

static int play(int mode) {
 
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 48000,
        .channels = 1
    };

    static pa_buffer_attr ba = {
	.maxlength = -1,
	.tlength = -1,
	.prebuf = 320,
	.minreq = -1,
	.fragsize = -1,
    };
 
    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, "unused", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, &ba, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    switch(mode) {
        int ret;
        case SHORT:
	    ret = pa_simple_write(s, (out_short_clean_wav + WAV_HEADER_LEN), out_short_clean_wav_len - WAV_HEADER_LEN, &error);
            break;
        case MEDIUM:
	    ret = pa_simple_write(s, (out_clean_wav + WAV_HEADER_LEN), out_clean_wav_len - WAV_HEADER_LEN, &error);
            break;
        default:
            break;
	if(ret < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
	}

    }
 
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    ret = 0;
 
finish:
    if (s)
        pa_simple_free(s);
 
    return ret;
}

int main(int ac, char **av)
{
	char str[4096];
	char fstat[512];
	int fd;
	int ret;
	int val      = 0;
	int old_val  = 1;
	int diff     = 0;

	if(ac > 1) {
	    sprintf(fstat, "/sys/block/%s/stat", av[1]);
	} else {
	    strcpy(fstat, "/sys/block/sda/stat");
	}

	while(1) {
		usleep(1234);
		fd = open(fstat, O_RDONLY);
	
		if(fd < 3) {
			perror("open");
			return -1;
		}
	
		ret = read(fd, str, sizeof(str));
		if(ret == -1) {
			perror("read");
			return -1;
		}
		close(fd);

		char delim[] = " ";
		char *ptr = strtok(str, delim);
	
		for(int i = 0; ptr != NULL; i++) {
			if(i == 2 || i == 6) { /* 2 = READ, 6 = WRITE */
				val = atoi(ptr);
				diff = val - old_val;
				if(diff > 1024) {
					play(MEDIUM);
					old_val = val;
					break;
				}
				if(diff > 0) {
					play(SHORT);
					old_val = val;
					break;
				}
			}
			ptr = strtok(NULL, delim);
		}
	}
	return 0;
}
