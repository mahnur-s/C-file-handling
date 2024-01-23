#include "file.h"


/*
    List files in the current directory.
    `ls` is invoked without any arguments and lists the files in the current working directory.
*/
void ls(){
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        perror("sorry couldn't enlist directory");
        return;
    }
    int FirstFileFlag = 1;  
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (!FirstFileFlag) {
            } else {
                FirstFileFlag = 0;
            }
            printf("%s  ", entry->d_name);
        }
    }
    printf("\n");  
    closedir(dir);
}

/*
    Create file 
    Be careful when you set permission about your file.
*/
void create_file(char* filename) {
    if (access(filename, F_OK) != -1) {
        // File already exists, ignore the command
        return;
    }

    int fd = creat(filename, 0666);

    if (fd != -1) {
        close(fd);
    }
}


/*
    Read 'size' bytes in file and print it.
    If there is no such file, print ("error occured\n").
*/
void read_file(char* filename, int size) {
    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        printf("error occured\n");
        return;
    }
    
    char buffer[size + 1];
    ssize_t bytes_read = read(file_descriptor, buffer, size);
    if (bytes_read == -1) {
        close(file_descriptor);
        return;
    }
    
    buffer[bytes_read] = '\0';
    if (bytes_read > 0) {
        printf("%d %s\n", size, buffer);
    } else {
        // File is empty.
    }
    
    close(file_descriptor);
    
}

/*
	Open file and write char array tmp to your file.
*/
void write_file(char* filename, char* tmp) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return;
    }

    size_t len = strlen(tmp);
    ssize_t bytes_written = write(fd, tmp, len);


    close(fd);
}

/* 
    Copy the contents of source file to the dest file back and forth as much as n bytes.For example, suppose "1234abcd\n" is the content of the source file. Then "1a2b3c4d\n" will be the content of the dest file. You sholud use lseek() function.
    If there is no dest file, create it by using "create_file" funtion
*/
void mixup_copy_file(char* source, char* dest,int n) {
    int source_fd = open(source, O_RDONLY);
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    char buffer[n];
    ssize_t bytes_read, bytes_written;

    off_t file_length = lseek(source_fd, 0, SEEK_END);
    lseek(source_fd, 0, SEEK_SET);


    bytes_read = read(source_fd, buffer, file_length);

    for (ssize_t i = 0; i < n; i++) {
        for (ssize_t j = 0; j < file_length; j = j+n) {
            write(dest_fd, &buffer[i+j], 1);
        }
    }    


    close(source_fd);
    close(dest_fd);

}

/*
    Get 'command' as an input,
    and execute proper functionalities according to the command line.
    Only `create`, `write`, `read`, `copy`, `ls` commands are allowed.
    If `file` takes input from one of those commands, it should return "True", otherwise "False".
    
*/
int file(char* command) {
    char copy[MAX_LEN];
    strcpy(copy, command);

    char* token = strtok(copy, " \n");  
    char* v1;
    char* v2;
    char* v3;
    if (strcmp(token, "create") == 0) {
        v1 = strtok(NULL, " \n");
        if (token != NULL) {
            create_file(token);
            return True;
        }
        if (token == NULL) {
            return False;
        } 
    } else if (strcmp(token, "write") == 0) {
        v1 = strtok(NULL, " \n");
        v2 = strtok(NULL, " \n");
        if (v1 != NULL) {
            if (v2 != NULL) {
                write_file(v1, v2);
                return True;
            } else {
                return False;
            }
        } 
    } else if (strcmp(token, "read") == 0) {
        v1 = strtok(NULL, " \n");
        v2 = strtok(NULL, " \n");
        int s = atoi(v2);
        if (v1 != NULL) {
            if (v2 != NULL) {
                read_file(v1, s);
                return True;
            } else {
                return False;
            }
        } 
    } else if (strcmp(token, "copy") == 0) {
        v1 = strtok(NULL, " \n");
        v2 = strtok(NULL, " \n");
        v3 = strtok(NULL, " \n");
        int s = atoi(v3);
        if (v1 != NULL) {
            if (v2 != NULL) {
                mixup_copy_file(v1,v2, s);
                return True;
            } else {
                return False;
            }
        } 
    } else if (strcmp(token, "ls") == 0) {
        ls();
        return True;
    } else {
        return False;
    }

    return 0;  // Command not recognized
}