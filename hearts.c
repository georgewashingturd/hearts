#include <stdio.h>
#include <windows.h>
#include <Psapi.h>
#include <stdlib.h>
#include <time.h>

#define PIDARRAYSIZE 1024
#define MODULEARRAYSIZE 1024
#define MAXBASENAME 1024

DWORD GetProcessPidByName(char * name)
{
    DWORD PidArray[PIDARRAYSIZE];
    DWORD PidRead;
    HMODULE ModuleArray[MODULEARRAYSIZE];
    DWORD ModuleRead;
    HANDLE prochandle;
    char BaseName[MAXBASENAME];

    if (!EnumProcesses((DWORD*)&PidArray, PIDARRAYSIZE*sizeof(DWORD), &PidRead)) 
        return 0;
    
    for (int i = 0; i < (int)(PidRead / sizeof(DWORD)); i++)
    {
        prochandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PidArray[i]);
        
        if (!prochandle) 
            continue;

        if (!EnumProcessModules(prochandle, (HMODULE*)&ModuleArray, MODULEARRAYSIZE*sizeof(HMODULE), &ModuleRead)) 
            continue;
        
        // USE 0 IN ARRAY, LOOPING PRODUCES AN UNDESIRED EFFECT OF LISTING DLL'S AND JUNK
        //if (!GetModuleFileNameEx(prochandle, ModuleArray[0], (LPTSTR)&BaseName, MAXBASENAME))
        if (!GetModuleBaseName(prochandle, ModuleArray[0], (LPTSTR)&BaseName, MAXBASENAME))
            continue;

        if (!stricmp(name, BaseName))
            return PidArray[i];
    }
    
    return -1;
}

#define DIAMOND 4
#define CLUB    5
#define HEART   3
#define SPADE   6

BYTE Player [4][13][2];
/*
void PrintCards(BYTE * cards, BYTE CardKind, BYTE count)
{
    BYTE i;
    char ItemString[8];
    
    for (i = 0;i < count;i++)
    {
        switch (cards[i])
        {
            case 11:
                sprintf(ItemString,"%c J ",CardKind);
                break;
            case 12:
                sprintf(ItemString,"%c Q ",CardKind);
                break;
            case 13:
                sprintf(ItemString,"%c K ",CardKind);
                break;
            case 14:
                sprintf(ItemString,"%c A ",CardKind);
                break;
            default:
                sprintf(ItemString,"%c%2d ",CardKind,cards[i]);
                break;
        }
        printf("%s",ItemString);
    }
    
    if (count == 0)
        printf("Empty");
}
*/

void PrintCards(BYTE * cards, BYTE CardKind, BYTE count)
{
    BYTE i;
    BYTE j;
    BYTE k;
    char ItemString[8];
    
    j = 2;
    for (i = 0;i < count;i++)
    {
        if (i == 0 && cards[i] != 2)
        {
            for (k = 0 ; k < (cards[i] - j); k++)
            {
                printf("%c%c%c%c",178,178,178,178);
            }
        }
        else
        {
            for (k = 0 ; k < (cards[i] - j - 1) && (cards[i] - j) > 1; k++)
            {
                printf("%c%c%c%c",178,178,178,178);
            }
        }
        
        j = cards[i];

        switch (cards[i])
        {
            case 11:
                sprintf(ItemString,"%c J ",CardKind);
                break;
            case 12:
                sprintf(ItemString,"%c Q ",CardKind);
                break;
            case 13:
                sprintf(ItemString,"%c K ",CardKind);
                break;
            case 14:
                sprintf(ItemString,"%c A ",CardKind);
                break;
            default:
                sprintf(ItemString,"%c%2d ",CardKind,cards[i]);
                break;
        }
        printf("%s",ItemString);
    }
    
    if (count == 0)
        printf("Empty %c", CardKind);
}


void sort (BYTE * cards, BYTE count)
{
    BYTE i, j;
    BYTE temp;
    
    for (i = 0;i < count - 1;i++)
    {
        for (j = i;j < count;j++)
        {
            if (cards[i] > cards[j])
            {
                temp = cards[j];
                cards[j] = cards[i];
                cards[i] = temp;
            }
        }
    }
}

