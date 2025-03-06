#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int *maxx;
    int *allocated;
    int *need;
}process_data;

int main(){
    //looping variables.
    int i,j;

    //accptingt the number of processes and resources.
    int process_num,resource_num;
    
    printf("enter the number of process : \n");
    scanf("%d",&process_num);
    printf("enter number of resources : \n");
    scanf("%d",&resource_num);

    //stores data of available proccess.
    int available[resource_num];
    for(i = 0 ; i < resource_num ; i++){
        printf("enter the available resource %d \n",i+1);
        scanf("%d",&available[i]);
    }

    //creating array of struct.
    process_data pd[process_num];

    for(i = 0; i < process_num ; i++){

        pd[i].maxx = (int *)malloc(resource_num * sizeof(int));
        pd[i].allocated = (int *)malloc(resource_num * sizeof(int));
        pd[i].need = (int *)malloc(resource_num * sizeof(int));

        for (j = 0; j < resource_num; j++)
        {
            printf("enter the maxx and allocated resouce %d for process %d \n",j+1,i+1);
            scanf("%d%d",&pd[i].maxx[j],&pd[i].allocated[j]);
            pd[i].need[j] = pd[i].maxx[j] - pd[i].allocated[j];    
        }

    }

    //updates the updated matrix.
    int updated_available[resource_num];
    for (j = 0; j < resource_num; j++) {
        updated_available[j] = available[j];
        for (i = 0; i < process_num; i++) {
            updated_available[j] -= pd[i].allocated[j];
        }
    }

    //cretaing active array
    int active_processes[process_num];
    int sum ;
    for(i = 0 ; i < process_num ; i++){
        sum = 0;
        for(j = 0 ; j < resource_num ; j++){
            sum += pd[i].need[j];
        }
        if(sum != 0){//proceeses ateast required one resource.
            active_processes[i] = 1;
        }
        else
        {
            active_processes[i]= 0;
        }
    }

    //calculate the count of completed process.
    int count = 0 ;
    for(i = 0 ; i < process_num ; i++){
        if(active_processes[i] == 0){
            printf("p%d\n",i);
            count++;
        }
    }

    int flag1,flag2;
    do{
        flag2 = 1;
        for(i = 0 ; i < process_num ; i++){
            flag1 = 0 ;
            if(active_processes[i] == 1){
                for(j = 0 ; j < resource_num ; j++){
                    if(updated_available[j] >= pd[i].need[j])
                    {
                        flag1++;
                    }
                }
                if(flag1 == resource_num){
                    for(int k = 0; k < resource_num ; k++){
                        printf("p%d\n",i);
                        updated_available[k] += pd[i].allocated[k];
                        active_processes[i] = 0;//makes the processes as in active
                        count++; 
                        flag2 = 0;
                        break;//to again start over from i = 0;
                    }
                }
            }
        }
    }while(!flag2);//no avaiable resources for any of the active processes hence terminate.

    if(count == process_num){
        printf("deadlock doesn't occur.");
    }
    else
    {
        printf("deadlock occured.");
    }

     // Free allocated memory
     for (i = 0; i < process_num; i++) {
        free(pd[i].maxx);
        free(pd[i].allocated);
        free(pd[i].need);
    }
    return 0;
}
