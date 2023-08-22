#include <stdio.h>
#include<stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
/**
 * _strcat - concatenates two strings
 * @dest: input value
 * @src: input value
 *
 * Return: void
 */
char *_strcat(char *dest, char *src)
{
	int i;
	int j;

	i = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	j = 0;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}

	dest[i] = '\0';
	return (dest);
}
/**
 * char *_strcpy - a function that copies the string pointed to by src
 * @dest: copy to
 * @src: copy from
 * Return: string
 */
char *_strcpy(char *dest, char *src)
{
	int l ;
	int x ;

	l = strlen(src);
	for (x = 0; x < l ; x++)
	{
		dest[x] = src[x];
	}
	dest[l] = '\0';
	return (dest);
}
/**
 *_strlen - return the length of a string
 *@s: input
 *Return: int length of string
 */
int _strlen(char *s)
{
	int size = 0;

	while (s[size] != '\0')
	{
		size++;
	}
	return (size);
}
/**
 * get_location - return the path of a command
 * @command: command word
 * Return: path 
 */
 char *get_location(char *command)
 {
     char *path, *pathcpy = NULL, *token, *filep;
     int clen, dlen;
     struct stat buff;
     
     path = getenv("PATH");
     if (path)
     {
         _strcpy(pathcpy, path);
         clen = _strlen(command);
         token = strtok(pathcpy, ":");
         while (token)
         {
             dlen = _strlen(token);
             filep = malloc(dlen + clen + 2);
             _strcat(filep, token);
             _strcat(filep, "/");
             _strcat(filep, command);
             _strcat(filep, "\0");
             if (stat(filep, &buff) == 0)
             {
                 free(pathcpy);
                 return(filep);
             }
             else
             {
                 free(filep);
                 token = strtok(NULL, ":");
             }
         }
         free(pathcpy);
         if (stat(command, &buff) == 0)
         {
             return (command);
         }
         return (NULL);
     }
     return (NULL);
 }
 /**
 * execute - executes a given command
 * @argv: pointer to strings array
 */
 void execute(char **argv)
 {
     char *command = NULL;
     
     command = malloc(_strlen(argv[0]));
     if (command == NULL)
     {
         perror("memory allocation error");
         return;
     }
     if (argv)
     {
         command = argv[0];
         if (execve(command, argv, NULL) == -1)
         {
             perror("no such file or directory...");
         }
     }
 }
char *get_loc(char *command)
{
    char *path, *pathcpy = NULL, *token, *filep;
     int clen, dlen;
     struct stat buff;
     
     path = getenv("PATH");
     if (path != NULL)
     {
         pathcpy = malloc(_strlen(path));
         _strcpy(pathcpy, path);
         clen = _strlen(command);
         token = strtok(pathcpy, ":");
         while (token)
         {
             dlen = _strlen(token);
             filep = malloc(dlen + clen + 2);
             if(filep)
             {
                 _strcat(filep, token);
                 _strcat(filep, "/");
                 _strcat(filep, command);
                 _strcat(filep, "\0");
             }
             if (stat(filep, &buff) == 0)
             {
		     return(filep);
             }
             else
             {
                 token = strtok(NULL, ":");
             }
         }
         free(pathcpy);
	 free(filep);
         if (stat(command, &buff) == 0)
         {
             return(command);
         }
	 return(NULL);
     }
     return(NULL);
}
int main(void)
{
	char *prompt = "$ ";
     char *line = NULL, *linecpy = NULL, *token, *path;
     size_t num = 0;
     ssize_t nchar = 0;
     const char *d = " \n";
     char **str;
     int num_tokens = 0;
     int pid;
     while (1)
     {
         printf("%s", prompt);
         nchar = getline(&line, &num, stdin);
         if (nchar == -1)
         {
             printf("Exiting Shell ....\n");
             return (-1);
         }
         linecpy = malloc(sizeof(char) * nchar);
         if (linecpy == NULL)
         {
             perror("memory allocation error...");
             return (-1);
         }
         _strcpy(linecpy, line);
         str = malloc(sizeof(char *) * num_tokens);
         token = strtok(linecpy, d);
         while(token)
         {
             str[num_tokens] = malloc(sizeof(char) * _strlen(token));
             _strcpy(str[num_tokens], token);
             token = strtok(NULL, d);
             num_tokens++;
         }
         str[num_tokens] = token;
         str[num_tokens++] = NULL;
         /*pid = fork();
         if (pid == 0)
         {
             execute(str);
         }
         else
         {
            wait(NULL);
         }*/
         pid = fork();
         if(pid == 0)
         {
             path = get_loc(str[0]);
             if (path){
                 if (execve(path, str, NULL) == -1)
                 {
                     printf("No such file or directory...\n");
                     return(-1);
                 }
             }
             else
             {
                 wait(NULL);
             }
         }
     }
     free(line);
     free(linecpy);
     free(str);
     /*printf("%d", num_tokens);*/
     return (0);
}
