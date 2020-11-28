#include <iostream>
#include <vector>
#include<stdlib.h>
#include<iomanip>
using namespace std;


void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
int process_num;
vector<int> process_id(10,0);
int burst_time;
int arrival_time;
int priority;
vector <int> arrival_time_list(10,0);
vector <int> burst_time_list(10,0);
vector<int> priority_list(10,0);

void getdata()
{

    cout<<"Enter number of Processes: "; cin>>process_num;
    cout<<"Enter Arrival Time for: \n";
    for(int i=0; i<process_num; i++)
    {
        cout<<"\nProcess "<<i+1<<": "; cin>>arrival_time;
        arrival_time_list[i] = arrival_time;
        process_id[i] = i+1;

    }

    cout<<"Enter Burst Time for: \n";
    for(int i=0; i<process_num; i++)
    {
        cout<<"\nProcess "<<i+1<<": "; cin>>burst_time;
        burst_time_list[i] = burst_time;
    }
}

void showdata()
{
    cout<<"\nProcess ID \tArrival Time \tBurst Time \tPriority\n";

    for(int i=0; i<process_num; i++)
    {
        cout<<"  "<<process_id[i]<<" \t\t  "<<arrival_time_list[i]<<" \t\t  "<<burst_time_list[i]<<" \t\t  "<<priority_list[i]<<endl;
    }
}


class Process
{
public:

    vector<int>wait_time;
    vector<int> tat_time;
    vector<int>service_time;
    vector<int>gantt_list;

    Process(): wait_time(10,0), tat_time(10,0), service_time(10,0){}

    void sort_result();
    void showresults();
    void sorted_data();
    void throughput();
    float avg_waiting_time();
    float avg_tat();
    void gantt_chart();
};


//Function to sort given data in increasing order of arrival time
void Process::sorted_data()
{
    int i, j;
        int n = process_num;
        for (i = 0; i < n-1; i++)
        {
            for (j = 0; j < n-i-1; j++)
            {
                if (arrival_time_list[j] > arrival_time_list[j+1])
                {   swap(&arrival_time_list[j], &arrival_time_list[j+1]);
                    swap(&process_id[j], &process_id[j+1]);
                    swap(&burst_time_list[j], &burst_time_list[j+1]);
                    swap(&priority_list[j], &priority_list[j+1]);
                }
            }
        }
}

void Process::sort_result()
{
    int i, j;
        int n = process_num;
        for (i = 0; i < n-1; i++)
        {
            for (j = 0; j < n-i-1; j++)
            {
                if (process_id[j] > process_id[j+1])
                {
                    swap(&process_id[j], &process_id[j+1]);
                    swap(&wait_time[j], &wait_time[j+1]);
                    swap(&tat_time[j], &tat_time[j+1]);
                }
            }
        }
}

//Function for calculating throughput
void Process::throughput()
{
    float sum =0;
    for(int i=0; i<process_num; i++)
    {
        sum+=burst_time_list[i];
    }
    float result = sum/process_num;
    cout<<"\nThroughput = "<<result;
}


void Process::showresults()
{
    cout<<"\n::::::::::::::::::::RESULTS:::::::::::::::::::::\n";
    cout<<"\nProcess ID \tWaiting Time \tTurnaround Time\n";
    for(int i=0; i<process_num; i++)
    {
        cout<<"  "<<process_id[i]<<"\t\t   "<<wait_time[i]<<"\t\t  "<<tat_time[i]<<endl;
    }
}

float Process::avg_waiting_time()
{
    float avg_wait = 0;

    for(int i=0; i<process_num; i++)
    {
        avg_wait += wait_time[i];
    }
   return avg_wait/process_num;
}

float Process::avg_tat()
{
    float avg_tat = 0;
     for(int i=0; i<process_num; i++)
    {
        avg_tat += tat_time[i];
    }
    return avg_tat/process_num;
}

//Function to show gantt chart
void Process::gantt_chart()
{
    cout<<"\n\n\t=======PROCESS GANTT CHART=======\n";


    for(auto i= gantt_list.begin(); i != gantt_list.end(); ++i)
    {
        cout<<"-----";
    }
    cout<<endl;

    for(auto i= gantt_list.begin(); i != gantt_list.end(); ++i)
    {
        cout<<"| P"<<*i<<" ";
    }
    cout<<"|"<<endl;

    for(auto i= gantt_list.begin(); i != gantt_list.end(); ++i)
    {
        cout<<"-----";
    }
}


class FCFS: public Process
{
public:

    void waiting_time();
    void tat();
};

//Function for calculating waiting time of each process
void FCFS::waiting_time()
{
    gantt_list.push_back(process_id[0]);
    service_time[0] = arrival_time_list[0];
    wait_time[0] = 0;


    // calculating waiting time
    for (int i = 1; i < process_num ; i++)
    {
        // Add burst time of previous processes
        service_time[i] = service_time[i-1] + burst_time_list[i-1];

        wait_time[i] = service_time[i] - arrival_time_list[i];

        if (wait_time[i] < 0)
            {
                wait_time[i] = 0;
            }
        gantt_list.push_back(process_id[i]);
    }
}

