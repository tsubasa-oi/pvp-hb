#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// グローバル変数を定義
int firstNum;
int secondNum;
int thirdNum;
int forthNum;

// 4桁のランダムな数値を生成する
void fourNumberGen(){
    srand((unsigned)time(NULL));
    firstNum = rand() % 10;

    // 4桁の重複しない乱数を生成
    do {
        secondNum = rand() % 10;
    } while (firstNum == secondNum);

    do {
        thirdNum = rand() % 10;
    } while (firstNum == thirdNum | secondNum == thirdNum);

    do {
        forthNum = rand() % 10;
    } while (firstNum == forthNum | secondNum == forthNum | thirdNum == forthNum);

    // //確認用
     printf("4つのランダムな整数を出力しました:%d,%d,%d,%d\n", firstNum,secondNum,thirdNum,forthNum);
}

// 数当てを判定する関数
void hitAndBrow(const char *client1Choice, const char *client2Choice, char *result1P, char *result2P) {
    // 使う変数を一旦定義しておく
    int arraySize = 4;
    int total;
    int inputArray1P[4];
    int inputArray2P[4];
    int hit1P;
    int brow1P;
    int hit2P;
    int brow2P;
    int count = 0;

    // プレイヤー1の処理を実行
    int client1Num;
    sscanf(client1Choice, "%d", &client1Num);
    printf("プレイヤーAは%dと入力しました\n", client1Num);

    inputArray1P[0] = client1Num / 1000;
    inputArray1P[1] = (client1Num % 1000) / 100;
    inputArray1P[2] = (client1Num % 100) / 10;
    inputArray1P[3] = client1Num % 10;

    // プレイヤー2の処理を実行
    int client2Num;
    sscanf(client2Choice, "%d", &client2Num);
    printf("プレイヤーBは%dと入力しました\n", client2Num);

    inputArray2P[0] = client2Num / 1000;
    inputArray2P[1] = (client2Num % 1000) / 100;
    inputArray2P[2] = (client2Num % 100) / 10;
    inputArray2P[3] = client2Num % 10;

    // hitとbrowの値を初期化しておく
    hit1P = 0;
    brow1P = 0;
    hit2P = 0;
    brow2P = 0;

    // プレイヤー1から、hitしていれば値を足す
    // 的中していればhitの値を追加する
        for (int i = 0 ; i < arraySize ; i++){
            if(inputArray1P[i] == firstNum){
                hit1P++;
                break;
            }
        }
        for (int i = 0 ; i < arraySize ; i++){
            if(inputArray1P[i] == secondNum){
                hit1P++;
                break;
            }
        }
        for (int i = 0 ; i < arraySize ; i++){
            if(inputArray1P[i] == thirdNum){
                hit1P++;
                break;
            }
        }
        for (int i = 0 ; i < arraySize ; i++){
            if(inputArray1P[i] == forthNum){
                hit1P++;
                break;
            }
        }

    // プレイヤー2で、hitしていれば値を足す
        for (int j = 0 ; j < arraySize ; j++){
            if(inputArray2P[j] == firstNum){
                hit2P++;
                break;
            }
        }
        for (int j = 0 ; j < arraySize ; j++){
            if(inputArray2P[j] == secondNum){
                hit2P++;
                break;
            }
        }
        for (int j = 0 ; j < arraySize ; j++){
            if(inputArray2P[j] == thirdNum){
                hit2P++;
                break;
            }
        }
        for (int j = 0 ; j < arraySize ; j++){
            if(inputArray2P[j] == forthNum){
                hit2P++;
                break;
            }
        }

    // それぞれのbrow数を計算
    brow1P = arraySize - hit1P;
    brow2P = arraySize - hit2P;

    if (hit1P && hit2P == 4){
        strcpy(result1P, "引き分けです\n");
        strcpy(result2P, "引き分けです\n");
    } else if(hit1P == 4){
        strcpy(result1P, "あなたの勝利です\n");
        strcpy(result2P, "あなたの負けです\n");
    } else if(hit2P == 4){
        strcpy(result1P, "あなたの負けです\n");
        strcpy(result2P, "あなたの勝利です\n");
    } else {
        sprintf(result1P, "hit:%d brow:%dです\n", hit1P, brow1P);
        sprintf(result2P, "hit:%d brow:%dです\n", hit2P, brow2P);
    }

}

int main() {
    int serverSocket, client1Socket, client2Socket;
    struct sockaddr_in serverAddr, client1Addr, client2Addr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // 4桁の数値を生成します。
    fourNumberGen();

    // ソケットを作成
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("ソケットの作成に失敗しました");
        exit(EXIT_FAILURE);
    }

    // サーバーアドレスを設定
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // ソケットにバインド
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("バインドに失敗しました");
        exit(EXIT_FAILURE);
    }

    // 接続待機
    if (listen(serverSocket, 2) == -1) {
        perror("接続待機に失敗しました");
        exit(EXIT_FAILURE);
    }

    printf("クライアントの接続を待っています...\n");

    // 1つ目のクライアントを受け入れ
    client1Socket = accept(serverSocket, (struct sockaddr *)&client1Addr, &addrSize);
    printf("クライアント1が接続しました\n");

    // 2つ目のクライアントを受け入れ
    client2Socket = accept(serverSocket, (struct sockaddr *)&client2Addr, &addrSize);
    printf("クライアント2が接続しました\n");

    // ここからループして処理
    do {
    // クライアントからの数値を受信
    char client1Choice[BUFFER_SIZE], client2Choice[BUFFER_SIZE];
    recv(client1Socket, &client1Choice, sizeof(client1Choice), 0);
    recv(client2Socket, &client2Choice, sizeof(client2Choice), 0);

    // 勝者を判定
    char result1P[BUFFER_SIZE];
    char result2P[BUFFER_SIZE];
    hitAndBrow(client1Choice, client2Choice, result1P, result2P);

    // 結果をクライアントに送信
    if (send(client1Socket, result1P, strlen(result1P) + 1, 0) == -1) {
        perror("client1Socket send error");
        // エラー処理を追加するか、適切に対処する必要があります
    }

    if (send(client2Socket, result2P, strlen(result2P) + 1, 0) == -1) {
        perror("client2Socket send error");
        // エラー処理を追加するか、適切に対処する必要があります
    }


    // クライアントに再プレイの意志を問い合わせ
    char playAgain1, playAgain2;
    recv(client1Socket, &playAgain1, sizeof(char), 0);
    recv(client2Socket, &playAgain2, sizeof(char), 0);

    if (playAgain1 != 'y' && playAgain1 != 'Y' && playAgain2 != 'y' && playAgain2 != 'Y') {
            break;  // 終了
        }

    

    } while (1);

    // ソケットを閉じる
    close(client1Socket);
    close(client2Socket);
    close(serverSocket);

    return 0;
}
