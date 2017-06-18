#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc , char** argv)
{
    int size , rank , n ,*arr , i, Psize;

    MPI_Init(&argc , &argv);

    MPI_Comm_size(MPI_COMM_WORLD , &size);
    MPI_Comm_rank(MPI_COMM_WORLD , &rank);

    if(rank==0)
    {
        printf("Enter Array Size:  ");
        scanf("%d" , &n);

        arr=malloc(n*sizeof(int));

        printf("Enter Array Elements \n");

        for(i=0 ;i<n ;i++)
             scanf("%d" , &arr[i]);

        Psize=n/(size-1);

        int idx=-1 ;

        for(i=1 ; i<size ;i++)
        {
            idx=(i-1)*Psize;
            MPI_Send(&Psize , 1 ,MPI_INT , i , 0 , MPI_COMM_WORLD);
            MPI_Send(&arr[idx] , Psize , MPI_INT ,i , 0 , MPI_COMM_WORLD );
        }

        int mx =0;

        int rem = 0 ;
        rem =  n%(size-1) ;
	
		//handle reminder
        if(rem!=0)
        {
            i=Psize*(size-1);

            for(i ;i<n ;i++)
            {
                if(arr[i]>mx)
                    mx=arr[i] ;
            }
           // printf("Here \n");
        }

        int temp;
        for(i=1 ;i<size ;i++)
        {
            MPI_Recv(&temp , 1 , MPI_INT , i , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);

            if(mx<temp)
                mx=temp;
        }

        printf("Maximum number in array = %d \n" , mx);

    }
    else
    {
        int ps ;
        MPI_Recv(&ps , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE );
        arr=malloc(ps*sizeof(int));
        MPI_Recv(arr , ps , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);

        int mx = arr[0];
        i=0 ;

        for(i ;i<ps ;i++)
        {
            if(arr[i]>mx)
                mx=arr[i];
        }

        MPI_Send(&mx , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);


    }

  //  MPI_Finalize();
    MPI_Finalize();

}
