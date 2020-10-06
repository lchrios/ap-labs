#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/inotify.h>
#include <string.h>

#define MAXDIR 200
#define MAXLEN 1000
#define BUFFSIZE (10 *(sizeof(struct inotify_event) + MAXLEN + 1))

int inotifyState, op;

struct op {
    char name[MAXLEN];
    int op;
};

struct op op_arr[(10 * (sizeof(struct op) + MAXLEN + 1))];
int op_index = 0;
int op_size = 0;

void createop_arr(const char * name, int watchDescriptor){
    struct op op_struct;
    strcpy(op_struct.name, name);
    op_struct.op = op;
    op_arr[op_index++] = op_struct;
    op_size++;
}

static int add_watcher(const char *path, const struct stat *sb, int type){
    if(type != FTW_D){
        return 0;
    }

    int watchDescriptor;
    if((watchDescriptor = inotify_add_watch(inotifyState, path, IN_CREATE | IN_DELETE | IN_MODIFY)) == -1){
        errorf("Error: failed creating watch descriptor.");
        return -1;
    }
    createoptruct(path, watchDescriptor);
    return 0;
}

char * findWatchDescriptor(int op) {
    int i;
    for (i = 0; i < op_size; i++) {
        if (op_arr[i].op == op) {
            return op_arr[i].name;
        }
    }

    return NULL;
}

static void printEvent(struct inotify_event *i) {
    infof("op: %d detected event: %s in file %s", i->op, 
        i->mask & IN_CREATE ? "IN CREATE" : i->mask & IN_DELETE ? "IN DELETE" :
        i->mask & IN_MODIFY ? "IN MODIFY" : "???", i->name);   
}

int main(int argc, char** argv){
    if(argc < 2){
        errorf("Error: try using ./monitor <path>");
        exit(1);
    }

    const char *path;
    char buf[BUFFSIZE];
    ssize_t nrd;
    char *p;


    struct inotify_event *event;

    // Init inotify
    infof("Initiating inotify");
    path = argv[1];
    inotifyState = inotify_init();


    if(inotifyState == -1){
        errorf("Error: failure at the inotify initialization");
        exit(1);
    } else {    
        infof("Successfully initiated inotify");
    }

    if(ftw(path, add_watcher, MAXDIR) == -1){
        errorf("Error: failed at traversing the file tree.");
        exit(1);
    }

    for(;;){
        if((nrd = read(inotifyState, buf, BUFFSIZE)) == 0){
            errorf("Error: failed reading inotify file descriptor.");
            exit(1);
        }

        if(nrd == -1){
            errorf("Error: failed reading inotify file descriptor.");
            exit(1);
        }

        for(p = buf; p < buf + nrd;){
            event = (struct inotify_event * ) p;
            printEvent(event);
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}