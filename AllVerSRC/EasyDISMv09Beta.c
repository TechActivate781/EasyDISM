#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <versionhelpers.h>
#include <stdlib.h>
#include <string.h>

#define error_job_not_specified 2
#define error_dism_command_failed 4
#define no_bcdboot_request_by_user 3
#define nt_version_too_old 5
#define nt_bcdboot_exit 6   


// ver 0.9; first usable version

int VersionCheck();
void ErrorCheck(int returncode);
int CheckWimOld(char path[]);
int CheckWimNew(char path[]);
int ApplyWimNew(char path[], char driveletter, int index);
int ApplyWimOld(char path[], char driveletter, int index);
int bcdboot(int choice, char installdrive);
void GracefullExit();

int main(){
    int IsNew = VersionCheck(); //0 = new; 1 = win7/8; 2 = ancient
    if(IsNew == 2){
        printf("Too old version of Windows. The app requires at least Windows 7 or above\n");
        exit(nt_version_too_old);
    }

    printf("Welcome\nType I to view the information about an image\nType A to apply an image\n");
    char choice;
    scanf("%c", &choice);

    char path[1000];
    int returnCode;
    char driveletter;
    char bcdbootdriveletter;
    int bcdbootchoice;
    int index;

    if((choice == 'I' || choice == 'i') && IsNew == 0){
        printf("What is the path to the image\n");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = '\0';
        returnCode = CheckWimNew(path);
        ErrorCheck(returnCode);
        GracefullExit();
    }

    else if((choice == 'I' || choice == 'i') && IsNew == 1){
        printf("What is the path to the image\n");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = '\0';
        returnCode = CheckWimOld(path);
        ErrorCheck(returnCode);
        GracefullExit();
    }

    else if((choice == 'A' || choice == 'a') && IsNew == 0){
        printf("What is the path to the image\n");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = '\0';
        returnCode = CheckWimNew(path);
        ErrorCheck(returnCode);
        printf("These are the indexes. Please choose one: ");
        scanf(" %d", &index);
        printf("Please write the letter to the drive you will apply the image to: ");
        scanf(" %c", &driveletter);
        returnCode = ApplyWimNew(path, driveletter, index);
        ErrorCheck(returnCode);

        // bcdboot now. if the program has failed, will not reach here

        printf("Would you like to make the image bootable? Type 1 for yes; type 0 for no\n");
        scanf(" %d", &bcdbootchoice);
        bcdboot(bcdbootchoice, driveletter);
        
    }

    else if((choice == 'A' || choice == 'a') && IsNew == 1){
        printf("What is the path to the image\n");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = '\0';
        returnCode = CheckWimOld(path);
        ErrorCheck(returnCode);
        printf("These are the indexes. Please choose one: ");
        scanf(" %d", &index);
        printf("Please write the letter to the drive you will apply the image to: ");
        scanf(" %c", &driveletter);
        returnCode = ApplyWimOld(path, driveletter, index);
        ErrorCheck(returnCode);
        printf("Would you like to make the image bootable? Type 1 for yes; type 0 for no\n");
        scanf(" %d", &bcdbootchoice);
        bcdboot(bcdbootchoice, driveletter);
    }

    else{
        printf("Weird input\n");
        system("pause");
        return 0;
    }
}

void GracefullExit(){
    printf("Thank you for using the program\n");
    system("pause");
    exit(0);
}

int ApplyWimOld(char path[], char driveletter, int index){
    char command[1000]; 
    sprintf(command, "dism /apply-wim /wimfile:%s /index:%d /applydir:%c:\\", path, index, driveletter);
    int code = system(command);
    return code;
}

int bcdboot(int choice, char installdrive){
    if(choice == 0){
        printf("BCDBoot will not be used. Thank you for using the program\n");
        system("pause");
        exit(no_bcdboot_request_by_user);
    }

    else if(choice == 1){
        printf("Would you like to put the BCD files on another partition? Type 1 for yes, type 0 for no\n");
        int choice;
        scanf(" %d", &choice);
        char bcdcommand[1000];
        if(choice == 0){
            sprintf(bcdcommand, "bcdboot %c:\\windows", installdrive);
            GracefullExit();
        }

        else if(choice == 1){
            printf("What is the boot drive? ");
            char bcdbootdrive;
            scanf("%c", &bcdbootdrive);
            printf("Would you like to use UEFI? Type 1 for yes, 0 for no\n");
            scanf("%d", &choice);

            if(choice == 0){
                sprintf(bcdcommand, "bcdboot %c:\\windows /s %c:\\", installdrive, bcdbootdrive);
                system(bcdcommand);
                GracefullExit();
            }

            else if(choice == 1){
                sprintf(bcdcommand, "bcdboot %c:\\windows /s %c:\\ /f UEFI", installdrive, bcdbootdrive);
                system(bcdcommand);
                GracefullExit();
            }

            else{
                printf("Weird input. Thanks for using the program\n");
                system("pause");
                exit(0);
            }

        }
    }

    else{
        printf("Unkown input. Exiting\n");
        system("pause");
        exit(0);
    }
}

int ApplyWimNew(char path[], char driveletter, int index){
    char command[1000]; // not super efficient but its < 1kb so...
    sprintf(command, "dism /apply-image /imagefile:%s /index:%d /applydir:%c:\\", path, index, driveletter);
    int code = system(command);
    return code;
}

int CheckWimOld(char path[]){
    char command[1000];
    sprintf(command, "dism /get-wiminfo /wimfile:\"%s\"", path);
    int code = system(command);
    return code;
}

int CheckWimNew(char path[]){
    char command[1000];
    sprintf(command, "dism /get-imageinfo /imagefile:\"%s\"", path);
    int code = system(command);
    return code;
}

int VersionCheck(){
    if(IsWindows8Point1OrGreater() == 1){
        return 0;
    }

    if(IsWindows7OrGreater() == 1){
        return 1;
    }

    else{
        return 2;
    }
}

void ErrorCheck(int returncode){
    // not sure what that previous version of it was lol
    if(returncode != 0){
    switch(returncode){
        case 740:
        system("cls");
        printf("Re-run the app with Administrator right\n");
        system("pause");
        exit(error_dism_command_failed);
        break;

        case 87:
        system("cls");
        printf("There is an issue with the syntax\n");
        system("pause");
        exit(error_dism_command_failed);
        break;

        case 2:
        system("cls");
        printf("The file could not be found\n");
        system("pause");
        exit(error_dism_command_failed);
        break;

        default:
        printf("An error occoured\n");
        system("pause");
        exit(error_dism_command_failed);
        break;
    }
}
}
