#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <versionhelpers.h>
#include <stdlib.h>

#define error_job_not_specified 2
#define error_dism_command_failed 4
#define no_bcdboot_request_by_user 3
#define nt_version_too_old 5

// to do: uhhh add gui


void VersionCheck(){
    if(IsWindows8Point1OrGreater() == 1){
        printf("Re-run the app with Windows 8.1 (NT 6.3) or greater\n");
        exit(5);
    }
}

void check_dism_error(int return_code) {

	printf("DISM command failed with code %d.\n", return_code);
	switch (return_code) {
	case 87:
		system("cls");
		printf("DISM failed with code 87\n");
		printf("There is an issue with the syntax.\n");
		system("pause");
		exit(error_dism_command_failed);
		break;
	case 2:
		system("cls");
		printf("DISM Failed with code 2\n");
		printf("The file could not be found\n");
		system("pause");
		exit(error_dism_command_failed);
		break;
	case 740:
		system("cls");
		printf("DISM failed with code 740\n");
		printf("Re-run the program as Administrator or an above level.\n");
		system("pause");
		exit(error_dism_command_failed);
		break;
	default:
		printf("An unkown error occurred.\n");
		system("pause");
		exit(error_dism_command_failed);
		break;

	}

}
int main() {
    VersionCheck();
	printf("Welcome to the program.\nType 'I' to see the info of a WIM/ESD file.\nType 'A' to apply a WIM/ESD image to a specific drive. \n");

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



		int return_code = system(commandgetwiminfo);

		if (return_code != 0) {
		check_dism_error(return_code);
		}

		printf("Command completed successfully\n");
		system("pause\n");
		exit(0);

	}
	/*	if (return_code != 0) {
			system("cls");
			check_dism_error(return_code);
		}

		printf("Goodbye!\n");
		system("pause");
		return 0;
	}*/

	if (i == 'A')

	{
		char commandapplyimage[3000];
		printf("Enter the path to the WIM/ESD file\n"); //path for esd/wim
		char wimpathforapply[1000];
		scanf("%s", &wimpathforapply);
		printf("\n%s is the path of the WIM.", wimpathforapply);

		char getindexforapply[2000];
		sprintf(getindexforapply, "dism /get-imageinfo /imagefile:%s", wimpathforapply);

		int return_code = system(getindexforapply);
		
		if (return_code != 0) {
			check_dism_error(return_code);
		}

		printf("\nHere are the index numbers.\nChoose a number; this will be the edition which will be installed.\n");
		// bit above will show indexes so that usr can decide what to do

		int index;
		scanf("%d", &index);
		// index selection

		printf("Now choose a drive letter to install Windows to\n");
		char installdrive[2000];
		scanf("%s", &installdrive);

		// dism /apply-image /imagefile:install.wim (wimpathforapply) /index:1 (index) /applydir:C:\ (installdrive)

		sprintf(commandapplyimage, "dism /apply-image /imagefile:%s /index:%d /applydir:%s:\\", wimpathforapply, index, installdrive);
		return_code = system(commandapplyimage);
		
		if (return_code != 0) {
			check_dism_error(return_code);
		}

		// bcdboot now to make it bootable

		int bcdboot;
		printf("Would you like to make the installed OS bootable? If so, type 1. Type 0 if you don't want to.\n");
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
			printf("Image applied successfully\nBoot files NOT created.\n");
			system("pause");
			exit(no_bcdboot_request_by_user);
		}


	}

	printf("No acceptable user input was given.\nGoodbye!\n");
	system("pause");
	return 0;
}
