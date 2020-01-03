#include "control.h"

#define SHM_KEY 24602
#define SEM_KEY 11280
#define SEG_SIZE 200
#define FILE_NAME "semaphone"

int crt_file(char * fname) {
  int fd;
  fd = open(fname, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0644);
  if (fd < 0) {
    fd = open(fname, O_RDWR | O_TRUNC);
  }
  printf("file created\n");
  return fd;
}

void rem_file(char * fname) {
  if (remove(fname) == 0) {
    printf("file removed\n");
  } else {
    printf("file not removed");
  }
}

int crt_sem(){
  int semd;
  int r;
  int v;
  semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    printf("error %s\n", strerror(errno));
    semd = semget(SEM_KEY,1,0);
    v = semctl(semd, 0, GETVAL, 0);
    //printf("semctl returned %d\n", v);
  }
  else{
    union semun usl;
    usl.val = 1;
    r = semctl(semd, 0, SETVAL, usl);
    //printf("semctl, returned: %d\n", r);
  }
  printf("semaphore created\n");
  return semd;
}

void rem_sem(int semd){
  semctl(semd, IPC_RMID, 0);
  printf("semaphore removed\n");
}

int crt_shm(){
  int shmd;
  shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT|0600);
  printf("shared memory created\n");
  return shmd;
}

void rem_shm(int shmd){
  shmctl(shmd, IPC_RMID, 0);
  printf("shared memory removed\n");
}

int print_story(){
  printf("The story so far:\n");
  char story[SEG_SIZE];
  int fd;
  fd = open(FILE_NAME, O_RDONLY);
  int bytes = read(fd, story, sizeof(story));
  if (bytes < 0){
    printf("Reading file errno: %d\nerror: %s\n",errno, strerror(errno));
    return 0;
  }
  while(bytes > 0){
    printf("%s\n", story);
    bytes = read(fd, story, sizeof(story));
  }
  close(fd);
  printf("\n");
}

int main(int argc, char * argv[]) {
  int semd;
  int shmd;
  int fd;
  if (argc > 1) {
    if(strcmp(argv[1], "-c") == 0) {
      semd = crt_sem();
      shmd = crt_shm();
      fd = crt_file(FILE_NAME);
    }
    else if (strcmp(argv[1], "-r") == 0){
      print_story();
      semd = semget(SEM_KEY, 1, 0);
      shmd = shmget(SHM_KEY, SEG_SIZE, 0600);
      rem_shm(shmd);
      rem_file(FILE_NAME);
      rem_sem(semd);
    }
    else if (strcmp(argv[1], "-v") == 0) {
      print_story();
    }
  }
  else if (argc == 1){
    printf("Argument needed\n");
  }
  else if (argc > 2){
    printf("Too many arguments\n");
  }
  return 0;
}

/*int create(){
  int r,v;
  int semd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0644);
  printf("semaphore created\n");
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("shared memory created\n");
  int fd = open("semaphone.txt", O_CREAT | O_TRUNC | O_RDWR | O_TRUNC, 0644);
  if (fd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    fd = open("semaphone.txt", O_RDWR | O_TRUNC);
  }
  printf("file created\n");
  close(fd);
  return 0;
}

int remove_story(){
  int semd = semget(SEM_KEY, 1, 0);
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("trying to get in\n");
  semop(semd, &sb, 1);
  int shmd = shmget(SHM_KEY, sizeof(char*), 0);
  if (shmd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  view();
  shmctl(shmd, IPC_RMID,0);
  printf("shared memory removed\n");
  remove("semaphone.txt");
  printf("file removed\n");
  semctl(semd, IPC_RMID, 0);
  printf("semaphore removed\n");
  return 0;
}

int view(){
	FILE *fptr;
	char ch;
	fptr = fopen("semaphone.txt", "r");
	if (fptr == NULL){
		printf("File can't be opened\n");
		return -1;
	}
	printf("The story so far...\n");
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	printf("\n");
	fclose(fptr);
	return 0;
} */