//Function for turnaround time in FCFS
void FCFS::tat()
{
    for(int i=0; i<process_num; i++)
    {
        tat_time[i] = burst_time_list[i] + wait_time[i];
    }
}


//Class declaration for Shortest Job First
class SJF: public Process
{
public:

    void waiting_time();
    void tat();
};

//Function to find waiting time of each process in SJF
void SJF::waiting_time()
{
     int n = process_num; int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = burst_time_list[i];

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;
// Process until all processes gets completed
    while (complete != n) {

        /* Find process with minimum remaining time among the processes that arrives till the current time*/
        for (int j = 0; j < n; j++) {
            if ((arrival_time_list[j] <= t) &&
            (rt[j] < minm) && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                gantt_list.push_back(process_id[shortest]);
                check = true;
            }
        }
        if (check == false) {
            t++;
            continue;
        }
       rt[shortest]--;
       minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;
       // If a process gets complete
        if (rt[shortest] == 0) {
            complete++;
            check = false;
            finish_time = t + 1;

            wait_time[shortest] = (finish_time - burst_time_list[shortest])- arrival_time_list[shortest];
            if (wait_time[shortest] < 0)
                wait_time[shortest] = 0;
        }
        t++;
    }
}
void SJF::tat()
{
    for (int i = 0; i < process_num; i++)
        tat_time[i] = burst_time_list[i] + wait_time[i];
}

//Class declaration for Priority Scheduling
class PR: public Process
{
public:

    void getpriority();
    void result();
};

//Getting priority of processes
void PR::getpriority()
{
    cout<<"\nEnter Priority for: \n";
    for(int i=0; i<process_num; i++)
    {
        cout<<"\nProcess "<<i+1<<": "; cin>>priority;
        cout<<"\n";
        priority_list[i] = priority;
    }
}

//Function to find waiting time and turnaround time of each processes for Priority Algorithm
void PR::result()
{
    int n = process_num; int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = burst_time_list[i];

    int complete = 0, t = 0, maxm;
    int finish_time;
    bool check = false;
    int k = -1;
    priority_list[9] = -1;
    while(complete != n)
    {
        maxm = 9;
        for(int j=0; j<n; j++)
        {
            if((arrival_time_list[j] <=t) &&
            (priority_list[j]>priority_list[maxm]) && rt[j] > 0) {
                maxm = j;
                check = true;
            }
        }

        if(check==false) {
            t++;
            continue;
        }
        if(maxm!=k)
            gantt_list.push_back(process_id[maxm]);
        rt[maxm]--;
        k = maxm;
        if(rt[maxm]==0) {
            complete++;
            check = false;
            finish_time = t+1;
            wait_time[maxm] = finish_time - burst_time_list[maxm] - arrival_time_list[maxm];
            tat_time[maxm] = burst_time_list[maxm] + wait_time[maxm];
            if(wait_time[maxm]<0)
                wait_time[maxm] = 0;
        }
        t++;
    }
}

//Class Declaration for Round Robin
class RR: public Process
{
public:

    int time_quan;
    void get_time_quant();
    void result();
    RR(){time_quan = 3;}
};

//Function to get time quantum
void RR::get_time_quant()
{
    cout<<"\nEnter time quantum: "; cin>>time_quan;
}

//Function for calculating waiting time and turnaround time
void RR::result()
{
      int n = process_num;
      int rem_time[n];
      int completion_time[n];

      for(int i = 0; i < n; i++)
        rem_time[i] = burst_time_list[i];

      int t = arrival_time_list[0];
     /* Processing until the value of element of rem_time array is 0*/
      while(true)
      {
        bool done = true;
        for(int i = 0; i < n; i++)
        {
          if(rem_time[i] > 0)
          {
            done = false;
            if(rem_time[i] > time_quan && arrival_time_list[i] <= t)
            {
              t += time_quan;
              rem_time[i] -= time_quan;
            }
            else
            {
              if(rem_time[i]<=time_quan && arrival_time_list[i] <= t)
              {
                t += rem_time[i];
                rem_time[i] = 0;
                completion_time[i] = t;
              }
            }
            gantt_list.push_back(process_id[i]);
          }
          wait_time[i] = completion_time[i] - arrival_time_list[i] - burst_time_list[i];
          tat_time[i] = completion_time[i] - arrival_time_list[i];

        }
        if(done==true)
          {break;}
      }
}


void compare_algos(FCFS p1, SJF p2, PR p3, RR p4)
{
    setprecision(2);
    p1.sorted_data();
    p1.waiting_time();
    p1.tat();
    cout<<"\n\n\t\t====COMPARISON TABLE====\n";
    cout<<"\n----------------------------------------------------------";
    cout<<"\n ALGORITHM |Average Waiting Time |Average Turnaround Time|";
    cout<<"\n----------------------------------------------------------";
    cout<<"\n  FCFS     |\t\t"<<p1.avg_waiting_time()<<"\t  | \t  "<<p1.avg_tat()<<"\t\t |";
    cout<<"\n----------------------------------------------------------";

    p2.sorted_data();
    p2.waiting_time();
    p2.tat();
    cout<<"\n  SJF      |\t\t"<<p2.avg_waiting_time()<<"\t  | \t  "<<p2.avg_tat()<<"\t\t |";
    cout<<"\n----------------------------------------------------------";

    p3.sorted_data();
    p3.result();
    cout<<"\n Priority  |\t\t"<<p3.avg_waiting_time()<<"\t  | \t  "<<p3.avg_tat()<<"\t\t |";
    cout<<"\n----------------------------------------------------------";

    p4.sorted_data();
    p4.result();
    cout<<"\nRound Robin|\t\t"<<p4.avg_waiting_time()<<"\t  | \t  "<<p4.avg_tat()<<"\t\t |";
    cout<<"\n----------------------------------------------------------";
}