void SortCards(void)
{
    BYTE club[4][13];
    BYTE diamond[4][13];
    BYTE heart[4][13];
    BYTE spade[4][13];
    BYTE c_count[4];
    BYTE d_count[4];
    BYTE h_count[4];
    BYTE s_count[4];
    BYTE i, j;
    
    for (i = 0;i < 4;i++)
    {
        c_count[i] = 0;
        d_count[i] = 0;
        h_count[i] = 0;
        s_count[i] = 0;

        for (j = 0;j < 13;j++)
        {
            switch (Player[i][j][0])
            {
                case CLUB:
                club[i][c_count[i]++] = Player[i][j][1];
                break;
                case DIAMOND:
                diamond[i][d_count[i]++] = Player[i][j][1];
                break;
                case HEART:
                heart[i][h_count[i]++] = Player[i][j][1];
                break;
                case SPADE:
                spade[i][s_count[i]++] = Player[i][j][1];
                break;
            }
        }
        sort(club[i], c_count[i]);
        sort(diamond[i], d_count[i]);
        sort(spade[i], s_count[i]);
        sort(heart[i], h_count[i]);
    }
    
    printf("Left  : "); PrintCards(club[1], CLUB, c_count[1]); printf("\n");
    printf("Top   : "); PrintCards(club[2], CLUB, c_count[2]); printf("\n");
    printf("Right : "); PrintCards(club[3], CLUB, c_count[3]); printf("\n");
    printf("You   : "); PrintCards(club[0], CLUB, c_count[0]); printf("\n");
    printf("\n");

    printf("Left  : "); PrintCards(diamond[1], DIAMOND, d_count[1]); printf("\n");
    printf("Top   : "); PrintCards(diamond[2], DIAMOND, d_count[2]); printf("\n");
    printf("Right : "); PrintCards(diamond[3], DIAMOND, d_count[3]); printf("\n");
    printf("You   : "); PrintCards(diamond[0], DIAMOND, d_count[0]); printf("\n");
    printf("\n");

    printf("Left  : "); PrintCards(spade[1], SPADE, s_count[1]); printf("\n");
    printf("Top   : "); PrintCards(spade[2], SPADE, s_count[2]); printf("\n");
    printf("Right : "); PrintCards(spade[3], SPADE, s_count[3]); printf("\n");
    printf("You   : "); PrintCards(spade[0], SPADE, s_count[0]); printf("\n");
    printf("\n");

    printf("Left  : "); PrintCards(heart[1], HEART, h_count[1]); printf("\n");
    printf("Top   : "); PrintCards(heart[2], HEART, h_count[2]); printf("\n");
    printf("Right : "); PrintCards(heart[3], HEART, h_count[3]); printf("\n");
    printf("You   : "); PrintCards(heart[0], HEART, h_count[0]); printf("\n");
    printf("\n");
}

void ReadHeartsMemory(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD StaticAddress = 0x0100d514;
    DWORD PointerArrayAddress;
    DWORD aArray[4];
    DWORD CardNum, CardKind;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)StaticAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 16, NULL);
    for (int i = 0 ; i < 4 ; i++)
        aArray[i] = (buffer[4*i] + 256*buffer[4*i + 1] + 256*256*buffer[4*i + 2] + 256*256*256*buffer[4*i + 3] + 0x1c);
            
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0; j < 13 ; j++)
        {
            PointerArrayAddress = ((int)aArray[i] + (16 * j));
            ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            CardNum = (buffer[0] / 4) + 1;  
            CardKind = (buffer[0] % 4) + 1;
            
            if (CardNum > 13)
                continue;
                
            if (CardKind == 1)
                CardKind = CLUB;
            else
            if (CardKind == 4)
                CardKind = SPADE;
            else
            if (CardKind == 2)
                CardKind = DIAMOND;

            if (CardNum == 1)
                CardNum = 14;
                
            Player[i][j][0] = CardKind;
            Player[i][j][1] = CardNum;            
        }
    }

    CloseHandle(handle);
}

