#include <iostream>
#include <deque> 
using namespace std;

void printKMax(int arr[], int n, int k){
    deque<int> dq;
    int i, j;
    dq.push_back(0);
    for (i=1; i<k; i++)
    {
        int idx = dq.back();
        while (arr[idx] <= arr[i])
        {
            dq.pop_back();
            if (dq.empty())
                break;
            idx = dq.back();
        }
        dq.push_back(i);
    }

    // printf("%d ", arr[dq[0]]);
    cout << arr[dq[0]] << " ";

    for (i=k; i<n; i++)
    {
        if (k==1)
        {
            printf("%d ", arr[i]);
            continue;
        }
        if (dq[0] < (i - (k-1)) )
            dq.pop_front();

        int idx = dq.back();
        while (arr[idx] <= arr[i])
        {
            // printf("%d <= %d \n", idx, i);
            dq.pop_back();
            if (dq.empty())
                break;
            idx = dq.back();
        }
        dq.push_back(i);
        printf("%d ", arr[dq[0]]);
    }

    printf("\n");

}

int main(){
  
	int t;
	cin >> t;
	while(t>0) {
		int n,k;
    	cin >> n >> k;
    	int i;
    	int arr[n];
    	for(i=0;i<n;i++)
      		cin >> arr[i];
    	printKMax(arr, n, k);
    	t--;
  	}
  	return 0;
}