int main()
{
    system("Color 70");
    setprecision(2);

    FCFS p1;
    SJF p2;
    PR p3;
    RR p4;

    int choice1;
    char choice2 = 'y';
    while(choice2 == 'y'||'Y')
    {
        system("cls");
        cout<<"::::::::::::::::::::CPU SCHEDULING ALGORITHM SIMULATOR::::::::::::::::::::::\n";
        cout<<"\nChoose one of the following algorithms:\n";
        cout<<"\n\t1.FIRST COME FIRST SERVE\n";
        cout<<"\n\t2.SHORTEST JOB FIRST\n";
        cout<<"\n\t3.PRIORITY SCHEDULING\n";
        cout<<"\n\t4.ROUND ROBIN SCHEDULING\n";
        cout<<"\n\t5.EXIT\n";
        cout<<"\n\tEnter your choice(1-5): "; cin>>choice1;
        system("cls");

        switch(choice1)
        {
            case 1: {cout<<"\nYou chose FCFS Scheduling...\n";
                    getdata();

                    system("cls");
                    cout<<":::::::::::::::::FCFS:::::::::::::::::\n";
                    cout<<"\nYou entered following deltails: \n";
                    showdata();
                    p1.sorted_data();
                    p1.waiting_time();
                    p1.tat();
                    p1.sort_result();
                    p1.showresults();
                    p1.throughput();
                    cout<<"\nAverage Waiting Time is: "<<p1.avg_waiting_time();
                    cout<<"\nAverage Turnaround Time is: "<<p1.avg_tat()<<endl;
                    p1.gantt_chart();
                    compare_algos(p1,p2,p3,p4);
                    p1.gantt_list.clear();

                    break;}

            case 2: { cout<<"\nYou chose SJF Scheduling...\n";
                    getdata();

                    system("cls");
                    cout<<":::::::::::::::::SJF:::::::::::::::::\n";
                    cout<<"\nYou entered following deltails: \n";
                    showdata();
                    p2.sorted_data();
                    p2.waiting_time();
                    p2.tat();
                    p2.sort_result();
                    p2.showresults();
                    p2.throughput();
                    cout<<"\nAverage Waiting Time is: "<<p2.avg_waiting_time();
                    cout<<"\nAverage Turnaround Time is: "<<p2.avg_tat()<<endl;
                    p2.gantt_chart();
                    compare_algos(p1,p2,p3,p4);
                    p2.gantt_list.clear();

                    break;}

            case 3:
                    cout<<"\nYou chose Priority Scheduling...\n";
                    getdata();
                    p3.getpriority();
                    system("cls");
                    cout<<":::::::::::::::::PRIORITY:::::::::::::::::\n";
                    cout<<"\nYou entered following deltails: \n";
                    showdata();
                    p3.sorted_data();

                    p3.result();
                    p3.sort_result();
                    p3.showresults();
                    p3.throughput();
                    cout<<"\nAverage Waiting Time is: "<<p3.avg_waiting_time();
                    cout<<"\nAverage Turnaround Time is: "<<p3.avg_tat()<<endl;
                    p3.gantt_chart();
                    compare_algos(p1,p2,p3,p4);
                    p3.gantt_list.clear();

                    break;

            case 4:cout<<"\nYou chose Round Robin Scheduling...\n";
                    getdata();
                    p4.get_time_quant();
                    system("cls");
                    cout<<":::::::::::::::::ROUND ROBIN:::::::::::::::::\n";
                    cout<<"\nYou entered following deltails: \n";
                    showdata();
                    cout<<"\nTime Quantum = "<<p4.time_quan<<endl;
                    p4.sorted_data();
                    p4.result();
                    p4.sort_result();

                    p4.showresults();
                    p4.throughput();
                    cout<<"\nAverage Waiting Time is: "<<p4.avg_waiting_time();
                    cout<<"\nAverage Turnaround Time is: "<<p4.avg_tat()<<endl;
                    p4.gantt_chart();
                    compare_algos(p1,p2,p3,p4);
                    p4.gantt_list.clear();

                    break;
            case 5:  exit(0);
                     break;
            default: cout<<"\nPlease Enter Valid Choice!!\n";
        }
       cout<<"\n\nDo you want to continue(Y/N)? "; cin>>choice2;
       if(choice2 == 'y'|| choice2=='Y')
        continue;
       if(choice2 == 'n'|| choice2=='N')
        break;
    }
    return 0;
    cin.get();
}