void MaximizeCards(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[24];
    BYTE b1[4];
    BYTE b2[4];
    DWORD StaticAddress = 0x0100d514;
    DWORD PointerArrayAddress;
    DWORD aArray[4];
    DWORD cards[4][13];
    DWORD CardNum, CardKind;
    DWORD i;
    DWORD j;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)StaticAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 16, NULL);
    for (int i = 0 ; i < 4 ; i++)
        aArray[i] = (buffer[4*i] + 256*buffer[4*i + 1] + 256*256*buffer[4*i + 2] + 256*256*256*buffer[4*i + 3] + 0x1c);
            
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 13; j++)
        {
            cards[i][j] = 0;
        }
    }
    
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0; j < 13 ; j++)
        {
            PointerArrayAddress = ((int)aArray[i] + (16 * j));
            ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            
            if (buffer[0] == 0xff)
                continue;
            
            CardNum = buffer[0] >> 2;  
            CardKind = buffer[0] & 3;
            
            if (CardNum == 0)
                cards[CardKind][12] = PointerArrayAddress;
            else
                cards[CardKind][CardNum - 1] = PointerArrayAddress;
        }
    }
    
    CardKind = 5;

    for (int j = 0; j < 13 ; j++)
    {
        PointerArrayAddress = ((int)aArray[0] + (16 * j));
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) b1, 4, NULL);

        if (b1[0] == 0xff)
            continue;
            
        if (CardKind != (DWORD)(b1[0] & 3))
        {
            CardKind = b1[0] & 3;
            i = 13;
        }
        
        do
        {
            i--;
            if (cards[CardKind][i] != 0)
            {
                ReadProcessMemory(handle, (LPCVOID)cards[CardKind][i], (LPVOID) b2, 4, NULL);
                WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) b2, 4, NULL);
                WriteProcessMemory(handle, (LPCVOID)cards[CardKind][i], (LPVOID) b1, 4, NULL);
                if ((b1[0] >> 2) == 0)
                    cards[CardKind][12] = cards[CardKind][i];
                else
                    cards[CardKind][(b1[0] >> 2) - 1] = cards[CardKind][i];
                break;
            }
        }while (i > 0);              
    }

    CloseHandle(handle);
}

void MinimizeCards(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[24];
    BYTE b1[4];
    BYTE b2[4];
    DWORD StaticAddress = 0x0100d514;
    DWORD PointerArrayAddress;
    DWORD aArray[4];
    DWORD cards[4][13];
    DWORD CardNum, CardKind;
    DWORD i;
    DWORD j;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)StaticAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 16, NULL);
    for (int i = 0 ; i < 4 ; i++)
        aArray[i] = (buffer[4*i] + 256*buffer[4*i + 1] + 256*256*buffer[4*i + 2] + 256*256*256*buffer[4*i + 3] + 0x1c);
            
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 13; j++)
        {
            cards[i][j] = 0;
        }
    }
    
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0; j < 13 ; j++)
        {
            PointerArrayAddress = ((int)aArray[i] + (16 * j));
            ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            
            if (buffer[0] == 0xff)
                continue;
            
            CardNum = buffer[0] >> 2;  
            CardKind = buffer[0] & 3;
            
            if (CardNum == 0)
                cards[CardKind][12] = PointerArrayAddress;
            else
                cards[CardKind][CardNum - 1] = PointerArrayAddress;
        }
    }
    
    CardKind = 5;

    for (int j = 13; j > 0 ; j--)
    {
        PointerArrayAddress = ((int)aArray[0] + (16 * (j - 1)));
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) b1, 4, NULL);

        if (b1[0] == 0xff)
            continue;
            
        if (CardKind != (DWORD)(b1[0] & 3))
        {
            CardKind = b1[0] & 3;
            i = 0;
        }
        
        while (i < 13)
        {
            if (cards[CardKind][i] != 0)
            {
                ReadProcessMemory(handle, (LPCVOID)cards[CardKind][i], (LPVOID) b2, 4, NULL);
                WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) b2, 4, NULL);
                WriteProcessMemory(handle, (LPCVOID)cards[CardKind][i], (LPVOID) b1, 4, NULL);
                if ((b1[0] >> 2) == 0)
                    cards[CardKind][12] = cards[CardKind][i];
                else
                    cards[CardKind][(b1[0] >> 2) - 1] = cards[CardKind][i];
                i++;
                break;
            }
            
            i++;
        }              
    }

    CloseHandle(handle);
}


void WriteHeartsMemory(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD StaticAddress = 0x0100d514;
    DWORD PointerArrayAddress;
    DWORD aArray[4];
    DWORD CardNum, CardKind;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)StaticAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 16, NULL);
    for (int i = 0 ; i < 4 ; i++)
        aArray[i] = (buffer[4*i] + 256*buffer[4*i + 1] + 256*256*buffer[4*i + 2] + 256*256*256*buffer[4*i + 3] + 0x1c);
            
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0; j < 13 ; j++)
        {
            PointerArrayAddress = ((int)aArray[i] + (16 * j));
            ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 1, NULL);
            CardNum = (buffer[0] / 4) + 1;  
            CardKind = (buffer[0] % 4) + 1;
            if (CardKind != 1 || CardNum != 2 || i == 0)
            {
                buffer[0] = (BYTE)(j * 4 + i);
                PointerArrayAddress = ((int)aArray[i] + (16 * j));
                WriteProcessMemory(handle, (LPVOID)PointerArrayAddress, (LPVOID) buffer, 1, NULL);
            }
        }
    }

    CloseHandle(handle);
}

