#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUF_LEN 1024
#define CMD_LEN 256
#define HIST_LEN 2048
#define LIST_LEN 2048

int is_directory(const char *path)
{
   struct stat path_stat;
   if (stat(path, &path_stat) != 0)
       return 0;
   return S_ISDIR(path_stat.st_mode);
}

void recu_del(const char *path, bool verbose, bool force)
{
	DIR * dir;
	int verify;
	char approved;
	dir = opendir (path);
	char next_path[BUF_LEN];
	struct dirent * next_file;
	while ((next_file = readdir (dir)) != NULL)
	{
		if (strcmp(next_file->d_name, ".") && strcmp(next_file->d_name, ".."))
		{
			strcpy (next_path, path);
			strcat (next_path, "/");
			strcat (next_path, next_file->d_name);
			if (is_directory (next_path))
			{
				if (access( next_path, W_OK ) != 0 && force == false)
				{
					printf ("remove write protected %s (y or n)? ", next_path);
					scanf ("%c", &approved);
					getchar();
					if (approved == 'y')
					{
						recu_del (next_path, verbose, force);
						verify = remove (next_path);
						if (verify != 0)
							printf ("%s does not exist\n", next_path);
						else if (verbose == true)
							printf ("removed: %s\n", next_path);
					}
				}
				else
				{
					recu_del (next_path, verbose, force);
					verify = remove (next_path);
					if (verify != 0)
						printf ("%s does not exist\n", next_path);
					else if (verbose == true)
						printf ("removed: %s\n", next_path);
				}
			}
			else
			{
				if (access( next_path, W_OK ) != 0 && force == false)
				{
					printf ("remove write protected %s (y or n)? ", next_path);
					scanf ("%c", &approved);
					getchar();
					if (approved == 'y')
					{
						verify = remove (next_path);
						if (verify != 0)
							printf ("%s does not exist\n", next_path);
						else if (verbose == true)
							printf ("removed: %s\n", next_path);
					}
				}
				else
				{
					verify = remove (next_path);
					if (verify != 0)
						printf ("%s does not exist\n", next_path);
					else if (verbose == true)
						printf ("removed: %s\n", next_path);
				}

			}
		}
	}
	closedir (dir);
}

void kill_child(int sig)
{
    kill(0,SIGKILL);
}

