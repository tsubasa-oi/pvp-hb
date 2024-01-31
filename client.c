#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr;

    // ソケットを作成
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("ソケットの作成に失敗しました");
        exit(EXIT_FAILURE);
    }

    // サーバーアドレスを設定
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // サーバーのIPアドレス
    serverAddr.sin_port = htons(PORT);

    // サーバーに接続
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("接続に失敗しました");
        exit(EXIT_FAILURE);
    }

    do {
        // 選択を入力
        char selectNumber[5];
        printf("あなたはプレーヤーAです\n");
        printf("数値を4桁で入力してください: ");
        scanf(" %4s", &selectNumber);

        // 選択をサーバーに送信
        send(clientSocket, &selectNumber, sizeof(selectNumber), 0);

        // 結果を受信
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("結果: %s\n", buffer);

        // もう一度プレイするかどうかを尋ねる
        char playAgain;
        printf("もう一度プレイしますか？ (y/n): ");
        scanf(" %c", &playAgain);

        // サーバーにプレイの意志を送信
        send(clientSocket, &playAgain, sizeof(char), 0);

        if (playAgain != 'y' && playAgain != 'Y') {
            break;  // もうプレイしない場合、終了
        }

    } while (1);

    // ソケットを閉じる
    close(clientSocket);

    return 0;
}
