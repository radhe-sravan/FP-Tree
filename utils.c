#include "header.h"

void partition(int  *array,int *frequency,int low,int high)
{
    	int mid;

    	if(low<high)
	{
         	mid=(low+high)/2;
         	partition(array,frequency,low,mid);
         	partition(array,frequency,mid+1,high);
         	mergeSort(array,frequency,low,mid,high);
    	}
}

void mergeSort(int *array,int *frequency,int low,int mid,int high)
{

    	int i,m,k,l,*temp;
	temp = malloc(sizeof(int)*75);

    	l=low;
    	i=low;
    	m=mid+1;

    	while((l<=mid)&&(m<=high))
	{
         	if(frequency[array[l]]>=frequency[array[m]])
		{
	             	temp[i]=array[l];
             		l++;
         	}
         	else
		{
             		temp[i]=array[m];
             		m++;
         	}
         	i++;
    	}

    	if(l>mid)
	{
         	for(k=m;k<=high;k++)
		{
	             	temp[i]=array[k];
             		i++;
         	}
    	}
    	else
	{
         	for(k=l;k<=mid;k++)
		{
             		temp[i]=array[k];
             		i++;
         	}
    	}
   
    	for(k=low;k<=high;k++)
	{
	         array[k]=temp[k];
    	}
}
