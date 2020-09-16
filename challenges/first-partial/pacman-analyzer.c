#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define REPORT_FILE "packages_report.txt"
#define SIZE 1000

struct Package_info
{
    int key, 
        upgN;
    char *name,
         *insDate,
         *upgDate,
         *remDate;
};

struct Resume
{
    int installed,
        upgraded,
        removed,
        current;
};



int lastItem = 0;
struct Package_info *packages[SIZE], 
                *dummy;

    struct Resume *results;                


void analizeLog(char *logFile, char *report);
struct Package_info *search(int key);
struct Package_info* searchByName(char* name);
void insert(char *name, char *insDate, char *upgDate, char *remDate, int upgN);
size_t getln(char *line, size_t size, FILE *fp);
struct Package_info* delete(struct Package_info pkg);
void showPkgInfo(int i, FILE *fw);

int main(int argc, char **argv) {

    if (argc == 5 && strcmp(argv[1], "-input") == 0 && strcmp(argv[3], "-output") == 0) {
        analizeLog(argv[2], REPORT_FILE);
    } else if (argc == 3 && strcmp(argv[1], "-input") == 0) {
        analizeLog(argv[2], argv[4]);
    } else {
        printf("Usage:./pacman-analizer.o \n");
        return 1;
    }

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.


    // configs
    //dummy->key = NULL;

    dummy = (struct Package_info *) malloc(sizeof(struct Package_info));
    results = (struct Resume *) malloc(sizeof(struct Resume));

    /*for (int i = 0; i < SIZE; i++) {
        packages[i] = (struct Package_info *) malloc(sizeof(struct Package_info));
    }*/

    // open file
    FILE *fp;
    fp = fopen(logFile, "r");
    
    // read line
    int n = 0;
    size_t line_size  = 512;
    char line[line_size];
    bool isFstFile = true; 

    while(getln(line, line_size, fp) != -1) {
        if (*line == EOF) {
            break;
        }
        if (n == 0 && 'T' == line[11]) {
            isFstFile = false;
        }
        
        char ndate[10] = "", nhour[8] = "", naction[100] = "", nname[100] = "";
        char *token, *date = ndate, *hour = nhour, *action = naction, *name = nname;

        char *debug = "";

        // pacman.txt format
        if (isFstFile) {
            
            
            // date storing
            token = strtok(line, "[] ");            
            strcat(date, token);
            printf("line : %d   date: %s   ", n, token);

            // hour storing
            token = strtok(NULL, " ]");
            strcat(hour, token);
            hour[5] = '\0';
            printf("hour: %s   ", token);
        } 
        // pacman2.txt format
        else {
            
            // date storing
            token = strtok(line, "[]T+ ");
            strcat(date, token);
            printf("line: %d   date: %s   ", n, date);

            // hour storing
            token = strtok(NULL, "T+ ");
            strcat(hour, token);
            hour[4] = '\0';
            printf("hour: %s", hour);



        }

            // action storing
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            strcat(action, token);
            //printf("action: %s   ", action);

        // target storing
        if (strcmp(action, "installed") == 0) {

            token = strtok(NULL, " ");
            if (token != NULL) {
                strcat(name, token);
            }
            printf("action: %s   target: %s\n", action, name);

            //insert(name, date, date, "-", 0);

            results->installed = results->installed + 1;

        } else if (strcmp(action, "upgraded") == 0) {

            token = strtok(NULL, " ");
            if (token != NULL) {
                strcat(name, token);
            }
            printf("action: %s   target: %s\n", action, name);

            struct Package_info *pkg = searchByName(name);
            /*++pkg->upgN;
            strcpy(pkg->upgDate, date);
            strcat(pkg->upgDate, hour);*/

            results->upgraded += 1;

        } else if (strcmp(action, "removed") == 0) {

            token = strtok(NULL, " ");
            if (token != NULL) {
                strcat(name, token);
            }
            
            printf("action: %s   target: %s\n", action, name);

            //delete(*searchByName(name));
        
            results->removed += 1;

        } else {
            printf("\n");
            ++n;
            if (n == 4267 || (n == 3862 && isFstFile))
                break;
            continue;
        }
         
        date[10] = '\0';
        //printf("Line: %d   Date: %s %s   Action: %s   Target: %s\n", n, date, hour, action, name);
        ++n;
        if (n == 4267 || (n == 3862 && isFstFile))
            break;
    } 



    // analyze data
    // show data
    // write data
    FILE *fw = fopen(report, "w");

    results->current = results->installed - results->removed;
    fprintf(fw, "Pacman Packages Report\n----------------------\n");
    fprintf(fw, "- Installed packages : %d", results->installed);
    fprintf(fw, "- Removed packages   : %d", results->removed);
    fprintf(fw, "- Upgraded packages  : %d", results->upgraded);
    fprintf(fw, "- Current installed  : &d", results->current);
    for (int i = 0; i < SIZE; i++) {
        showPkgInfo(i, fw);
    }
    

    printf("Report is generated at: [%s]\n", report);
    
    fclose(fp);
    fclose(fw);
}









