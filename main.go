package main

import (
	"encoding/hex"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Printf("Usage: %s <PE file>\n", os.Args[0])
		os.Exit(1)
	}

	peFile := os.Args[1]

	// ファイルを読み込む
	data, err := os.ReadFile(peFile)
	if err != nil {
		fmt.Printf("Error reading file: %v\n", err)
		os.Exit(1)
	}

	// バイナリデータをシェルコード形式に変換
	shellcode := "\\x" + hex.EncodeToString(data)
	shellcode = formatShellcode(shellcode)

	fmt.Printf("Shellcode:\n%s\n", shellcode)
}

// シェルコードを適切にフォーマットする
func formatShellcode(sc string) string {
	result := ""
	for i := 0; i < len(sc); i += 2 {
		result += "\\" + sc[i:i+2]
	}
	return result
}
