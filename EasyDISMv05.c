	#define _CRT_SECURE_NO_WARNINGS

	#include <stdio.h>
	#include <windows.h>
	#include <stdlib.h>

	#define error_job_not_specified

	/* to do: uhhh add gui
		also add the error codes
	*/
	int main() {


		printf("Welcome to the program. Type 'I' to see the info of a WIM/ESD file, and type 'A' to apply a WIM/ESD image to a specific drive. \n");

		char i;
		scanf("%c", &i);
		char wimpath[1000];

			if (i == 'I')
			{
				char commandgetwiminfo[3000];

				printf("Enter the path to the WIM/ESD file.\n");
				scanf("%s", &wimpath); // path that the indexes of the wim/esd will be shown

				printf("%s is the path to the WIM/ESD file.\n", wimpath);
				sprintf(commandgetwiminfo, "dism /get-imageinfo /imagefile:%s", wimpath);

				system(commandgetwiminfo);
				system("pause");
				return 0;
			}

			if (i == 'A')

			{
				char commandapplyimage[3000];
				printf("Enter the path to the WIM/ESD file\n"); //path for esd/wim
				char wimpathforapply[1000];
				scanf("%s", &wimpathforapply);
				printf("\n%s is the path of the WIM.", wimpathforapply);

				char getindexforapply[2000];
				sprintf(getindexforapply, "dism /get-imageinfo /imagefile:%s", wimpathforapply);
				system(getindexforapply);
				printf("\nHere are the index numbers. Choose a number; this will be the edition which will be installed.\n");
				// bit above will show indexes so that usr can decide what to do

				int index;
				scanf("%d", &index);
				// index selection

				printf("Now choose a drive lettwr to install Windows to\n");
				char installdrive[2000];
				scanf("%s", &installdrive);
				
				// dism /apply-image /imagefile:install.wim (wimpathforapply) /index:1 (index) /applydir:C:\ (installdrive)

				sprintf(commandapplyimage, "dism /apply-image /imagefile:%s /index:%d /applydir:%s:\\", wimpathforapply, index, installdrive);
				system(commandapplyimage);

				// bcdboot now to make it bootable
				
				int bcdboot;
				printf("Would you like to make the installed OS bootable? If so, type 1. Type 0 if you don't want to.");
				scanf("%d", &bcdboot);

				if (bcdboot == 1)
				{
					char bcdcommand[2000];
					sprintf(bcdcommand, "bcdboot %s:\\Windows", installdrive);
					system(bcdcommand);
					printf("Job done successfully\n");
					system("pause");
					return 0;
				}

				else
				{
					printf("Job done successfully\n");
					system("pause");
					return 0; // this is if no bcdboot
				}

				
			}

		printf("No acceptable user input was given.\nGoodbye!\n");
		system("pause");
		return 0;
	}