int hashCode(int key) {
    return key % SIZE;
}

struct Package_info *search(int key) {
    int hInd = hashCode(key);

    while(packages[hInd] != NULL) {
        
        if (packages[hInd]->key == key) {
            return packages[hInd];
        }

        ++hInd;

        hInd %= SIZE;

    }

    return NULL;
}

struct Package_info *searchByName(char* name) {
    int hInd = 0;

    while (packages[hInd] != NULL) {
        if (strcmp(packages[hInd]->name, name)){
            return packages[hInd];
        }

        ++hInd;

        hInd % SIZE;
    }

    return NULL;
}

void insert(char *name, char *insDate, char *upgDate, char *remDate, int upgN) {

    //struct Package_info *pkg = (struct Package_info*) malloc(sizeof(struct Package_info)) = ;
    int key = hashCode(lastItem);
    int hInd = hashCode(key);

    //packages[hInd] = (struct Package_info*) malloc(sizeof(struct Package_info));
    *packages[hInd] = (struct Package_info) {.key = key, .upgN = upgN, .name = name, .insDate = insDate, .upgDate = upgDate, .remDate = remDate};
    /*pkg->key = key;
    pkg->name = name;
    pkg->insDate = insDate;
    pkg->upgDate = upgDate;
    pkg->remDate = remDate;
    pkg->upgN = upgN;*/


    while (packages[hInd] != NULL && packages[hInd]->key != -1) {
        ++hInd;
        hInd %= SIZE;
    }
    ++lastItem;

}

struct Package_info* delete(struct Package_info pkg) {
    int key = pkg.key;

    int hInd = hashCode(key);

    while(packages[hInd] != NULL) {
        if (packages[hInd]->key == key){
            struct Package_info* tmp = packages[hInd];

            packages[hInd] = dummy;
            return tmp;
        }

        ++hInd;

        hInd %= SIZE;
    }

    return NULL;
}

void showPkgInfo(int i, FILE *fw) {
    fprintf(fw, "- Package name:        %s\n", packages[i]->name);
    fprintf(fw, "  - Install date:      %s\n", packages[i]->insDate);
    fprintf(fw, "  - Last update date:  %s\n", packages[i]->upgDate);
    fprintf(fw, "  - How many updates:  %s\n", packages[i]->upgN);
    fprintf(fw, "  - Removal date:      %s\n", packages[i]->remDate);
}

size_t getln(char *line, size_t size, FILE *fp) {

    if (size == 0)
        return 0;

    size_t count;
    int c = 0;
    for (count = 0; c != '\n' && count < size - 1; count++) {
        c = getc(fp);

        if (c == EOF) {
            if (count == 0)
                fclose(fp);
                return -1;
                break;
        }

        line[count] = (char) c;
    }

    line[count] = '\0';
    return (size_t) count;
}
