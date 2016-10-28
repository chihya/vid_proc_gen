#define _CRT_SECURE_NO_WARNINGS
#define _MBCS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <stdio.h>

BOOL vpFileExists(LPCSTR fileName);

long vpGetFileLength(FILE *);

void vpCreateFile(LPCSTR fileName);

DWORD GetHardwareSerialNumber();

BOOL vpCheckRegisterStatus(LPCSTR fileName);

void vpPatchRegInfo(LPCSTR fileName);

int pow(int, int);

inline BOOL getYN() {
    int c = getchar();
    return c == 10 || c == 13 || c == 'Y' || c == 'y';
}

int main() {
    auto programFile = "VidProc.exe";
    if (!vpFileExists(programFile)) {
        printf("Please place this application to the directory containing '%s'.\n", programFile);
        return 1;
    }
    auto regFile = "yuvxtra.sec";
    if (!vpFileExists(regFile)) {
        printf("The licensing file '%s' doesn't exist. Do you want to create one? [Y/n]\n", regFile);
        if (getYN()) {
            vpCreateFile(regFile);
        } else {
            printf("Licensing file creation aborted. Exiting.\n");
            return 0;
        }
    }
    auto b = vpCheckRegisterStatus(regFile);
    printf("License validity: %s\n", b ? "Valid" : "INVALID");
    if (!b) {
        printf("Current license is invalid. Do you want to patch the license? [Y/n]\n");
        if (getYN()) {
            vpPatchRegInfo(regFile);
        } else {
            printf("Patching aborted.\n");
        }
    }
    return 0;
}

#define DEF_CH(n) int c##n
#define DEF_V(n) int v##n

BOOL vpCheckRegisterStatus(LPCSTR fileName) {
    FILE *fp;
    DEF_CH(1);
    DEF_CH(2);
    DEF_CH(3);
    DEF_CH(4);
    DEF_CH(5);
    DEF_CH(6);
    DEF_CH(7);
    DEF_CH(8);
    DEF_CH(9);
    DEF_V(1);
    DEF_V(2);
    DEF_V(3);
    DEF_V(4);
    DEF_V(5);
    DEF_V(6);
    DEF_V(7);
    DEF_V(8);
    DEF_V(9);
    DEF_V(10);
    DEF_V(11);

#define READ_CHAR(n) c##n = fgetc(fp)

    fp = fopen(fileName, "r");
    if (!fp) {
        return FALSE;
    }

    if (vpGetFileLength(fp) < 410) {
        fclose(fp);
        return FALSE;
    }

    // Dummy info
    for (auto i = 0; i < 400; ++i) {
        fgetc(fp);
    }


    READ_CHAR(1);
    //v1 = 100000000 * c1 - 505032704; // ?
    v1 = 100000000 * (c1 - '0');
    READ_CHAR(2);
    v2 = 10000000 * (c2 - '0') + v1;
    READ_CHAR(3);
    v3 = 1000000 * (c3 - '0') + v2;
    READ_CHAR(4);
    v4 = 100000 * (c4 - '0') + v3;
    READ_CHAR(5);
    v5 = 10000 * (c5 - '0') + v4;
    READ_CHAR(6);
//    v6 = 125 * (c6 - '0');
//    v7 = v5 + 8 * v6;
    v7 = v5 + 1000 * (c6 - '0');
    READ_CHAR(7);
//    v8 = 25 * (c7 - '0');
//    v9 = v7 + 4 * v8;
    v9 = v7 + 100 * (c7 - '0');
    READ_CHAR(8);
    v10 = v9 + 10 * (c8 - '0');
    READ_CHAR(9);
    v11 = v10 + (c9 - '0');
#undef READ_CHAR
    fclose(fp);

    auto hardwareSerial = GetHardwareSerialNumber();
//    v11 = v11 - 100000000;
//    BOOL result = v11 - hardwareSerial / 10 == 1010101;
    BOOL result = v11 - hardwareSerial / 10 - 101010101 == 0;
    return result;
}

#undef DEF_CH
#undef DEF_V

void vpPatchRegInfo(LPCSTR fileName) {
    auto fp = fopen(fileName, "r+");
    if (!fp) {
        return;
    }

    fseek(fp, 400, SEEK_SET);
    auto hardwareSerial = GetHardwareSerialNumber();
    auto regCode = (int)hardwareSerial / 10 + 101010101;
    auto div = pow(10, 8);
    while (div > 0) {
        auto v = regCode / div;
        v %= 10;
        v += '0';
        fputc(v, fp);
        div /= 10;
    }

    fclose(fp);

    printf("Patching done.\n");
}

DWORD GetHardwareSerialNumber() {
    DWORD dwMachineSerial = 0, dwMaxComponentLength = 0xff;
    GetVolumeInformation("C:\\", nullptr, 0, &dwMachineSerial, &dwMaxComponentLength, nullptr, nullptr, 0);
    return dwMachineSerial;
}

BOOL vpFileExists(LPCSTR fileName) {
    auto fp = fopen(fileName, "r");
    if (!fp) {
        return FALSE;
    } else {
        fclose(fp);
        return TRUE;
    }
}

long vpGetFileLength(FILE *fp) {
    auto pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    auto v = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return v;
}

void vpCreateFile(LPCSTR fileName) {
    if (vpFileExists(fileName)) {
        return;
    }
    auto fp = fopen(fileName, "w+");
    fclose(fp);
}

int pow(int base, int power) {
    if (power < 0) {
        return 0;
    } else {
        auto v = 1;
        while (power > 0) {
            v *= base;
            --power;
        }
        return v;
    }
}
