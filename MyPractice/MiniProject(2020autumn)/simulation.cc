#include "util/util.h"

#include "queue.h"
#include "priority_queue.h"

int cmp(const Customer& a, const Customer& b)
{
    if (a.leave_time < b.leave_time)
        return -1;
    else if (a.leave_time > b.leave_time)
        return 1;
    else
        return 0;
}

double simulate(const size_t queue_capacity, const size_t num_servers, Queue<Customer>& arrival_flow)
{
    Queue<Customer> queue;
    Priority_Queue<Customer> leave_flow;

    int num_customers = 0;
    double total_queue_time = 0;

    Queue<Customer>::init(queue, queue_capacity);
    Priority_Queue<Customer>::init(leave_flow, num_servers, cmp);

    while (!Queue<Customer>::empty(arrival_flow) || !Queue<Customer>::empty(queue) || !Priority_Queue<Customer>::empty(leave_flow))
    {
        Customer customer_in, customer_out;
        Queue<Customer>::top(arrival_flow, customer_in);
        Priority_Queue<Customer>::top(leave_flow, customer_out);
        if (Priority_Queue<Customer>::empty(leave_flow) || ( !Queue<Customer>::empty(arrival_flow)&&(customer_in.arrive_time <= customer_out.leave_time)))
        {
            Queue<Customer>::dequeue(arrival_flow, customer_in);
            if (Queue<Customer>::full(queue))
                continue;
            ++num_customers;
            if (!Priority_Queue<Customer>::full(leave_flow))
            {
                customer_in.leave_time = customer_in.arrive_time + customer_in.service_time;
                Priority_Queue<Customer>::enqueue(leave_flow, customer_in);
            }
            else
                Queue<Customer>::enqueue(queue, customer_in);
        }
        else if (Queue<Customer>::empty(arrival_flow) || customer_in.arrive_time >= customer_out.leave_time)
        {
            Priority_Queue<Customer>::dequeue(leave_flow, customer_out);
            total_queue_time += (customer_out.leave_time - customer_out.arrive_time - customer_out.service_time);
            if (!Queue<Customer>::empty(queue))
            {
                Queue<Customer>::dequeue(queue, customer_in);
                customer_in.leave_time = customer_out.leave_time + customer_in.service_time;
                Priority_Queue<Customer>::enqueue(leave_flow, customer_in);
            }
        }
    }

    Queue<Customer>::destroy(arrival_flow);
    Queue<Customer>::destroy(queue);
    Priority_Queue<Customer>::destroy(leave_flow);

    return total_queue_time / num_customers;
}
