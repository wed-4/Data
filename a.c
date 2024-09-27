#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// シェルコードを実行する関数ポインタ型
typedef void (*shellcode_func)();

int main(int argc, char *argv[]) {
    // 引数のチェック
    if (argc != 2) {
        fprintf(stderr, "使い方: %s <シェルコードファイル>\n", argv[0]);
        return 1;
    }

    // ファイルからシェルコードを読み取る
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("ファイルが開けません");
        return 1;
    }

    // シェルコードを格納するバッファ
    char shellcode[256];
    if (fgets(shellcode, sizeof(shellcode), file) == NULL) {
        perror("シェルコードの読み取りに失敗しました");
        fclose(file);
        return 1;
    }
    fclose(file);

    // シェルコードの長さを計算
    size_t shellcode_len = strlen(shellcode) / 4; // 1つのバイトは4文字（例: "\x31"）

    // 実行可能なメモリ領域を確保（読み書き実行可能）
    void *mem = mmap(0, shellcode_len, PROT_READ | PROT_WRITE | PROT_EXEC, 
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        perror("メモリ確保に失敗しました");
        return 1;
    }

    // シェルコードをバイナリに変換してメモリにコピー
    unsigned char *sc_ptr = (unsigned char *)mem;
    for (size_t i = 0; i < shellcode_len; i++) {
        sscanf(&shellcode[i * 4 + 2], "%2hhx", &sc_ptr[i]); // "\x31" -> 0x31
    }

    // シェルコードを実行
    ((shellcode_func)mem)();

    // メモリを解放
    munmap(mem, shellcode_len);

    return 0;
}