void ResetHeartsScore(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    BYTE b1[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};
    BYTE b2[7] = {0x89,0x8c,0x96,0x24,0x01,0x00,0x00};
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    DWORD k;
    DWORD l;    
    DWORD m;

    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b1, 7, NULL);

    ReadProcessMemory(handle, (LPCVOID)0x100D4F0, (LPVOID) buffer, 4, NULL);
    i = PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    PointerArrayAddress *= 4;
    PointerArrayAddress += 0x100D41C;

    for (; i > 0; i--)
    {    
         buffer[0]=0;buffer[1]=0;buffer[2]=0;buffer[3]=0;
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress -= 4;
    }

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);
    
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    buffer[0]=0;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress += (0x124 - 0x18);
    buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    for (i = 1; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    
    srand((DWORD)time(NULL));
    m = rand() % 3;
    j = rand() % 14;
    if (j == 14)
        j--;    
    for (i = 0; i < j; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 0)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);
   
    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    k = rand() % (14 - j);
    for (i = 0; i < k; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 1)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }


    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    l = (13 - j - k);
    for (i = 0; i < l; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 2)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);


    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    while (buffer[0]!=0xff || buffer[1] !=0xff || buffer[2]!=0xff || buffer[3]!=0xff)
    {
        if (ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL) == 0)
            break;
        Sleep(10);
    }
    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b2, 7, NULL);

    CloseHandle(handle);
}

/*
void ResetHeartsScore(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress;
        DWORD TempPointerArray;
        DWORD i;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D4F0, (LPVOID) buffer, 4, NULL);
    i = PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    PointerArrayAddress *= 4;
    PointerArrayAddress += 0x100D41C;

    for (; i > 0; i--)
    {    
         buffer[0]=0;buffer[1]=0;buffer[2]=0;buffer[3]=0;
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress -= 4;
    }

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);
    
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    buffer[0]=0;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress += (0x124 - 0x18);
    buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    for (i = 1; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);
   

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);                         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xC), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);                         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);
    

    CloseHandle(handle);
}
*/

void WinHearts(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    BYTE b1[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};
    BYTE b2[7] = {0x89,0x8c,0x96,0x24,0x01,0x00,0x00};
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    DWORD k;
    DWORD l;    
    DWORD m;

    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b1, 7, NULL);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);
    
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    for (i = 1; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }
    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }
    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }


    srand((DWORD)time(NULL));
    m = (rand() % 3) + 1;
    
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + (0x4 * m)), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    if ((75 - i) == 26)
        i = 76;
    else
        i = 75;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);

    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    while (buffer[0]!=0xff || buffer[1] !=0xff || buffer[2]!=0xff || buffer[3]!=0xff)
    {
        if (ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL) == 0)
            break;
        Sleep(10);
    }
    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b2, 7, NULL);

    CloseHandle(handle);
}

void AsciiTable (void)
{
    WORD i;
    
    for (i = 0;i < 256;i++)
        printf("%d %c ", i, (BYTE)i);
}

void Debug(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress1;
    DWORD PointerArrayAddress2;
    DWORD PointerArrayAddress3;
    DWORD PointerArrayAddress4;
    DWORD TempPointerArray;
    DWORD i;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress1 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress2 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress3 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 12), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress4 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    printf("    %s            %s           %s          %s\n\n","You","Left","Top","Right");
    for (i = 0; i < 14; i++)
    {    
         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress1, (LPVOID) buffer, 4, NULL);
         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress2, (LPVOID) &buffer[4], 4, NULL);
         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress3, (LPVOID) &buffer[8], 4, NULL);
         ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress4, (LPVOID) &buffer[12], 4, NULL);

         printf("%2X %2X %2X %2X || ",buffer[0],buffer[1],buffer[2],buffer[3]);
         printf("%2X %2X %2X %2X || ",buffer[4],buffer[5],buffer[6],buffer[7]);
         printf("%2X %2X %2X %2X || ",buffer[8],buffer[9],buffer[10],buffer[11]);
         printf("%2X %2X %2X %2X\n",buffer[12],buffer[13],buffer[14],buffer[15]);

         PointerArrayAddress1 += 4;
         PointerArrayAddress2 += 4;
         PointerArrayAddress3 += 4;
         PointerArrayAddress4 += 4;
    }
    
    CloseHandle(handle);
}