int main()
{
	DIR * dir;
	int i, j, k;
	int n, count = 0, verify, len_list;
	bool attr_f, attr_v, attr_r, ls_x;
	char * pch;
	char approved;
	char cwd[BUF_LEN];
	char path[BUF_LEN];
	char inp_f[BUF_LEN];
	char out_f[BUF_LEN];
	char name_exe[BUF_LEN];
	char cmd[CMD_LEN];
	char hist[HIST_LEN][CMD_LEN];
	char list_files[LIST_LEN][BUF_LEN];
	struct dirent * next_file;

	do
	{
		len_list = 0;
		attr_f = false;
		attr_v = false;
		attr_r = false;
		ls_x = false;
		getcwd (cwd, BUF_LEN);
		printf ("%s$ ", cwd);
		scanf ("%[^\n]%*c", cmd);
		strcpy (hist[count++], cmd);
		exe_cmd:
		pch = strtok (cmd, " ");
		if (!strcmp (pch, "exit"))
			goto end;
		if (!strcmp (pch, "ls"))
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				if ((pch = strtok (NULL, " ")) != NULL)
					dir = opendir (pch);
				else
					dir = opendir (cwd);
				while ((next_file = readdir (dir)) != NULL)
					printf("%s\n", next_file->d_name);
				closedir (dir);
				exit (127);
			}
			waitpid(pid, 0, 0);
		}
		else if (!strcmp (pch, "cd"))
		{
			pch = strtok (NULL, " ");
			chdir (pch);
		}
		else if (!strcmp (pch, "rm"))
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				while ((pch = strtok (NULL, " ")) != NULL)
				{
					if (pch[0] == '-')
					{
						for (i = 1; pch[i] != 0; i++)
						{
							if (pch[i] == 'f')
								attr_f = true;
							else if (pch[i] == 'v')
								attr_v = true;
							else if (pch[i] == 'r')
								attr_r = true;
						}
					}
					else
					{
						if (is_directory (pch))
						{
							if (attr_r == true)
							{
								if (access( pch, W_OK ) != 0 && attr_f == false)
								{
									printf ("remove write protected %s (y or n)? ", pch);
									scanf ("%c", &approved);
									getchar();
									if (approved == 'y')
									{
										recu_del (pch, attr_v, attr_f);
										verify = remove (pch);
										if (verify != 0)
											printf ("%s does not exist\n", pch);
										else if (attr_v == true)
											printf ("removed: %s\n", pch);
									}
								}
								else
								{
									recu_del (pch, attr_v, attr_f);
									verify = remove (pch);
									if (verify != 0)
										printf ("%s does not exist\n", pch);
									else if (attr_v == true)
										printf ("removed: %s\n", pch);
								}
							}
							else if (attr_f == false)
								printf ("%s: is a directory\n", pch);
						}
						else
						{
							if (access( pch, F_OK ) != -1)
							{
								if (access( pch, W_OK ) != 0 && attr_f == false)
								{
									printf ("remove write protected %s (y or n)? ", pch);
									scanf ("%c", &approved);
									getchar();
									if (approved == 'y')
									{
										verify = remove (pch);
										if (verify != 0)
												printf ("%s does not exist\n", pch);
										else if (attr_v == true)
											printf ("removed: %s\n", pch);
									}
								}
								else
								{
									verify = remove (pch);
									if (verify != 0)
										printf ("%s does not exist\n", pch);
									else if (attr_v == true)
										printf ("removed: %s\n", pch);
								}
							}
							else
								printf(" %s does not exist\n", pch);
						}
					}
				}
			exit (127);
			}
			waitpid(pid, 0, 0);

		}
		else if (!strcmp (pch, "history"))
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				if ((pch = strtok (NULL, " ")) != NULL)
				{
					n = atoi(pch);
					if (n > count)
						n = count;
					for (i = n; i > 0; i--)
					{
						printf ("  %4d  %s\n", count -i + 1, hist[count - i]);
					}

				}
				else
				{
					for (i = 0; i < count; i++)
					{
						printf ("  %4d  %s\n", i + 1, hist[i]);
					}
				}
				exit (127);
			}
			waitpid(pid, 0, 0);
		}
		else if (!strcmp (pch, "issue"))
		{
			if ((pch = strtok (NULL, " ")) != NULL)
			{
				n = atoi(pch);
				strcpy (cmd, hist[n - 1]);
				goto exe_cmd;
			}
		}
		else if (!strcmp (pch, "rmexcept"))
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				while ((pch = strtok (NULL, " ")) != NULL)
				{
					strcpy (list_files[len_list++], pch);
				}
				dir = opendir (cwd);
				bool del;
				while ((next_file = readdir (dir)) != NULL)
				{
					if (strcmp (next_file->d_name, ".") != "." && strcmp (next_file->d_name, "..") != "..")
					{
						del = true;
						for (i = 0; i < len_list; i++)
						{
							if (!strcmp (list_files[i], next_file->d_name))
							{
								del = false;
								break;
							}
						}
						if (del == true)
						{
							printf ("removed: %s\n", next_file->d_name);
							remove (next_file->d_name);
						}
					}
				}
				closedir (dir);
				exit (127);
			}
			waitpid(pid, 0, 0);
		}
		else
		{
			strcpy (name_exe, pch);
			bool f_inp = false;
			bool f_out = false;
			bool tmr = false;
			int timer_exe;
			while ((pch = strtok (NULL, " ")) != NULL)
			{
				if(!strcmp(pch, "<"))
				{
					pch = strtok (NULL, " ");
					strcpy (inp_f, pch);
					f_inp = true;
				}
				else if(!strcmp(pch, ">"))
				{
					pch = strtok (NULL, " ");
					strcpy (out_f, pch);
					f_out = true;
				}
				else
				{
					timer_exe = atoi(pch);
					tmr = true;
				}
			}
			pid_t pid = fork();
			if (pid == 0) {
				int fd1, fd2;
				if (f_inp == true)
				{
					fd1 = open(inp_f, O_RDONLY);
					dup2(fd1, 0);
				}
				if (f_out == true)
				{
					fd2 = open(out_f, O_RDWR | O_CREAT);
					dup2(fd2, 1);
				}
				if (tmr)
				{
					signal(SIGALRM,(void (*)(int))kill_child);
					alarm(timer_exe);
				}
        		execl(name_exe, NULL);
        		if (f_inp == true)
        			close(fd1);
        		if (f_out == true)
        			close(fd2);
        		exit(127);
    		}
       		waitpid(pid, 0, 0);
		}
	}
	while (true);

	end:
	return 0;
}
