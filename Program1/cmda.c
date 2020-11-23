#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//t,r의 chipsequence를 입력하는 함수 a*2-1은 a가 0일때 -1, 1일때 1이 된다.
void setSequence(int *sequence){
    for(int i = 0;i<8;i++){
        scanf("%d", &sequence[i]);
        sequence[i] = sequence[i]*2-1;
    }
}

//Joiner프로세스에서 사용할 함수로 전달받은 signal에 따라 a인지 not a인지 계산하는 함수
void calSequence(int t, int *sequence_t, int *sequence){
    for(int i = 0;i<8;i++){
        if(t==0){
            sequence[i]+=(-1)*sequence_t[i];
        }else if(t==1){
            sequence[i]+=sequence_t[i];
        }
    }
}

//수신기에서 Joiner로부터 전달받은 sequence로부터 signal을 구하는 함수
int restoreSignal(int *receive, int *sequence){
    int s=0;
    for(int i = 0;i<8;i++){
        s+=receive[i]*sequence[i];
    }
    return s/8;
}

int main(){
    //파이프 TXtoJoiner(송신기->Joiner), JoinertoRX(Joiner->수신기)
    int pipeT0toJoiner[2],pipeT1toJoiner[2],pipeT2toJoiner[2],pipeT3toJoiner[2];
    int pipeJoinertoR0[2],pipeJoinertoR1[2],pipeJoinertoR2[2],pipeJoinertoR3[2];

    //0~3의 chipsequence 크기는 8이다.
    int chipsequence0[8],chipsequence1[8],chipsequence2[8],chipsequence3[8];
    //수신기에서의 sequence를 저장할 배열
    int R0receive[8],R1receive[8],R2receive[8],R3receive[8];
    //Joiner는 송신기로부터 1개씩의 signal을, 수신기로 sequence를 보낸다.
    int JoinerReceive[4]={0,}, JoinerSend[8]={0,};
    //chipsequence입력
    printf("Enter t0,r0 chipsequence: ");
    setSequence(chipsequence0);
    printf("Enter t1,r1 chipsequence: ");
    setSequence(chipsequence1);
    printf("Enter t2,r2 chipsequence: ");
    setSequence(chipsequence2);
    printf("Enter t3,r3 chipsequence: ");
    setSequence(chipsequence3);

    //자식프로세스들을 위한 pid, 4개의 수신기 프로세스 Joiner프로세스 4개의 송신기 프로세스
    int T0pid,T1pid,T2pid,T3pid;
    int R0pid,R1pid,R2pid,R3pid;
    int Joinerpid;
    //송신기에서 보낼 signal
    int T0send,T1send,T2send,T3send;

    //pipe 생성 예외처리
    if(pipe(pipeT0toJoiner)==-1||pipe(pipeT1toJoiner)==-1||pipe(pipeT2toJoiner)==-1||pipe(pipeT3toJoiner)==-1){
        printf("pipe error");
        exit(1);
    }
    if(pipe(pipeJoinertoR0)==-1||pipe(pipeJoinertoR1)==-1||pipe(pipeJoinertoR2)==-1||pipe(pipeJoinertoR3)==-1){
        printf("pipe error");
        exit(1);
    }

    //각 송신기에서의 보낼 signal입력
    printf("T0 signal(-1 mean do not send): ");
    scanf("%d", &T0send);
    printf("T1 signal(-1 mean do not send): ");
    scanf("%d", &T1send);
    printf("T2 signal(-1 mean do not send): ");
    scanf("%d", &T2send);
    printf("T3 signal(-1 mean do not send): ");
    scanf("%d", &T3send);

    //자식프로세스
    T0pid=fork();
    T1pid=fork();
    T2pid=fork();
    T3pid=fork();
    if(T0pid==0){//0번 송신기
        write(pipeT0toJoiner[1],&T0send,sizeof(T0send));
        exit(0);
    }else if(T1pid==0){//1번 송신기
        write(pipeT1toJoiner[1],&T1send,sizeof(T1send));
        exit(0);
    }else if(T2pid==0){//2번 송신기
        write(pipeT2toJoiner[1],&T2send,sizeof(T2send));
        exit(0);
    }else if(T3pid==0){//3번 송신기
        write(pipeT3toJoiner[1],&T3send,sizeof(T3send));
        exit(0);
    }else{//부모프로세스, sleep은 대기시간 0.05초
        sleep(0.05);
        Joinerpid=fork();//Joiner 프로세스
        if(Joinerpid==0){
            sleep(0.05);
            //t0~t3로부터 입력받은 bit를 JoinerReceive에 저장한다.
            read(pipeT0toJoiner[0],&JoinerReceive[0],sizeof(int));
            read(pipeT1toJoiner[0],&JoinerReceive[1],sizeof(int));
            read(pipeT2toJoiner[0],&JoinerReceive[2],sizeof(int));
            read(pipeT3toJoiner[0],&JoinerReceive[3],sizeof(int));

            //입력받은 bit가 0이거나 1일 때 수신기에 보낼 sequence에 연산을 한다.
            if(JoinerReceive[0]==0||JoinerReceive[0]==1){
                calSequence(JoinerReceive[0],chipsequence0,JoinerSend);
            }
            if(JoinerReceive[1]==0||JoinerReceive[1]==1){
                calSequence(JoinerReceive[1],chipsequence1,JoinerSend);
            }
            if(JoinerReceive[2]==0||JoinerReceive[2]==1){
                calSequence(JoinerReceive[2],chipsequence2,JoinerSend);
            }
            if(JoinerReceive[3]==0||JoinerReceive[3]==1){
                calSequence(JoinerReceive[3],chipsequence3,JoinerSend);
            }
            //수신기에 보낼 Joiner Sequence출력
            printf("Joiner Sequence: ");
            for(int i = 0;i<8;i++){
                printf("%d",JoinerSend[i]);
            }
            printf("\n");

            //수신기에 sequence를 보낸다.
            write(pipeJoinertoR0[1],JoinerSend,sizeof(JoinerSend));
            write(pipeJoinertoR1[1],JoinerSend,sizeof(JoinerSend));
            write(pipeJoinertoR2[1],JoinerSend,sizeof(JoinerSend));
            write(pipeJoinertoR3[1],JoinerSend,sizeof(JoinerSend));

            //수신기 프로세스
            R0pid=fork();
            R1pid=fork();
            R2pid=fork();
            R3pid=fork();

            if(R0pid==0){//r0
                //신호를 받은 뒤 chipsequence에 대해 bit를 구한다.
                read(pipeJoinertoR0[0],R0receive,sizeof(R0receive));
                int restore0 = restoreSignal(R0receive,chipsequence0);
                //구한 bit를 출력한다. (bit+1)/2는 bit가 -1일때 0, 1일때 1이 된다.
                if(restore0==0){
                    printf("r0 do not receive signal!!\n");
                }else{
                    printf("r0 receive %d!!\n",(restore0+1)/2);
                }
            }else if(R1pid==0){//r1
                //신호를 받은 뒤 chipsequence에 대해 bit를 구한다.
                read(pipeJoinertoR1[0],R1receive,sizeof(R1receive));
                int restore1 = restoreSignal(R1receive,chipsequence1);
                //구한 bit를 출력한다. (bit+1)/2는 bit가 -1일때 0, 1일때 1이 된다.
                if(restore1==0){
                    printf("r1 do not receive signal!!\n");
                }else{
                    printf("r1 receive %d!!\n",(restore1+1)/2);
                }
            }else if(R2pid==0){//r2
                //신호를 받은 뒤 chipsequence에 대해 bit를 구한다.
                read(pipeJoinertoR2[0],R2receive,sizeof(R2receive));
                int restore2 = restoreSignal(R2receive,chipsequence2);
                //구한 bit를 출력한다. (bit+1)/2는 bit가 -1일때 0, 1일때 1이 된다.
                if(restore2==0){
                    printf("r2 do not receive signal!!\n");
                }else{
                    printf("r2 receive %d!!\n",(restore2+1)/2);
                }
            }else if(R3pid==0){//r3
                //신호를 받은 뒤 chipsequence에 대해 bit를 구한다.
                read(pipeJoinertoR3[0],R3receive,sizeof(R3receive));
                int restore3 = restoreSignal(R3receive,chipsequence3);
                //구한 bit를 출력한다. (bit+1)/2는 bit가 -1일때 0, 1일때 1이 된다.
                if(restore3==0){
                    printf("r3 do not receive signal!!\n");
                }else{
                    printf("r3 receive %d!!\n",(restore3+1)/2);
                }
            }
        }
    }
    exit(0);
    return 0;
}