void DebugGUI(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress1;
    DWORD PointerArrayAddress2;
    DWORD PointerArrayAddress3;
    DWORD PointerArrayAddress4;
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    BYTE CardKind;
    char ItemString[8];
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress1 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress2 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress3 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 12), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress4 = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    printf("\n %s    %s   %s    %s\n\n","You","Left","Top","Right");
    for (i = 0; i < 14; i++)
    {    
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress1, (LPVOID) buffer, 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress2, (LPVOID) &buffer[1], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress3, (LPVOID) &buffer[2], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress4, (LPVOID) &buffer[3], 1, NULL);
        
        for (j = 0 ; j < 4 ; j++)
        {
            switch ((buffer[j] & 3))
            {
                case 0:
                    CardKind = CLUB;
                    break;
                case 1:
                    CardKind = DIAMOND;
                    break;
                case 2:
                    CardKind = HEART;
                    break;
                case 3:
                    CardKind = SPADE;
                    break;
                default:
                    break;
            }
            
            switch ((buffer[j] >> 2))
            {
                case 10:
                    sprintf(ItemString," %c J ",CardKind);
                    break;
                case 11:
                    sprintf(ItemString," %c Q ",CardKind);
                    break;
                case 12:
                    sprintf(ItemString," %c K ",CardKind);
                    break;
                case 0:
                    sprintf(ItemString," %c A ",CardKind);
                    break;
                default:
                    sprintf(ItemString," %c%2d ",CardKind,(buffer[j] >> 2) + 1);
                    break;
            }
            
            if (buffer[j] == 0xff)
                sprintf(ItemString," --- ");
                
            printf("%s", ItemString);
            
            if (j < 3)
                printf("||");
            else
                printf("\n");
        }
        
        PointerArrayAddress1 += 4;
        PointerArrayAddress2 += 4;
        PointerArrayAddress3 += 4;
        PointerArrayAddress4 += 4;
    }
    
    CloseHandle(handle);
}

void EatRaw(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress[4];
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    BYTE CardKind;
    char ItemString[8];
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[0] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[1] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[2] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 12), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[3] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    printf("\n       %s               %s              %s              %s\n\n","You","Left","Top","Right");

    for (i = 0; i < 14; i++)
    {    
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[0], (LPVOID) buffer, 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[1], (LPVOID) &buffer[1], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[2], (LPVOID) &buffer[2], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[3], (LPVOID) &buffer[3], 1, NULL);
        
        for (j = 0 ; j < 4 ; j++)
        {
            switch ((buffer[j] & 3))
            {
                case 0:
                    CardKind = CLUB;
                    break;
                case 1:
                    CardKind = DIAMOND;
                    break;
                case 2:
                    CardKind = HEART;
                    break;
                case 3:
                    CardKind = SPADE;
                    break;
                default:
                    break;
            }
            
            switch ((buffer[j] >> 2))
            {
                case 10:
                    sprintf(ItemString," %c J",CardKind);
                    break;
                case 11:
                    sprintf(ItemString," %c Q",CardKind);
                    break;
                case 12:
                    sprintf(ItemString," %c K",CardKind);
                    break;
                case 0:
                    sprintf(ItemString," %c A",CardKind);
                    break;
                default:
                    sprintf(ItemString," %c%2d",CardKind,(buffer[j] >> 2) + 1);
                    break;
            }
            
            if (buffer[j] == 0xff)
                sprintf(ItemString," ---");
                
            printf("%05X:%s (%02X) ", PointerArrayAddress[j], ItemString, buffer[j]);
            
            if (j < 3)
                printf("| ");          
        }

        printf("\n");
        
        PointerArrayAddress[0] += 4;
        PointerArrayAddress[1] += 4;
        PointerArrayAddress[2] += 4;
        PointerArrayAddress[3] += 4;
    }
    
    CloseHandle(handle);
}

void CardMap(void)
{
    BYTE buffer[4];
    DWORD i;
    DWORD j;
    BYTE CardKind;
    char ItemString[8];
    

    for (i = 0; i < 13; i++)
    {                    
        for (j = 0 ; j < 4 ; j++)
        {
            buffer[j] = ((i << 2) | j);

            switch ((buffer[j] & 3))
            {
                case 0:
                    CardKind = CLUB;
                    break;
                case 1:
                    CardKind = DIAMOND;
                    break;
                case 2:
                    CardKind = HEART;
                    break;
                case 3:
                    CardKind = SPADE;
                    break;
                default:
                    break;
            }
            
            switch ((buffer[j] >> 2))
            {
                case 10:
                    sprintf(ItemString," %c J",CardKind);
                    break;
                case 11:
                    sprintf(ItemString," %c Q",CardKind);
                    break;
                case 12:
                    sprintf(ItemString," %c K",CardKind);
                    break;
                case 0:
                    sprintf(ItemString," %c A",CardKind);
                    break;
                default:
                    sprintf(ItemString," %c%2d",CardKind,(buffer[j] >> 2) + 1);
                    break;
            }
            
            printf("%s (%02X) ", ItemString, buffer[j]);
            
            if (j < 3)
                printf("|");
            else
                printf("\n");
        }        
    }
}

