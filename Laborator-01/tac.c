
       #include <stdio.h>
       #include <stdlib.h>
       #include<string.h>

       int main(int argc, char *argv[])
       {
           FILE *stream;	// fisierul 
           char *line = NULL;	// linia
           size_t len = 0;	// lungime maxima linie
           ssize_t nread;	// rezultat getline
	   char *buf[10];

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <file>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           stream = fopen(argv[1], "r");
           if (stream == NULL) {
               perror("fopen");
               exit(EXIT_FAILURE);
           }
	int i = 0;
           while ((nread = getline(&line, &len, stream)) != -1) {
		buf[i] = (char*)malloc(10*sizeof(char));
		strcpy(buf[i], line);
		i++;
           }
	
	int j;
	for (j = i - 1; j >= 0; j--) {
		printf("%s", buf[j]);
	}
	
           free(line);
           fclose(stream);
           exit(EXIT_SUCCESS);
       }