void ReadRaw(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress[4];
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    BYTE CardKind;
    char ItemString[8];
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[0] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x1c);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[1] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x1c);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[2] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x1c);

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 12), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress[3] = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x1c);

    printf("\n       %s               %s              %s              %s\n\n","You","Left","Top","Right");

    for (i = 0; i < 13; i++)
    {    
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[0], (LPVOID) buffer, 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[1], (LPVOID) &buffer[1], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[2], (LPVOID) &buffer[2], 1, NULL);
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress[3], (LPVOID) &buffer[3], 1, NULL);
        
        for (j = 0 ; j < 4 ; j++)
        {
            switch ((buffer[j] & 3))
            {
                case 0:
                    CardKind = CLUB;
                    break;
                case 1:
                    CardKind = DIAMOND;
                    break;
                case 2:
                    CardKind = HEART;
                    break;
                case 3:
                    CardKind = SPADE;
                    break;
                default:
                    break;
            }
            
            switch ((buffer[j] >> 2))
            {
                case 10:
                    sprintf(ItemString," %c J",CardKind);
                    break;
                case 11:
                    sprintf(ItemString," %c Q",CardKind);
                    break;
                case 12:
                    sprintf(ItemString," %c K",CardKind);
                    break;
                case 0:
                    sprintf(ItemString," %c A",CardKind);
                    break;
                default:
                    sprintf(ItemString," %c%2d",CardKind,(buffer[j] >> 2) + 1);
                    break;
            }
            
            if (buffer[j] == 0xff)
                sprintf(ItemString," ---");
                
            printf("%05X:%s (%02X) ", PointerArrayAddress[j], ItemString, buffer[j]);
            
            if (j < 3)
                printf("| ");          
        }

        printf("\n");
        
        PointerArrayAddress[0] += 16;
        PointerArrayAddress[1] += 16;
        PointerArrayAddress[2] += 16;
        PointerArrayAddress[3] += 16;
    }
    
    CloseHandle(handle);
}

void MemoryPatcher(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer;
    DWORD PointerArrayAddress;
    DWORD i;
    DWORD j;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);
    
    printf("Address : ");
    scanf("%x", &PointerArrayAddress);
    
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);
    printf("Old Data: %08X\n", i);
    
    printf("Overwrite ? ");
    fflush(stdin);
    buffer = getchar();
    
    i = 10;
    if (buffer == 'Y' || buffer == 'y')
    {
        printf("Data    : ");
        scanf("%x",&j);
        while ((WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &j, 4, NULL) == 0) && (i > 0))
        {
            Sleep(50);
            i--;
        }
    }   
    
    if (i == 0)
        printf("Write Failed\n");

    CloseHandle(handle);
}

void MemoryFiller(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer;
    DWORD PointerArrayAddress;
    DWORD i;
    DWORD j;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);
    
    printf("Address : ");
    scanf("%x", &PointerArrayAddress);
    
    printf("Size    : ");
    scanf("%d", &i);
    
    printf("Data    : ");
    scanf("%x", &j);

    printf("Overwrite ? ");
    fflush(stdin);
    buffer = getchar();
       
    if (buffer == 'Y' || buffer == 'y')
    {
        while (i > 0)
        {
            if (WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &j, 4, NULL) == 0)
            {
                printf("Write Failed\n");
                break;
            }    
            i--;
            PointerArrayAddress += 4;
        }
    }   
    
    CloseHandle(handle);
}

void MemorySwap(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer;
    DWORD PointerArrayAddress1;
    DWORD PointerArrayAddress2;
    DWORD i;
    DWORD j;
    DWORD k;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);
    
    printf("Source : ");
    scanf("%x", &PointerArrayAddress1);
    fflush(stdin);
    printf("Dest   : ");
    scanf("%x", &PointerArrayAddress2);
    
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress1, (LPVOID) &i, 4, NULL);
    printf("Old Data Src  : %08X\n", i);
    j = i;
    
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress2, (LPVOID) &i, 4, NULL);
    printf("Old Data Dest : %08X\n", i);
    k = i;
    
    printf("Swap ? ");
    fflush(stdin);
    buffer = getchar();
    
    i = 10;
    if (buffer == 'Y' || buffer == 'y')
    {
        while ((WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress2, (LPVOID) &j, 4, NULL) == 0) && (i > 0))
        {
            Sleep(50);
            i--;
        }
        
        i = 10;
        while ((WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress1, (LPVOID) &k, 4, NULL) == 0) && (i > 0))
        {
            Sleep(50);
            i--;
        }
    }   
    
    if (i == 0)
        printf("Write Failed\n");

    CloseHandle(handle);
}

void MemoryReader(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    DWORD buffer[512];
    DWORD PointerArrayAddress;
    DWORD i;
    DWORD j;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);
    
    printf("Address : ");
    scanf("%x", &PointerArrayAddress);
    
    printf("Size    : ");
    scanf("%d",&j);
   
    if (j > 512)
        j = 512;
        
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, j * 4, NULL);
    
    for (i = 0; i < j; i++)
    {
        printf("%08X: %08X", PointerArrayAddress, buffer[i]);
        PointerArrayAddress += 4;
        if (((i + 1) % 4) == 0)
            printf("\n");
        else
            printf("  ");
    }
        
    CloseHandle(handle);
}

void StopHeartsScore(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    BYTE b1[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};
    BYTE b2[7] = {0x89,0x8c,0x96,0x24,0x01,0x00,0x00};
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    DWORD i;
    DWORD j;
    DWORD k;
    DWORD l;    
    DWORD m;

    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b1, 7, NULL);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);
    
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    for (i = 1; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    
    srand((DWORD)time(NULL));
    m = rand() % 3;
    j = rand() % 14;
    if (j == 14)
        j--;
    for (i = 0; i < j; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 0)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);
   
    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    k = rand() % (14 - j);
    for (i = 0; i < k; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 1)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }


    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    l = (13 - j - k);
    for (i = 0; i < l; i++)
    {    
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    }

    if (m == 2)
    {    
        buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;
        WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    }

    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x18);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    i = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3]);
    i -= 26;
    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &i, 4, NULL);


    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    while (buffer[0]!=0xff || buffer[1] !=0xff || buffer[2]!=0xff || buffer[3]!=0xff)
    {
        if (ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL) == 0)
            break;
        Sleep(10);
    }
    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b2, 7, NULL);

    CloseHandle(handle);
}

void ResumeHeartsScore(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
        BYTE b2[7] = {0x89,0x8c,0x96,0x24,0x01,0x00,0x00};


    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b2, 7, NULL);
}

void UndoCards(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    DWORD cards[14];
    DWORD i;
    DWORD j = 0;

    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    for (i = 0; i < 14; i++)
    {   
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &cards[j], 4, NULL); 
        if (cards[j] != 0xffffffff)
        {
            buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
            WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            j++;
        }
        else
            break;
        PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    for (i = 0; i < 14; i++)
    {   
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &cards[j], 4, NULL); 
        if (cards[j] != 0xffffffff)
        {
            buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
            WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            j++;
        }
        else
            break;
        PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);

    for (i = 0; i < 14; i++)
    {   
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &cards[j], 4, NULL); 
        if (cards[j] != 0xffffffff)
        {
            buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
            WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
            j++;
        }
        else
            break;
        PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124 + (13 * 4));

    for (i = 14; i > 0 && j > 0; i--)
    {   
        ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
        if (buffer[0] == 0xff)
        {
            j--;
            WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) &cards[j], 4, NULL);
        }
        PointerArrayAddress -= 4;
    }

    CloseHandle(handle);
}

void ForfeitHearts(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE replacement1[5] = {0x90,0x90,0x90,0x90,0x90};
    BYTE replacement2[5] = {0x90,0x90,0x90,0x90,0x90};
    BYTE replacement3[4] = {0x90,0x90,0x90,0x90};
    BYTE replacement4[3] = {0x90,0x90,0x90};
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x10087FB, (LPVOID) replacement1, 5, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x1008802, (LPVOID) replacement2, 5, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x10092D8, (LPVOID) replacement3, 4, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x10092E3, (LPVOID) replacement4, 3, NULL);
}

void NormalHearts(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE original1[5] = {0x83,0x44,0x95,0xb4,0xe6};
    BYTE original2[5] = {0x83,0x44,0x95,0xb4,0x1a};
    BYTE original3[4] = {0x83,0x41,0x18,0x0d};
    BYTE original4[3] = {0xff,0x41,0x18};
    
    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x10087FB, (LPVOID) original1, 5, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x1008802, (LPVOID) original2, 5, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x10092D8, (LPVOID) original3, 4, NULL);
    WriteProcessMemory(handle, (LPCVOID)0x10092E3, (LPVOID) original4, 3, NULL);

    CloseHandle(handle);
}

void ShootTheMoon(void)
{
    // Search the Hearts Process
    DWORD pid = GetProcessPidByName("mshearts.exe");
    HANDLE handle;
    BYTE buffer[256];
    BYTE b1[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};
    BYTE b2[7] = {0x89,0x8c,0x96,0x24,0x01,0x00,0x00};
    DWORD PointerArrayAddress;
    DWORD TempPointerArray;
    DWORD i;

    if (pid == -1)
       return;
    
    handle = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_READ, FALSE, pid);

    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b1, 7, NULL);

    ReadProcessMemory(handle, (LPCVOID)0x100D514, (LPVOID) buffer, 4, NULL);
    TempPointerArray = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x130);
    
    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);

    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0x2f;buffer[1]=0;buffer[2]=0;buffer[3]=0;

    WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress += 4;
    buffer[0]=0x2;buffer[1]=0;buffer[2]=0;buffer[3]=0;
    for (i = 1; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x4), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }
   
    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0x8), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }

    //====================================================================================================
    ReadProcessMemory(handle, (LPCVOID)(TempPointerArray + 0xc), (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    buffer[0]=0xff;buffer[1]=0xff;buffer[2]=0xff;buffer[3]=0xff;
    for (i = 0; i < 14; i++)
    {    
         WriteProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);
         PointerArrayAddress += 4;
    }


    ReadProcessMemory(handle, (LPCVOID)TempPointerArray, (LPVOID) buffer, 4, NULL);
    PointerArrayAddress = (buffer[0] + 256*buffer[1] + 256*256*buffer[2] + 256*256*256*buffer[3] + 0x124);
    ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL);

    while (buffer[0]!=0xff || buffer[1] !=0xff || buffer[2]!=0xff || buffer[3]!=0xff)
    {
        if (ReadProcessMemory(handle, (LPCVOID)PointerArrayAddress, (LPVOID) buffer, 4, NULL) == 0)
            break;
        Sleep(10);
    }
    WriteProcessMemory(handle, (LPCVOID)0x100923D, (LPVOID) b2, 7, NULL);

    CloseHandle(handle);
}

int main(int argc, char* argv[])
{
    if (argc > 1 && !stricmp(argv[1],"Moon"))
    {
        ShootTheMoon();
        //WriteHeartsMemory();
    }
    else if (argc > 1 && !stricmp(argv[1],"Read"))
    {
        ReadHeartsMemory();
        SortCards();
    }
    else if (argc > 1 && !stricmp(argv[1],"Win"))
    {
        WinHearts();
    }
    else if (argc > 1 && !stricmp(argv[1],"Normal"))
    {
        NormalHearts();
    }
    else if (argc > 1 && !stricmp(argv[1],"Reset"))
    {
        ResetHeartsScore();
    }
    else if (argc > 1 && !stricmp(argv[1],"Forfeit"))
    {
        ForfeitHearts();
    }
    else if (argc > 1 && !stricmp(argv[1],"Stop"))
    {
        StopHeartsScore();
        }
    else if (argc > 1 && !stricmp(argv[1],"Resume"))
    {
        ResumeHeartsScore();
    }
    else if (argc > 1 && !stricmp(argv[1],"Max"))
    {
        MaximizeCards();
    }
    else if (argc > 1 && !stricmp(argv[1],"Min"))
    {
        MinimizeCards();
    }
    else if (argc > 1 && !stricmp(argv[1],"Undo"))
    {
        UndoCards();
    }
    else if (argc > 1 && !stricmp(argv[1],"Eat"))
    {
        DebugGUI();
    }
    else if (argc > 1 && !stricmp(argv[1],"Mem"))
    {
        MemoryPatcher();
    }
    else if (argc > 1 && !stricmp(argv[1],"Fill"))
    {
        MemoryFiller();
    }
    else if (argc > 1 && !stricmp(argv[1],"Memread"))
    {
        MemoryReader();
    }
    else if (argc > 1 && !stricmp(argv[1],"Swap"))
    {
        MemorySwap();
    }
    else if (argc > 1 && !stricmp(argv[1],"Readraw"))
    {
        ReadRaw();
    }
    else if (argc > 1 && !stricmp(argv[1],"Eatraw"))
    {
        EatRaw();
    }
    else if (argc > 1 && !stricmp(argv[1],"Map"))
    {
        CardMap();
    }
    else
    {
        Debug();
        //AsciiTable();
    }

    return 0;
}
